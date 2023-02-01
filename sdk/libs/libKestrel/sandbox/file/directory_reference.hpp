// Copyright (c) 2021 Tom Hancocks
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

#include <string>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/sandbox/file/file_reference.hpp>

namespace kestrel::sandbox
{
    struct lua_api(Kestrel.Sandbox.DirectoryReference, Available_0_8) directory_reference
    {
    public:
        has_constructable_lua_api(directory_reference);

        directory_reference() = default;
        explicit directory_reference(const std::string& path);

        lua_function(usingPath, Available_0_8) static auto get(const std::string& path) -> lua_reference;

        lua_function(checkPathExists, Available_0_8) static auto check_path_exists(const std::string& path) -> bool;
        lua_getter(exists, Available_0_8) [[nodiscard]] auto exists() const -> bool;

        lua_getter(parent, Available_0_8) [[nodiscard]] auto parent() const -> lua_reference;
        lua_getter(path, Available_0_8) [[nodiscard]] auto path() const -> std::string;
        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_function(contents, Available_0_8) [[nodiscard]] auto contents(bool include_dot_files) const -> lua::vector<file_reference::lua_reference>;
        lua_function(file, Available_0_8) [[nodiscard]] auto file(const std::string& file) const -> file_reference::lua_reference;
        lua_function(directory, Available_0_8) [[nodiscard]] auto directory(const std::string& dir) const -> lua_reference;

        lua_function(createDirectory, Available_0_8) auto create_directory(bool intermediates = true) -> void;

    private:
        std::string m_path;
    };
}
