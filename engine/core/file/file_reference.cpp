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

#include <sys/stat.h>
#include "core/file/file_reference.hpp"
#include "core/file/directory_reference.hpp"

// MARK: - Lua

auto host::sandbox::file_reference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Kestrel")
            .beginNamespace("Sandbox")
                .beginClass<file_reference>("FileReference")
                    .addProperty("exists", &file_reference::exists)
                    .addProperty("name", &file_reference::name)
                    .addProperty("path", &file_reference::path)
                    .addProperty("basename", &file_reference::basename)
                    .addProperty("extension", &file_reference::extension)
                    .addFunction("openResourceFile", &file_reference::open_resource_file)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

host::sandbox::file_reference::file_reference(const std::string &path)
    : m_path(path)
{
    assert(path[path.size() - 1] != '/');
}

// MARK: - Accessors

auto host::sandbox::file_reference::name() const -> std::string
{
    auto pos = m_path.find_last_of('/');
    return m_path.substr(pos + 1);
}

auto host::sandbox::file_reference::path() const -> std::string
{
    return m_path;
}

auto host::sandbox::file_reference::extension() const -> std::string
{
    auto name = this->name();
    auto pos = name.find_last_of('.');
    if (pos == std::string::npos || pos == name.size() - 1) {
        return "";
    }
    else {
        return name.substr(pos + 1);
    }
}

auto host::sandbox::file_reference::basename() const -> std::string
{
    auto name = this->name();
    auto pos = name.find_last_of('.');
    if (pos == std::string::npos) {
        return name;
    }
    else {
        return name.substr(0, pos);
    }
}

// MARK: - File Operations

auto host::sandbox::file_reference::is_directory() const -> bool
{
    struct stat s { 0 };
    stat(m_path.c_str(), &s);
    return S_ISREG(s.st_mode) == 0;
}

auto host::sandbox::file_reference::exists() const -> bool
{
    struct stat s { 0 };
    return (stat(m_path.c_str(), &s) == 0);
}

auto host::sandbox::file_reference::create_parent_directory() -> void
{
    // Make sure the parent directory exists...
    auto parent_directory_path = m_path.substr(0, m_path.find_last_of('/'));
    auto parent = directory_reference::lua_reference(new directory_reference(parent_directory_path));
    parent->create_directory();
}

auto host::sandbox::file_reference::touch() -> void
{
    create_parent_directory();
}

auto host::sandbox::file_reference::open_resource_file() const -> resource_file_reference::lua_reference
{
    try {
        resource_file_reference::lua_reference ref(new resource_file_reference(m_path));
        return ref;
    }
    catch (std::exception& e) {
        return { nullptr };
    }
}