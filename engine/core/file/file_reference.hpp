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

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/file/resource_file_reference.hpp"

namespace host::sandbox
{

    struct file_reference: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<host::sandbox::file_reference> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit file_reference(const std::string& path);

        lua_api auto create_parent_directory() -> void;
        lua_api auto touch() -> void;

        [[nodiscard]] lua_api auto exists() const -> bool;
        [[nodiscard]] lua_api auto is_directory() const -> bool;
        [[nodiscard]] lua_api auto name() const -> std::string;
        [[nodiscard]] lua_api auto path() const -> std::string;
        [[nodiscard]] lua_api auto extension() const -> std::string;
        [[nodiscard]] lua_api auto basename() const -> std::string;

        lua_api auto open_resource_file() const -> resource_file_reference::lua_reference;

    private:
        std::string m_path;

    };

}
