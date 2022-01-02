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

#include "core/asset/rsrc/resource_writer.hpp"
#include "core/file/files.hpp"

// MARK: - Lua

auto asset::resource_writer::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginNamespace("Resource")
                    .beginClass<asset::resource_writer>("Writer")
                        .addConstructor<auto(*)(const std::string&, int64_t, const std::string&, const resource_namespace::lua_reference&)->void, resource_writer::lua_reference>()
                        .addFunction("writeByte", &resource_writer::write_byte)
                        .addFunction("writeShort", &resource_writer::write_short)
                        .addFunction("writeLong", &resource_writer::write_long)
                        .addFunction("writeQuad", &resource_writer::write_quad)
                        .addFunction("writeSignedByte", &resource_writer::write_signed_byte)
                        .addFunction("writeSignedShort", &resource_writer::write_signed_short)
                        .addFunction("writeSignedLong", &resource_writer::write_signed_long)
                        .addFunction("writeSignedQuad", &resource_writer::write_signed_quad)
                        .addFunction("writePStr", &resource_writer::write_pstr)
                        .addFunction("writeCStr", &resource_writer::write_cstr)
                        .addFunction("writeCStrOfLength", &resource_writer::write_cstr_width)
                        .addFunction("writePoint", &resource_writer::write_point)
                        .addFunction("writeSize", &resource_writer::write_size)
                        .addFunction("writeRect", &resource_writer::write_rect)
                        .addFunction("writeMacintoshRect", &resource_writer::write_macintosh_rect)
                        .addFunction("writeVector", &resource_writer::write_vector)
                        .addFunction("writeColor", &resource_writer::write_color)
                        .addFunction("commit", &resource_writer::commit)
                    .endClass()
                .endNamespace()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::resource_writer::resource_writer(const std::string& type, int64_t id, const std::string& name, const resource_namespace::lua_reference& ns)
    : m_type(type), m_id(id), m_name(name), m_namespace(ns)
{
    m_writer = std::make_shared<graphite::data::writer>();
}

// MARK: - Writing

auto asset::resource_writer::write_signed_byte(int8_t v) -> void
{
    m_writer->write_signed_byte(v);
}

auto asset::resource_writer::write_signed_short(int16_t v) -> void
{
    m_writer->write_signed_short(v);
}

auto asset::resource_writer::write_signed_long(int32_t v) -> void
{
    m_writer->write_signed_long(v);
}

auto asset::resource_writer::write_signed_quad(int64_t v) -> void
{
    m_writer->write_signed_quad(v);
}

auto asset::resource_writer::write_byte(uint8_t v) -> void
{
    m_writer->write_byte(v);
}

auto asset::resource_writer::write_short(uint16_t v) -> void
{
    m_writer->write_short(v);
}

auto asset::resource_writer::write_long(uint32_t v) -> void
{
    m_writer->write_long(v);
}

auto asset::resource_writer::write_quad(uint64_t v) -> void
{
    m_writer->write_quad(v);
}

auto asset::resource_writer::write_pstr(const std::string& v) -> void
{
    m_writer->write_pstr(v);
}

auto asset::resource_writer::write_cstr(const std::string& v) -> void
{
    m_writer->write_cstr(v);
}

auto asset::resource_writer::write_cstr_width(const int& width, const std::string& v) -> void
{
    m_writer->write_cstr(v, width);
}


auto asset::resource_writer::write_point(const math::point& v) -> void
{
    write_signed_short(static_cast<int16_t>(v.x));
    write_signed_short(static_cast<int16_t>(v.y));
}

auto asset::resource_writer::write_size(const math::size& v) -> void
{
    write_signed_short(static_cast<int16_t>(v.width));
    write_signed_short(static_cast<int16_t>(v.height));
}

auto asset::resource_writer::write_rect(const math::rect& v) -> void
{
    write_point(v.origin);
    write_size(v.size);
}

auto asset::resource_writer::write_macintosh_rect(const math::rect& v) -> void
{
    write_signed_short(static_cast<int16_t>(v.origin.y));
    write_signed_short(static_cast<int16_t>(v.origin.x));
    write_signed_short(static_cast<int16_t>(v.size.height + v.origin.y));
    write_signed_short(static_cast<int16_t>(v.size.width + v.origin.x));
}

auto asset::resource_writer::write_vector(const math::vector& v) -> void
{
    write_signed_short(static_cast<int16_t>(v.x));
    write_signed_short(static_cast<int16_t>(v.y));
}

auto asset::resource_writer::write_color(const graphics::color::lua_reference& v) -> void
{
    write_long(v->value());
}

// MARK: - Resource Saving

auto asset::resource_writer::commit() -> void
{
    // Construct a list of attributes for the resource.
    std::map<std::string, std::string> attributes;
    if (!m_namespace->is_global() && !m_namespace->is_universal()) {
        attributes["namespace"] = m_namespace->primary_name();
    }

    // Find the appropriate file in the resource manager, and add the resource.
    const auto& save_file = host::sandbox::files::shared_files().get_current_save_file();
    if (!save_file.get()) {
        return;
    }
    save_file->create_parent_directory();

    for (auto& file : graphite::rsrc::manager::shared_manager().files()) {
        if (file->path() == save_file->path()) {
            // We've found the correct file in the resource manager, now write the resource to it.
            file->add_resource(m_type, m_id, m_name, m_writer->data(), attributes);
            return;
        }
    }

    // No file was created, so we need to add one to the resource manager.
    auto new_file = std::make_shared<graphite::rsrc::file>();
    new_file->add_resource(m_type, m_id, m_name, m_writer->data(), attributes);
    new_file->write(save_file->path(), graphite::rsrc::file::extended);

    graphite::rsrc::manager::shared_manager().import_file(new_file);
}