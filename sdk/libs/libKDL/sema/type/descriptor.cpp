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

#include <libKDL/sema/type/descriptor.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/spec/types.hpp>

auto kdl::sema::type_definition::descriptor::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> resource::definition::type::descriptor
{
    std::optional<std::string> type_name;
    std::vector<std::string> hints;
    bool is_reference = false;

    if (stream.expect({ expectation(tokenizer::identifier).be_true() })) {
        type_name = stream.read().string_value();
        if (stream.expect({ expectation(tokenizer::l_angle).be_true() })) {
            stream.advance();
            while (stream.expect({ expectation(tokenizer::r_angle).be_false() })) {
                hints.emplace_back(stream.read().string_value());

                if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                    stream.advance();
                    continue;
                }
                else if (stream.expect({ expectation(tokenizer::r_angle).be_true() })) {
                    break;
                }
            }
            stream.ensure({ expectation(tokenizer::r_angle).be_true() });
        }
    }
    else if (stream.expect({ expectation(tokenizer::string_type).be_true() })){
        type_name = spec::types::string;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::integer_type).be_true() })){
        type_name = spec::types::integer;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::bitmask_type).be_true() })){
        type_name = spec::types::bitmask;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::file_type).be_true() })){
        type_name = spec::types::file;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::image_type).be_true() })){
        type_name = spec::types::image;
        auto tk = stream.read();
        hints = tk.associated_values();
    }

    if (stream.expect({ expectation(tokenizer::amp).be_true() })) {
        stream.advance();
        is_reference = true;
    }

    if (type_name.has_value()) {
        return { is_reference, type_name.value(), hints };
    }
    else {
        return resource::definition::type::descriptor(is_reference);
    }
}

auto kdl::sema::type_definition::descriptor::infer_type(context &ctx) -> resource::definition::type::descriptor
{

}