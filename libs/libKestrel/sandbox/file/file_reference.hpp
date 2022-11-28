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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/sandbox/file/resource_file_reference.hpp>

namespace kestrel::sandbox
{

    struct lua_api(Kestrel.Sandbox.FileReference, Available_0_8) file_reference
    {
    public:
        has_constructable_lua_api(file_reference);

        explicit file_reference(const std::string& path);

        lua_function(createParentDirectory, Available_0_8) auto create_parent_directory() -> void;
        lua_function(touch, Available_0_8) auto touch() -> void;

        lua_getter(exists, Available_0_8) [[nodiscard]] auto exists() const -> bool;
        lua_getter(is_directory, Available_0_8) [[nodiscard]] auto is_directory() const -> bool;
        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_getter(path, Available_0_8) [[nodiscard]] auto path() const -> std::string;
        lua_getter(extension, Available_0_8) [[nodiscard]] auto extension() const -> std::string;
        lua_getter(basename, Available_0_8) [[nodiscard]] auto basename() const -> std::string;

        lua_function(openResourceFile, Available_0_8) auto open_resource_file() const -> resource_file_reference::lua_reference;

    private:
        std::string m_path;
    };

}
