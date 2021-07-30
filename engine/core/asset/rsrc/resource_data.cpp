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

#include "core/asset/rsrc/resource_data.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

auto asset::resource_data::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<asset::resource_data>("Data")
                   .addConstructor<auto(*)(const resource::lua_reference&)->void, resource_data::lua_reference>()
                   .addProperty("isValid", &resource_data::valid)
                   .addProperty("id", &resource_data::id)
                   .addProperty("name", &resource_data::name)
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
                   .addFunction("skip", &resource_data::skip)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::resource_data::resource_data(const std::string &type, const int64_t &id)
    : m_reader(nullptr)
{
    if (auto res = graphite::rsrc::manager::shared_manager().find(type, id).lock()) {
        m_type = type;
        m_id = id;
        m_name = res->name();
        m_reader = std::make_shared<graphite::data::reader>(res->data());
    }
}

asset::resource_data::resource_data(const asset::resource::lua_reference& ref)
    : m_reader(nullptr)
{
    if (ref->id().has_value() && ref->type().has_value()) {

        // Build a list of attributes to constrain the resource by.
        std::map<std::string, std::string> attributes {};
        if (ref->ns().has_value() && !ref->ns()->name().empty()) {
            attributes["namespace"] = ref->ns()->name();
        }

        if (auto res = graphite::rsrc::manager::shared_manager().find(ref->type().value(), ref->id().value(), attributes).lock()) {
            m_type = res->type_code();
            m_id = res->id();
            m_name = res->name();
            m_reader = std::make_shared<graphite::data::reader>(res->data());
        }
    }
}

// MARK: - Accessors

auto asset::resource_data::valid() const -> bool
{
    return (m_reader != nullptr);
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

// MARK: - Data Reading

auto asset::resource_data::read_signed_byte() -> int8_t
{
    return m_reader->read_signed_byte();
}

auto asset::resource_data::read_signed_short() -> int16_t
{
    return m_reader->read_signed_short();
}

auto asset::resource_data::read_signed_long() -> int32_t
{
    return m_reader->read_signed_long();
}

auto asset::resource_data::read_signed_quad() -> int64_t
{
    return m_reader->read_signed_quad();
}

auto asset::resource_data::read_byte() -> uint8_t
{
    return m_reader->read_byte();
}

auto asset::resource_data::read_short() -> uint16_t
{
    return m_reader->read_short();
}

auto asset::resource_data::read_long() -> uint32_t
{
    return m_reader->read_long();
}

auto asset::resource_data::read_quad() -> uint64_t
{
    return m_reader->read_quad();
}

auto asset::resource_data::read_pstr() -> std::string
{
    return m_reader->read_pstr();
}

auto asset::resource_data::read_cstr() -> std::string
{
    return m_reader->read_cstr();
}

auto asset::resource_data::read_cstr_width(const int& width) -> std::string
{
    return m_reader->read_cstr(width);
}

auto asset::resource_data::skip(const int &delta) -> void
{
    m_reader->move(delta);
}
