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
#include <libKestrel/sandbox/file/resource_file_reference.hpp>
#include <libKestrel/resource/namespace.hpp>

// MARK: - Construction

kestrel::sandbox::resource_file_reference::resource_file_reference(const std::string &path)
    : m_path(path)
{
    m_file = graphite::rsrc::file(path);
}

// MARK: - File Operations

auto kestrel::sandbox::resource_file_reference::type() const -> enum resource_file_type
{
    switch (m_file.format()) {
        case graphite::rsrc::file::format::classic:     return resource_file_type::classic;
        case graphite::rsrc::file::format::extended:    return resource_file_type::extended;
        case graphite::rsrc::file::format::rez:         return resource_file_type::rez;
        default:                                       return resource_file_type::none;
    }
};

auto kestrel::sandbox::resource_file_reference::name() const -> std::string
{
    auto pos = m_path.find_last_of('/');
    return m_path.substr(pos + 1);
}

auto kestrel::sandbox::resource_file_reference::path() const -> std::string
{
    return m_path;
}

auto kestrel::sandbox::resource_file_reference::extension() const -> std::string
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

auto kestrel::sandbox::resource_file_reference::basename() const -> std::string
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

auto kestrel::sandbox::resource_file_reference::exists() const -> bool
{
    struct stat s { 0 };
    return (stat(m_path.c_str(), &s) == 0);
}

// MARK: - Resource Management

auto kestrel::sandbox::resource_file_reference::all_types() const -> lua::vector<resource::descriptor::lua_reference>
{
    lua::vector<resource::descriptor::lua_reference> descriptors;

    for (const auto& type_hash : m_file.types()) {
        const auto& type = m_file.type(type_hash);

        auto ns = resource::resource_namespace::global();
        for (const auto& attr : type->attributes()) {
            if (attr.second.name() == resource::resource_namespace::attribute_name) {
                ns = resource::resource_namespace::lua_reference(new resource::resource_namespace(attr.second.string_value()));
                break;
            }
        }

        auto descriptor = ns->file_constraint(&m_file)->with_type(type->code());
        descriptors.emplace_back(descriptor);
    }
    return descriptors;
}

auto kestrel::sandbox::resource_file_reference::add_resource(const resource::writer::lua_reference &writer) -> void
{
    auto type_code = writer->type_code();
    auto id = writer->id();
    auto name = writer->name();
    auto ns = writer->ns();

    std::vector<graphite::rsrc::attribute> type_attributes;
    if (ns.get()) {
        type_attributes.emplace_back(graphite::rsrc::attribute(resource::resource_namespace::attribute_name, ns->primary_name()));
    }

    // Do we have an existing resource already?
    if (auto resource = m_file.find(type_code, id, type_attributes)) {
        const_cast<graphite::rsrc::resource *>(resource)->set_data(*const_cast<graphite::data::block *>(writer->data()));
        return;
    }

    auto type = const_cast<graphite::rsrc::type *>(m_file.type(type_code, type_attributes));
    if (type) {
        auto resource = new graphite::rsrc::resource(type, id, name, *writer->data());
        type->add_resource(resource);
    }
}

// MARK: - File Management

auto kestrel::sandbox::resource_file_reference::save() -> void
{
    m_file.write(m_path);
}