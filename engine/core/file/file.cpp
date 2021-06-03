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

#include "core/file/file.hpp"

// MARK: - Lua

auto host::file::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Host")
            .beginClass<host::file>("File")
                .addStaticFunction("exists", &host::file::exists)
                .addProperty("name", &host::file::name)
                .addProperty("path", &host::file::path)
                .addProperty("basename", &host::file::basename)
                .addProperty("extension", &host::file::extension)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

host::file::file(const std::string &path)
    : m_path(path)
{
    assert(path[path.size() - 1] != '/');
}

// MARK: - Accessors

auto host::file::name() const -> std::string
{
    auto pos = m_path.find_last_of('/');
    return m_path.substr(pos + 1);
}

auto host::file::path() const -> std::string
{
    return m_path;
}

auto host::file::extension() const -> std::string
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

auto host::file::basename() const -> std::string
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

auto host::file::exists(const std::string &path) -> bool
{
    return false;
}
