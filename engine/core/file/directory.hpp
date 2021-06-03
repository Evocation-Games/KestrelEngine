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

#if !defined(KESTREL_DIRECTORY_HPP)
#define KESTREL_DIRECTORY_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/file/file.hpp"

namespace host
{

    struct directory: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<host::directory> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        std::string m_path;

    public:
        explicit directory(const std::string& path);

        lua_api static auto get(const std::string& path) -> directory::lua_reference;

        lua_api static auto exists(const std::string& path) -> bool;

        lua_api auto path() const -> std::string;
        lua_api auto name() const -> std::string;
        lua_api auto contents(bool include_dot_files) const -> std::vector<host::file::lua_reference>;
    };

}

#endif //KESTREL_DIRECTORY_HPP
