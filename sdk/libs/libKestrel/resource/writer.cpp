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

#include <libKestrel/resource/writer.hpp>
#include <libKestrel/sandbox/file/files.hpp>
#include <libResourceCore/manager.hpp>

// MARK: - Construction

kestrel::resource::writer::writer(const std::string& type, resource_core::identifier id, const std::string& name, const resource::container::lua_reference& container)
    : m_type(type), m_id(id), m_name(name), m_container(container)
{}

// MARK: - Writing

auto kestrel::resource::writer::write_signed_byte(std::int8_t v) -> void
{
    m_writer.write_signed_byte(v);
}

auto kestrel::resource::writer::write_signed_short(std::int16_t v) -> void
{
    m_writer.write_signed_short(v);
}

auto kestrel::resource::writer::write_signed_long(std::int32_t v) -> void
{
    m_writer.write_signed_long(v);
}

auto kestrel::resource::writer::write_signed_quad(std::int64_t v) -> void
{
    m_writer.write_signed_quad(v);
}

auto kestrel::resource::writer::write_byte(std::uint8_t v) -> void
{
    m_writer.write_byte(v);
}

auto kestrel::resource::writer::write_short(std::uint16_t v) -> void
{
    m_writer.write_short(v);
}

auto kestrel::resource::writer::write_long(std::uint32_t v) -> void
{
    m_writer.write_long(v);
}

auto kestrel::resource::writer::write_quad(std::uint64_t v) -> void
{
    m_writer.write_quad(v);
}

auto kestrel::resource::writer::write_pstr(const std::string& v) -> void
{
    m_writer.write_pstr(v);
}

auto kestrel::resource::writer::write_cstr(const std::string& v) -> void
{
    m_writer.write_cstr(v);
}

auto kestrel::resource::writer::write_cstr_width(std::uint32_t width, const std::string& v) -> void
{
    m_writer.write_cstr(v, width);
}


auto kestrel::resource::writer::write_point(const math::point& v) -> void
{
    write_signed_short(static_cast<std::int16_t>(v.x()));
    write_signed_short(static_cast<std::int16_t>(v.y()));
}

auto kestrel::resource::writer::write_size(const math::size& v) -> void
{
    write_signed_short(static_cast<std::int16_t>(v.width()));
    write_signed_short(static_cast<std::int16_t>(v.height()));
}

auto kestrel::resource::writer::write_rect(const math::rect& v) -> void
{
    write_point(v.origin());
    write_size(v.size());
}

auto kestrel::resource::writer::write_macintosh_rect(const math::rect& v) -> void
{
    write_signed_short(static_cast<std::int16_t>(v.origin().y()));
    write_signed_short(static_cast<std::int16_t>(v.origin().x()));
    write_signed_short(static_cast<std::int16_t>(v.size().height() + v.origin().y()));
    write_signed_short(static_cast<std::int16_t>(v.size().width() + v.origin().x()));
}
auto kestrel::resource::writer::write_color(const graphics::color::lua_reference& v) -> void
{
    write_long(v->color_value());
}

// MARK: - Resource Saving

auto kestrel::resource::writer::commit() -> void
{
    // Construct a list of attributes for the resource.
    std::unordered_map<std::string, std::string> attributes;
    if (!m_container->is_global() && !m_container->is_universal()) {
        attributes[resource::container::attribute_name] = m_container->primary_name();
    }

    // Find the appropriate file in the resource manager, and add the resource.
    const auto& save_file = sandbox::files::shared_files().get_current_save_file();
    if (!save_file.get()) {
        return;
    }
    save_file->create_parent_directory();

    for (auto& file : resource_core::manager::shared_manager().file_references()) {
        if (file->path() == save_file->path()) {
            // We've found the correct file in the resource manager, now write the resource to it.
            file->add_resource(m_type, m_id, m_name, *m_writer.data(), attributes);
            return;
        }
    }

    // No file was created, so we need to add one to the resource manager.
    auto new_file = new resource_core::file();
    new_file->add_resource(m_type, m_id, m_name, *m_writer.data(), attributes);
    new_file->write(save_file->path(), resource_core::file::format::extended);

    resource_core::manager::shared_manager().import_file(new_file);
}

// MARK: - Accessors

auto kestrel::resource::writer::type_code() const -> std::string
{
    return m_type;
}

auto kestrel::resource::writer::name() const -> std::string
{
    return m_name;
}

auto kestrel::resource::writer::id() const -> resource_core::identifier
{
    return m_id;
}

auto kestrel::resource::writer::container() const -> resource::container::lua_reference
{
    return m_container;
}

auto kestrel::resource::writer::data() const -> const data::block *
{
    return m_writer.data();
}
