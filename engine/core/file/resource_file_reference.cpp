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

#include <sys/stat.h>
#include "core/file/resource_file_reference.hpp"
#include "core/asset/rsrc/namespace.hpp"

// MARK: - Constants

static auto s_resource_file_type_classic = static_cast<std::uint8_t>(host::sandbox::resource_file_reference::type::classic);
static auto s_resource_file_type_extended = static_cast<std::uint8_t>(host::sandbox::resource_file_reference::type::extended);
static auto s_resource_file_type_rez = static_cast<std::uint8_t>(host::sandbox::resource_file_reference::type::rez);

// MARK: - Lua

auto host::sandbox::resource_file_reference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Kestrel")
            .beginNamespace("ResourceFileFormat")
                .addProperty("Classic", &s_resource_file_type_classic, false)
                .addProperty("Extended", &s_resource_file_type_extended, false)
                .addProperty("Rez", &s_resource_file_type_rez, false)
            .endNamespace()
            .beginNamespace("Sandbox")
                .beginClass<resource_file_reference>("ResourceFileReference")
                    .addProperty("exists", &resource_file_reference::exists)
                    .addProperty("name", &resource_file_reference::name)
                    .addProperty("path", &resource_file_reference::path)
                    .addProperty("basename", &resource_file_reference::basename)
                    .addProperty("extension", &resource_file_reference::extension)
                    .addProperty("format", &resource_file_reference::type)
                    .addProperty("resourceTypes", &resource_file_reference::all_types)
                    .addFunction("save", &resource_file_reference::save)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

host::sandbox::resource_file_reference::resource_file_reference(const std::string &path)
    : m_path(path)
{
    m_file = graphite::rsrc::file(path);
}

// MARK: - File Operations

auto host::sandbox::resource_file_reference::type() const -> enum type
{
    switch (m_file.format()) {
        case graphite::rsrc::file::format::classic:     return type::classic;
        case graphite::rsrc::file::format::extended:    return type::extended;
        case graphite::rsrc::file::format::rez:         return type::rez;
        default:                                        return type::none;
    }
};

auto host::sandbox::resource_file_reference::name() const -> std::string
{
    auto pos = m_path.find_last_of('/');
    return m_path.substr(pos + 1);
}

auto host::sandbox::resource_file_reference::path() const -> std::string
{
    return m_path;
}

auto host::sandbox::resource_file_reference::extension() const -> std::string
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

auto host::sandbox::resource_file_reference::basename() const -> std::string
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

auto host::sandbox::resource_file_reference::exists() const -> bool
{
    struct stat s { 0 };
    return (stat(m_path.c_str(), &s) == 0);
}

// MARK: - Resource Management

auto host::sandbox::resource_file_reference::all_types() const -> util::lua_vector<asset::resource_descriptor::lua_reference>
{
    util::lua_vector<asset::resource_descriptor::lua_reference> descriptors;

    for (const auto& type_hash : m_file.types()) {
        const auto& type = m_file.type(type_hash);

        auto ns = asset::resource_namespace::global();
        for (const auto& attr : type->attributes()) {
            if (attr.second.name() == "namespace") {
                ns = asset::resource_namespace::lua_reference(new asset::resource_namespace(attr.second.string_value()));
                break;
            }
        }

        auto descriptor = ns->file_constraint(&m_file)->with_type(type->code());
        descriptors.emplace_back(descriptor);
    }
    return descriptors;
}

auto host::sandbox::resource_file_reference::add_resource(const asset::resource_writer::lua_reference &writer) -> void
{

}

// MARK: - File Management

auto host::sandbox::resource_file_reference::save() -> void
{
    m_file.write(m_path);
}