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

#include <dirent.h>
#include "core/file/directory.hpp"


// MARK: - Lua

auto host::directory::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Host")
            .beginClass<host::directory>("Directory")
                .addStaticFunction("exists", &host::directory::exists)
                .addProperty("name", &host::directory::name)
                .addProperty("path", &host::directory::path)
                .addFunction("contents", &host::directory::contents)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

host::directory::directory(const std::string &path)
{
    if (path[path.size() - 1] == '/') {
        m_path = path.substr(0, path.size() - 1);
    }
    else {
        m_path = path;
    }
}

auto host::directory::get(const std::string &path) -> host::directory::lua_reference
{
    return exists(path) ? new directory(path) : nullptr;
}

// MARK: - Accessors

auto host::directory::path() const -> std::string
{
    return m_path;
}

// MARK: - Directory Operations

auto host::directory::exists(const std::string &path) -> bool
{
    return true;
}

auto host::directory::name() const -> std::string
{
    auto pos = m_path.find_last_of('/');
    return m_path.substr(pos + 1);
}

auto host::directory::contents(bool include_dot_files) const -> std::vector<host::file::lua_reference>
{
    std::vector<host::file::lua_reference> contents;

    DIR *dir;
    struct dirent *ent;
    std::cout << m_path << std::endl;
    if ((dir = opendir(m_path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string name(ent->d_name);
            if (name == "." || name == "..") {
                continue;
            }
            if (!include_dot_files && name[0] == '.') {
                continue;
            }

            std::string file_path(m_path + "/");
            file_path.append(name);
            contents.emplace_back( new host::file(file_path) );
        }
        closedir(dir);
    }

    return contents;
}
