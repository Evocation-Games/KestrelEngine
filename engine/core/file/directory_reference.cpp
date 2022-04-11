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
#include <sys/stat.h>
#include "core/file/directory_reference.hpp"

// MARK: - Lua

auto host::sandbox::directory_reference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Kestrel")
            .beginNamespace("Sandbox")
                .beginClass<host::sandbox::directory_reference>("DirectoryReference")
                    .addProperty("name", &host::sandbox::directory_reference::name)
                    .addProperty("path", &host::sandbox::directory_reference::path)
                    .addFunction("exists", &host::sandbox::directory_reference::exists)
                    .addFunction("contents", &host::sandbox::directory_reference::contents)
                    .addFunction("file", &host::sandbox::directory_reference::file)
                    .addFunction("directory", &host::sandbox::directory_reference::directory)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

host::sandbox::directory_reference::directory_reference(const std::string &path)
{
    if (path[path.size() - 1] == '/') {
        m_path = path.substr(0, path.size() - 1);
    }
    else {
        m_path = path;
    }
}

auto host::sandbox::directory_reference::get(const std::string &path) -> host::sandbox::directory_reference::lua_reference
{
    return check_path_exists(path) ? new directory_reference(path) : nullptr;
}

// MARK: - Accessors

auto host::sandbox::directory_reference::parent() const -> directory_reference::lua_reference
{
    auto path = m_path.substr(0, m_path.find_last_of('/') - 1);
    return { new directory_reference(path) };
}

auto host::sandbox::directory_reference::path() const -> std::string
{
    return m_path;
}

// MARK: - Directory Operations

auto host::sandbox::directory_reference::check_path_exists(const std::string& path) -> bool
{
    DIR *dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return true;
    }
    else {
        return false;
    }
}

auto host::sandbox::directory_reference::exists() const -> bool
{
    return check_path_exists(m_path);
}

auto host::sandbox::directory_reference::name() const -> std::string
{
    auto pos = m_path.find_last_of('/');
    return m_path.substr(pos + 1);
}

auto host::sandbox::directory_reference::contents(bool include_dot_files) const -> util::lua_vector<host::sandbox::file_reference::lua_reference>
{
    if (!exists()) {
        return {};
    }

    util::lua_vector<host::sandbox::file_reference::lua_reference> contents;

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
            contents.emplace_back( new host::sandbox::file_reference(file_path) );
        }
        closedir(dir);
    }

    return contents;
}

auto host::sandbox::directory_reference::file(const std::string& file) const -> file_reference::lua_reference
{
    return { new file_reference(m_path + "/" + file) };
}

auto host::sandbox::directory_reference::directory(const std::string &dir) const -> directory_reference::lua_reference
{
    return { new directory_reference(m_path + "/" + dir) };
}

// MARK: - Directory Management

auto host::sandbox::directory_reference::create_directory(bool intermediates) -> void
{
    if (exists()) {
        return;
    }

    auto parent_directory_path = m_path.substr(0, m_path.find_last_of('/'));
    auto parent = directory_reference::lua_reference(new directory_reference(parent_directory_path));
    if (!parent->exists()) {
        if (intermediates) {
            parent->create_directory(intermediates);
        }
        else {
            throw std::runtime_error("Could not create directory: " + m_path);
        }
    }

#if TARGET_WINDOWS
    if (mkdir(m_path.c_str())) {
        throw std::runtime_error("Failed to create directory: " + m_path);
    }
#else
    if (mkdir(m_path.c_str(), 0755)) {
        throw std::runtime_error("Failed to create directory: " + m_path);
    }
#endif

}