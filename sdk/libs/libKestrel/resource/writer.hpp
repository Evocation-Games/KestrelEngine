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

#pragma once

#include <string>
#include <memory>
#include <libData/writer.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/container.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/types/color.hpp>

namespace kestrel::resource
{
    struct lua_api(Resource.Writer, Available_0_8) writer
    {
    public:
        has_constructable_lua_api(writer);

        lua_constructor(Available_0_8) writer(const std::string& type, resource_core::identifier id, const std::string& name, const resource::container::lua_reference& container);

        [[nodiscard]] auto type_code() const -> std::string;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto id() const -> resource_core::identifier;
        [[nodiscard]] auto container() const -> resource::container::lua_reference;
        [[nodiscard]] auto data() const -> const data::block*;

        lua_function(writeSignedByte, Available_0_8) auto write_signed_byte(std::int8_t v) -> void;
        lua_function(writeSignedShort, Available_0_8) auto write_signed_short(std::int16_t v) -> void;
        lua_function(writeSignedLong, Available_0_8) auto write_signed_long(std::int32_t v) -> void;
        lua_function(writeSignedQuad, Available_0_8) auto write_signed_quad(std::int64_t v) -> void;
        lua_function(writeByte, Available_0_8) auto write_byte(std::uint8_t v) -> void;
        lua_function(writeShort, Available_0_8) auto write_short(std::uint16_t v) -> void;
        lua_function(writeLong, Available_0_8) auto write_long(std::uint32_t v) -> void;
        lua_function(writeQuad, Available_0_8) auto write_quad(std::uint64_t v) -> void;
        lua_function(writePStr, Available_0_8) auto write_pstr(const std::string& v) -> void;
        lua_function(writeCStr, Available_0_8) auto write_cstr(const std::string& v) -> void;
        lua_function(writeCStrOfLength, Available_0_8) auto write_cstr_width(std::uint32_t width, const std::string& v) -> void;

        lua_function(writePoint, Available_0_8) auto write_point(const math::point& v) -> void;
        lua_function(writeSize, Available_0_8) auto write_size(const math::size& v) -> void;
        lua_function(writeRect, Available_0_8) auto write_rect(const math::rect& v) -> void;
        lua_function(writeMacintoshRect, Available_0_8) auto write_macintosh_rect(const math::rect& v) -> void;

        lua_function(writeColor, Available_0_8) auto write_color(const graphics::color::lua_reference& v) -> void;

        lua_function(commit, Available_0_8) auto commit() -> void;

    private:
        resource_core::identifier m_id;
        std::string m_name;
        std::string m_type;
        resource::container::lua_reference m_container { nullptr };
        data::writer m_writer;

    };
}
