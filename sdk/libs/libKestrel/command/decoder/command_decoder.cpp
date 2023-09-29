// Copyright (c) 2022 Tom Hancocks
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

#include <libKestrel/command/decoder/command_decoder.hpp>
#include <libKestrel/command/argument.hpp>

// MARK: - Enumerations

namespace kestrel::command
{
    enum flags : std::uint8_t
    {
        none = 0,
        has_children = 1,
        go_to_sibling = 2,
        terminate_to_parent_sibling = 4,
        fully_terminate = 8
    };
}

// MARK: - Construction

kestrel::command::decoder::decoder(const resource::reader::lua_reference &reader)
{
    parse(reader);
}

auto kestrel::command::decoder::from(const resource::reader::lua_reference &reader) -> lua_reference
{
    return { new decoder(reader) };
}

// MARK: - Errors

auto kestrel::command::decoder::has_error() const -> bool
{
    return (!m_error.empty()) && (!m_valid);
}

auto kestrel::command::decoder::error_message() const -> std::string
{
    return m_error;
}

// MARK: - Compilation

auto kestrel::command::decoder::compile_program() const -> program::lua_reference
{
    return { new program(m_program_nodes) };
}

// MARK: - Parsing

auto kestrel::command::decoder::parse(const resource::reader::lua_reference &reader) -> void
{
    std::vector<node::lua_reference> parents;

    while (reader->bytes_available()) {
        if (reader->read_byte() != 0) {
            m_valid = false;
            return;
        }

        auto name_hash = reader->read_quad();
        auto argument_count = reader->read_byte();
        auto flags = static_cast<enum flags>(reader->read_byte());

        command::node::lua_reference node(nullptr);
        if (!parents.empty()) {
            node = { new command::node(name_hash, parents.back().get()) };
        }
        else {
            node = { new command::node(name_hash) };
        }

        for (auto n = 1; n <= argument_count; ++n) {
            auto type = static_cast<argument::type>(reader->read_byte());
            switch (type) {
                case argument::key_path: {
                    node->add_argument(argument(type, reader->read_quad()));
                    break;
                }
                case argument::less_than:
                case argument::less_than_or_equal:
                case argument::more_than:
                case argument::more_than_or_equal:
                case argument::equal_to:
                case argument::not_equal_to: {
                    node->add_argument(argument(type, std::vector<std::int64_t>(reader->read_signed_quad())));
                    break;
                }
                case argument::inside: {
                    node->add_argument(argument(type, std::vector<std::int64_t>({ reader->read_signed_quad(), reader->read_signed_quad() })));
                    break;
                }
            }
        }

        m_program_nodes.emplace_back(node);

        if (flags & flags::has_children) {
            parents.emplace_back(node);
        }
        else if (flags & flags::go_to_sibling) {

        }
        else if (flags & flags::terminate_to_parent_sibling) {
            parents.pop_back();
        }
        else if (flags & flags::fully_terminate) {
            parents.clear();
        }
    }
}
