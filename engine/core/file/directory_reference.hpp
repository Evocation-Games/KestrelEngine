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

#if !defined(KESTREL_DIRECTORY_REFERENCE_HPP)
#define KESTREL_DIRECTORY_REFERENCE_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "util/lua_vector.hpp"
#include "core/file/file_reference.hpp"

namespace host::sandbox
{

    struct directory_reference: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<directory_reference> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        std::string m_path;

    public:
        explicit directory_reference(const std::string& path);

        lua_api static auto get(const std::string& path) -> directory_reference::lua_reference;

        lua_api static auto check_path_exists(const std::string& path) -> bool;
        [[nodiscard]] lua_api auto exists() const -> bool;

        [[nodiscard]] lua_api auto path() const -> std::string;
        [[nodiscard]] lua_api auto name() const -> std::string;
        [[nodiscard]] lua_api auto contents(bool include_dot_files) const -> util::lua_vector<file_reference::lua_reference>;
        [[nodiscard]] lua_api auto file(const std::string& file) const -> file_reference::lua_reference;
        [[nodiscard]] lua_api auto directory(const std::string& dir) const -> directory_reference::lua_reference;
    };

}

#endif //KESTREL_DIRECTORY_HPP
