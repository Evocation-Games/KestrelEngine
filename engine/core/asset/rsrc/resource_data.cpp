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

#include <unordered_map>
#include "core/asset/rsrc/resource_data.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

auto asset::resource_data::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginNamespace("Resource")
                    .beginClass<asset::resource_data>("Reader")
                        .addConstructor<auto(*)(const resource_descriptor::lua_reference&)->void, resource_data::lua_reference>()
                        .addProperty("isValid", &resource_data::valid)
                        .addProperty("id", &resource_data::id)
                        .addProperty("name", &resource_data::name)
                        .addProperty("reference", &resource_data::reference)
                        .addProperty("bytesAvailable", &resource_data::bytes_available)
                        .addFunction("readByte", &resource_data::read_byte)
                        .addFunction("readShort", &resource_data::read_short)
                        .addFunction("readLong", &resource_data::read_long)
                        .addFunction("readQuad", &resource_data::read_quad)
                        .addFunction("readSignedByte", &resource_data::read_signed_byte)
                        .addFunction("readSignedShort", &resource_data::read_signed_short)
                        .addFunction("readSignedLong", &resource_data::read_signed_long)
                        .addFunction("readSignedQuad", &resource_data::read_signed_quad)
                        .addFunction("readPStr", &resource_data::read_pstr)
                        .addFunction("readCStr", &resource_data::read_cstr)
                        .addFunction("readCStrOfLength", &resource_data::read_cstr_width)
                        .addFunction("readPoint", &resource_data::read_point)
                        .addFunction("readSize", &resource_data::read_size)
                        .addFunction("readRect", &resource_data::read_rect)
                        .addFunction("readMacintoshRect", &resource_data::read_macintosh_rect)
                        .addFunction("readColor", &resource_data::read_color)
                        .addFunction("readResourceReference", &resource_data::read_resource_reference)
                        .addFunction("readTypedResourceReference", &resource_data::read_typed_resource_reference)
                        .addFunction("readResourceReferenceWideValue", &resource_data::read_resource_reference_wide_value)
                        .addFunction("switchOnResourceReference", &resource_data::switch_on_resource_reference)
                        .addFunction("skip", &resource_data::skip)
                    .endClass()
                .endNamespace()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::resource_data::resource_data(const std::string &type, int64_t id)
    : m_reader(nullptr)
{
    if (auto res = graphite::rsrc::manager::shared_manager().find(type, id)) {
        m_type = type;
        m_id = id;
        m_name = res->name();
        m_reader = graphite::data::reader(&res->data());

        if (const auto& type = res->type()) {
            const auto& attributes = type->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name("namespace"));
            if (it != attributes.end()) {
                m_namespace = it->second.string_value();
            }
        }
    }
}

asset::resource_data::resource_data(const asset::resource_descriptor::lua_reference& ref)
    : m_reader(nullptr)
{
    if (auto resource = ref->load()) {
        m_type = resource->type_code();
        m_id = resource->id();
        m_name = resource->name();
        m_reader = graphite::data::reader(&resource->data());

        if (const auto& type = resource->type()) {
            const auto& attributes = type->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name("namespace"));
            if (it != attributes.end()) {
                m_namespace = it->second.string_value();
            }
        }
    }
}

// MARK: - Accessors

auto asset::resource_data::valid() const -> bool
{
    return (m_reader.data() != nullptr);
}

auto asset::resource_data::id() const -> int64_t
{
    return m_id;
}

auto asset::resource_data::name() const -> std::string
{
    return m_name;
}

auto asset::resource_data::type() const -> std::string
{
    return m_type;
}

auto asset::resource_data::reference() const -> asset::resource_descriptor::lua_reference
{
    if (m_namespace.empty()) {
        return resource_descriptor::typed_identified(type(), id());
    }
    else {
        return resource_namespace(m_namespace).typed_identified_resource(type(), id());
    }
}

auto asset::resource_data::bytes_available() const -> bool
{
    return !m_reader.eof();
}

// MARK: - Data Reading

auto asset::resource_data::read_signed_byte() -> int8_t
{
    return m_reader.read_signed_byte();
}

auto asset::resource_data::read_signed_short() -> int16_t
{
    return m_reader.read_signed_short();
}

auto asset::resource_data::read_signed_long() -> int32_t
{
    return m_reader.read_signed_long();
}

auto asset::resource_data::read_signed_quad() -> int64_t
{
    return m_reader.read_signed_quad();
}

auto asset::resource_data::read_byte() -> uint8_t
{
    return m_reader.read_byte();
}

auto asset::resource_data::read_short() -> uint16_t
{
    return m_reader.read_short();
}

auto asset::resource_data::read_long() -> uint32_t
{
    return m_reader.read_long();
}

auto asset::resource_data::read_quad() -> uint64_t
{
    return m_reader.read_quad();
}

auto asset::resource_data::read_pstr() -> std::string
{
    return m_reader.read_pstr();
}

auto asset::resource_data::read_cstr() -> std::string
{
    return m_reader.read_cstr();
}

auto asset::resource_data::read_cstr_width(int width) -> std::string
{
    return m_reader.read_cstr(width);
}

auto asset::resource_data::read_point() -> math::point
{
    return { static_cast<double>(read_signed_short()), static_cast<double>(read_signed_short()) };
}

auto asset::resource_data::read_size() -> math::size
{
    return { read_signed_short(), read_signed_short() };
}

auto asset::resource_data::read_rect() -> math::rect
{
    return {
        static_cast<double>(read_signed_short()),
        static_cast<double>(read_signed_short()),
        static_cast<double>(read_signed_short()),
        static_cast<double>(read_signed_short())
    };
}

auto asset::resource_data::read_macintosh_rect() -> math::rect
{
    return math::rect::macintosh_rect(read_signed_short(), read_signed_short(), read_signed_short(), read_signed_short());
}

auto asset::resource_data::read_color() -> graphics::color::lua_reference
{
    return graphics::color::color_value_ref(read_long());
}

auto asset::resource_data::read_resource_reference() -> asset::resource_descriptor::lua_reference
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
            descriptor = resource_namespace(read_pstr()).identified_resource(0);
        }
        if (flags & has_type) {
            descriptor = descriptor->with_type(read_cstr_width(4));
        }
        if (flags & same_file) {
            // TODO: Constrain to the same file as this data originates from.
        }

        descriptor = descriptor->with_id(read_signed_quad());
        return descriptor;
    }
    else {
        return resource_namespace::global()->identified_resource(read_signed_short());
    }
}

auto asset::resource_data::read_typed_resource_reference(const std::string &type) -> asset::resource_descriptor::lua_reference
{
    return read_resource_reference()->with_type(type);
}

auto asset::resource_data::read_resource_reference_wide_value() -> int64_t
{
    return static_cast<int64_t>(read_signed_short());
}

auto asset::resource_data::switch_on_resource_reference(const luabridge::LuaRef &body) -> void
{
    // TODO: Handle extended format.
    auto value = m_reader.read_signed_short(0, graphite::data::reader::mode::peek);

    // TODO: Logic to determine what the namespace is

    body(value, resource_namespace::universal());
}

auto asset::resource_data::skip(int delta) -> void
{
    m_reader.move(delta);
}
