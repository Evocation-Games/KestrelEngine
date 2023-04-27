// Copyright (c) 2020 Tom Hancocks
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

#include <limits>
#include <unordered_map>
#include <libKestrel/resource/reader.hpp>
#include <libKestrel/resource/container.hpp>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Construction

kestrel::resource::reader::reader(const graphite::data::block *data)
    : m_reader(data)
{}

kestrel::resource::reader::reader(const std::string &type, graphite::rsrc::resource::identifier id)
    : m_reader(nullptr)
{
    if (auto res = graphite::rsrc::manager::shared_manager().find(type, id)) {
        m_type = type;
        m_id = id;
        m_name = res->name();
        m_reader = graphite::data::reader(&res->data());

        if (const auto& type = res->type()) {
            const auto& attributes = type->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                m_container = it->second.string_value();
            }
        }
    }
}

kestrel::resource::reader::reader(const descriptor::lua_reference& ref)
    : m_reader(nullptr)
{
    if (auto resource = ref->load()) {
        m_type = resource->type_code();
        m_id = resource->id();
        m_name = resource->name();
        m_reader = graphite::data::reader(&resource->data());

        if (const auto& type = resource->type()) {
            const auto& attributes = type->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                m_container = it->second.string_value();
            }
        }
    }
}

// MARK: - Accessors

auto kestrel::resource::reader::valid() const -> bool
{
    return (m_reader.data() != nullptr);
}

auto kestrel::resource::reader::id() const -> graphite::rsrc::resource::identifier
{
    return m_id;
}

auto kestrel::resource::reader::name() const -> std::string
{
    return m_name;
}

auto kestrel::resource::reader::type() const -> std::string
{
    return m_type;
}

auto kestrel::resource::reader::reference() const -> descriptor::lua_reference
{
    if (m_container.empty()) {
        return descriptor::typed_identified(type(), id());
    }
    else {
        return resource::container(m_container).typed_identified_resource(type(), id());
    }
}

auto kestrel::resource::reader::bytes_available() const -> bool
{
    return !m_reader.eof();
}

// MARK: - Data Reading

auto kestrel::resource::reader::read_signed_byte() -> std::int8_t
{
    return m_reader.read_signed_byte();
}

auto kestrel::resource::reader::read_signed_short() -> std::int16_t
{
    return m_reader.read_signed_short();
}

auto kestrel::resource::reader::read_signed_long() -> std::int32_t
{
    return m_reader.read_signed_long();
}

auto kestrel::resource::reader::read_signed_quad() -> std::int64_t
{
    return m_reader.read_signed_quad();
}

auto kestrel::resource::reader::read_byte() -> std::uint8_t
{
    return m_reader.read_byte();
}

auto kestrel::resource::reader::read_short() -> std::uint16_t
{
    return m_reader.read_short();
}

auto kestrel::resource::reader::read_long() -> std::uint32_t
{
    return m_reader.read_long();
}

auto kestrel::resource::reader::read_quad() -> std::uint64_t
{
    return m_reader.read_quad();
}

auto kestrel::resource::reader::read_pstr() -> std::string
{
    return m_reader.read_pstr();
}

auto kestrel::resource::reader::read_cstr() -> std::string
{
    return m_reader.read_cstr();
}

auto kestrel::resource::reader::read_cstr_width(int width) -> std::string
{
    return m_reader.read_cstr(width);
}

auto kestrel::resource::reader::read_point() -> math::point
{
    return { static_cast<float>(read_signed_short()), static_cast<float>(read_signed_short()) };
}

auto kestrel::resource::reader::read_size() -> math::size
{
    return { static_cast<float>(read_signed_short()), static_cast<float>(read_signed_short()) };
}

auto kestrel::resource::reader::read_rect() -> math::rect
{
    return {
        static_cast<float>(read_signed_short()),
        static_cast<float>(read_signed_short()),
        static_cast<float>(read_signed_short()),
        static_cast<float>(read_signed_short())
    };
}

auto kestrel::resource::reader::read_macintosh_rect() -> math::rect
{
    return math::rect::macintosh_rect(read_signed_short(), read_signed_short(), read_signed_short(), read_signed_short());
}

auto kestrel::resource::reader::read_color() -> graphics::color::lua_reference
{
    return graphics::color::color_value_ref(read_long());
}

auto kestrel::resource::reader::read_resource_reference() -> descriptor::lua_reference
{
    enum resource_reference_flags : std::uint8_t {
        has_namespace = 0x01,
        has_type = 0x02,
        same_file = 0x04,
    };

    // TODO: Handle the namespacing format
    if (m_reader.data()->is_extended_format()) {
        auto flags = read_byte();

        auto descriptor = reference()->ignoring_id()->ignoring_name()->ignoring_type();

        if (flags & has_namespace) {
            descriptor = resource::container(read_pstr()).identified_resource(0);
        }
        if (flags & has_type) {
            descriptor = descriptor->with_type(read_cstr_width(4));
        }
        if (flags & same_file) {
            // TODO: Constrain to the same file as this data originates from.
        }

        auto id = read_signed_quad();
        if (id == std::numeric_limits<std::int64_t>::min() + 1) {
            return { nullptr };
        }

        descriptor = descriptor->with_id(id);
        return descriptor;
    }
    else {
        return resource::container::global()->identified_resource(read_signed_short());
    }
}

auto kestrel::resource::reader::read_typed_resource_reference(const std::string &type) -> descriptor::lua_reference
{
    return read_resource_reference()->with_type(type);
}

auto kestrel::resource::reader::read_resource_reference_wide_value() -> graphite::rsrc::resource::identifier
{
    return static_cast<graphite::rsrc::resource::identifier>(read_signed_short());
}

auto kestrel::resource::reader::switch_on_resource_reference(const luabridge::LuaRef &body) -> void
{
    // TODO: Handle extended format.
    auto value = m_reader.read_signed_short(0, graphite::data::reader::mode::peek);

    // TODO: Logic to determine what the namespace is

    body(value, resource::container::universal());
}

auto kestrel::resource::reader::skip(int delta) -> void
{
    m_reader.move(delta);
}

auto kestrel::resource::reader::read_remaining_data() -> graphite::data::block
{
    return m_reader.read_data(m_reader.size() - m_reader.position());
}