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

#pragma once

#include <memory>
#include <string>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/reader.hpp>
#include <libKestrel/command/node.hpp>
#include <libKestrel/command/program.hpp>

namespace kestrel::command
{
    class lua_api(Kestrel.Command.Decoder, Available_0_9) decoder
    {
    public:
        has_constructable_lua_api(decoder);

        explicit decoder(const resource::reader::lua_reference& reader);

        lua_function(from, Available_0_9) [[nodiscard]] static auto from(const resource::reader::lua_reference& reader) -> lua_reference;

        lua_getter(didError, Available_0_9) [[nodiscard]] auto has_error() const -> bool;
        lua_getter(errorMessage, Available_0_9) [[nodiscard]] auto error_message() const -> std::string;

        lua_function(compile, Available_0_9) [[nodiscard]] auto compile_program() const -> program::lua_reference;

    private:
        bool m_valid { false };
        std::string m_error;
        std::vector<node::lua_reference> m_program_nodes;
        program::lua_reference m_program { nullptr };

        auto parse(const resource::reader::lua_reference& reader) -> void;
    };
}