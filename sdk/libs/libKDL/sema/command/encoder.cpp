// Copyright (c) 2023 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <libKDL/sema/command/encoder.hpp>
#include <libData/writer.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

auto kdl::sema::command_encoder::parse(foundation::stream<tokenizer::token> &stream, sema::context &ctx) -> data::block
{
    data::writer encoder(data::byte_order::msb);
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    // Consume the command and encode it as a series of "packets". These packets are then encoded as a Binary Blob.
    // We need to keep parsing/consuming until we reach a closing brace.
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        // Get the name of the request/attribute.
        if (!stream.expect({ expectation(tokenizer::identifier).be_true() })) {
            throw diagnostic(stream.peek(), diagnostic::reason::KDL001);
        }

        // Leading NULL byte of the packet.
        encoder.write_byte(0);

        std::vector<std::pair<std::uint8_t, std::vector<__int128>>> arguments;
        auto name = stream.read();
        auto name_hash = foundation::hashing::string(name.string_value());
        encoder.write_quad(name_hash);

        // Do we have arguments (for a request), or not?
        if (stream.expect({ expectation(tokenizer::l_paren).be_true() })) {
            stream.advance();

            // Read the arguments for the request, and build a list of them.
            std::string argument;
            while (true) {
                if (stream.expect_any({ expectation(tokenizer::comma).be_true(), expectation(tokenizer::r_paren).be_true() })) {
                    if (!argument.empty()) {
                        arguments.emplace_back(static_cast<std::uint8_t>(0), std::vector({
                            static_cast<__int128>(foundation::hashing::string(argument))
                        }));
                    }

                    argument = "";
                    if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                        stream.advance();
                        continue;
                    }
                    break;
                }
                else if (stream.expect({
                    expectation(tokenizer::integer).be_true(),
                    expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(),
                    expectation(tokenizer::integer).be_true()
                })) {
                    auto v1 = static_cast<__int128>(stream.read().value<std::int64_t>()); stream.advance(3);
                    auto v2 = static_cast<__int128>(stream.read().value<std::int64_t>());
                    arguments.emplace_back(static_cast<std::uint8_t>(7), std::vector({ v1, v2 }));
                }
                else if (stream.expect({
                    expectation(tokenizer::integer).be_true(),
                    expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(),
                })) {
                    auto v1 = static_cast<__int128>(stream.read().value<std::int64_t>()); stream.advance(3);
                    arguments.emplace_back(static_cast<std::uint8_t>(1), std::vector({ v1 }));
                }
                else if (stream.expect({
                    expectation(tokenizer::integer).be_true(),
                    expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(),
                })) {
                    auto v1 = static_cast<__int128>(stream.read().value<std::int64_t>()); stream.advance(2);
                    arguments.emplace_back(static_cast<std::uint8_t>(2), std::vector({ v1 }));
                }
                else if (stream.expect({
                    expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(),
                    expectation(tokenizer::integer).be_true(),
                })) {
                    stream.advance(3);
                    auto v1 = static_cast<__int128>(stream.read().value<std::int64_t>());
                    arguments.emplace_back(static_cast<std::uint8_t>(5), std::vector({ v1 }));
                }
                else if (stream.expect({
                    expectation(tokenizer::dot).be_true(), expectation(tokenizer::dot).be_true(),
                    expectation(tokenizer::integer).be_true(),
                })) {
                    stream.advance(2);
                    auto v1 = static_cast<__int128>(stream.read().value<std::int64_t>());
                    arguments.emplace_back(static_cast<std::uint8_t>(6), std::vector({ v1 }));
                }
                else if (stream.expect({ expectation(tokenizer::integer).be_true() })) {
                    auto v1 = static_cast<__int128>(stream.read().value<std::int64_t>());
                    arguments.emplace_back(static_cast<std::uint8_t>(3), std::vector({ v1 }));
                }
                else if (stream.expect({ expectation(tokenizer::tilde).be_true(), expectation(tokenizer::integer).be_true() })) {
                    stream.advance(1);
                    auto v1 = static_cast<__int128>(~stream.read().value<std::int64_t>());
                    arguments.emplace_back(static_cast<std::uint8_t>(4), std::vector({ v1 }));
                }
                else {
                    argument += stream.read().string_value();
                }
            }
            stream.ensure({ expectation(tokenizer::r_paren).be_true() });
        }

        encoder.write_byte(static_cast<std::uint8_t>(arguments.size()));

        // Check how we are terminating this...
        std::uint8_t flags = 0;
        if (stream.expect({ expectation(tokenizer::semi).be_true(), expectation(tokenizer::semi).be_true() })) {
            // Terminate
            flags = 8;
            stream.advance(2);
        }
        else if (stream.expect({ expectation(tokenizer::semi).be_true() })) {
            // Got to parent sibling
            flags = 4;
            stream.advance();
        }
        else if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
            // Go to sibling
            flags = 2;
            stream.advance();
        }
        else if (stream.expect({ expectation(tokenizer::dot).be_true() })) {
            // Has children
            flags = 1;
            stream.advance();
        }

        encoder.write_byte(flags);

        // Encode the arguments
        for (const auto& arg : arguments) {
            encoder.write_byte(arg.first);
            if (arg.first == 0) {
                encoder.write_quad(arg.second.at(0));
            }
            else {
                for (const auto& v : arg.second) {
                    encoder.write_signed_quad(static_cast<std::int64_t>(v));
                }
            }
        }
    }

    stream.ensure({ expectation(tokenizer::r_brace).be_true() });

    // We have finished encoding the command packets.
    return std::move(*const_cast<data::block *>(encoder.data()));
}
