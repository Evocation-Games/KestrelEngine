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
#include <libGraphite/data/writer.hpp>
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"

namespace asset
{
    struct resource_writer: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<asset::resource_writer> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        int64_t m_id;
        std::string m_name;
        std::string m_type;
        resource_namespace::lua_reference m_namespace { nullptr };
        std::shared_ptr<graphite::data::writer> m_writer;

    public:
        resource_writer(const std::string& type, int64_t id, const std::string& name, const resource_namespace::lua_reference& ns);

        lua_api auto write_signed_byte(int8_t v) -> void;
        lua_api auto write_signed_short(int16_t v) -> void;
        lua_api auto write_signed_long(int32_t v) -> void;
        lua_api auto write_signed_quad(int64_t v) -> void;
        lua_api auto write_byte(uint8_t v) -> void;
        lua_api auto write_short(uint16_t v) -> void;
        lua_api auto write_long(uint32_t v) -> void;
        lua_api auto write_quad(uint64_t v) -> void;
        lua_api auto write_pstr(const std::string& v) -> void;
        lua_api auto write_cstr(const std::string& v) -> void;
        lua_api auto write_cstr_width(const int& width, const std::string& v) -> void;

        lua_api auto write_point(const math::point& v) -> void;
        lua_api auto write_size(const math::size& v) -> void;
        lua_api auto write_rect(const math::rect& v) -> void;
        lua_api auto write_macintosh_rect(const math::rect& v) -> void;

        lua_api auto write_color(const graphics::color::lua_reference& v) -> void;

        lua_api auto commit() -> void;
    };
}
