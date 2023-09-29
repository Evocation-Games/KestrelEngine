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

#pragma once

#include <string>
#include <optional>
#include <libData/reader.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/container.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/types/color.hpp>

namespace kestrel::resource
{
    struct lua_api(Resource.Reader, Available_0_8) reader
    {
    public:
        has_constructable_lua_api(reader);

        reader(const std::string& type, resource_core::identifier id);
        explicit reader(const data::block *data);
        lua_constructor(Available_0_8) explicit reader(const descriptor::lua_reference& ref);

        lua_getter(isValid, Available_0_8) [[nodiscard]] auto valid() const -> bool;

        lua_getter(id, Available_0_8) [[nodiscard]] auto id() const -> resource_core::identifier;
        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_getter(type, Available_0_8) [[nodiscard]] auto type() const -> std::string;
        lua_getter(reference, Available_0_8) [[nodiscard]] auto reference() const -> descriptor::lua_reference;

        lua_getter(bytesAvailable, Available_0_8) [[nodiscard]] auto bytes_available() const -> bool;

        lua_function(readSignedByte, Available_0_8) auto read_signed_byte() -> std::int8_t;
        lua_function(readSignedShort, Available_0_8) auto read_signed_short() -> std::int16_t;
        lua_function(readSignedLong, Available_0_8) auto read_signed_long() -> std::int32_t;
        lua_function(readSignedQuad, Available_0_8) auto read_signed_quad() -> std::int64_t;
        lua_function(readByte, Available_0_8) auto read_byte() -> std::uint8_t;
        lua_function(readShort, Available_0_8) auto read_short() -> std::uint16_t;
        lua_function(readLong, Available_0_8) auto read_long() -> std::uint32_t;
        lua_function(readQuad, Available_0_8) auto read_quad() -> std::uint64_t;
        lua_function(readPStr, Available_0_8) auto read_pstr() -> std::string;
        lua_function(readCStr, Available_0_8) auto read_cstr() -> std::string;
        lua_function(readCStrOfLength, Available_0_8) auto read_cstr_width(int width) -> std::string;

        lua_function(readPoint, Available_0_8) auto read_point() -> math::point;
        lua_function(readSize, Available_0_8) auto read_size() -> math::size;
        lua_function(readRect, Available_0_8) auto read_rect() -> math::rect;
        lua_function(readMacintoshRect, Available_0_8) auto read_macintosh_rect() -> math::rect;

        lua_function(readColor, Available_0_8) auto read_color() -> graphics::color::lua_reference;

        lua_function(readResourceReferenceWideValue, Available_0_8) auto read_resource_reference_wide_value() -> resource_core::identifier;
        lua_function(readResourceReference, Available_0_8) auto read_resource_reference() -> descriptor::lua_reference;
        lua_function(readTypedResourceReference, Available_0_8) auto read_typed_resource_reference(const std::string& type) -> descriptor::lua_reference;
        lua_function(switchOnResourceReference, Available_0_8) auto switch_on_resource_reference(const luabridge::LuaRef& body) -> void;

        lua_function(skip, Available_0_8) auto skip(int delta) -> void;

        auto read_remaining_data() -> data::block;

    private:
        resource_core::identifier m_id;
        std::string m_name;
        std::string m_type;
        data::reader m_reader;
        std::string m_container;

    };

}
