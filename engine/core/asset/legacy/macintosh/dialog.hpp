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

#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/asset/legacy/macintosh/item_list.hpp"
#include "scripting/state.hpp"
#include "math/rect.hpp"
#include "util/hint.hpp"

namespace asset::legacy::macintosh::toolbox
{
    struct dialog : public scripting::lua::object
    {
    public:
        constexpr static const char *type { "DLOG" };
        typedef luabridge::RefCountedPtr<dialog> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        lua_api explicit dialog(const asset::resource_descriptor::lua_reference& ref);
        lua_api static auto load(const asset::resource_descriptor::lua_reference& ref) -> lua_reference;

        [[nodiscard]] lua_api auto bounds() const -> math::rect { return m_bounds; }
        lua_api auto set_bounds(const math::rect& bounds) -> void { m_bounds = bounds; }

        [[nodiscard]] lua_api auto proc_id() const -> int16_t { return m_proc_id; }
        lua_api auto set_proc_id(int16_t id) -> void { m_proc_id = id; }

        [[nodiscard]] lua_api auto visible() const -> bool { return m_visible; }
        lua_api auto set_visible(bool f) -> void { m_visible = f; }

        [[nodiscard]] lua_api auto go_away() const -> bool { return m_go_away; }
        lua_api auto set_go_away(bool f) -> void { m_go_away = f; }

        [[nodiscard]] lua_api auto ref_con() const -> int32_t { return m_ref_con; }
        lua_api auto set_ref_con(int32_t con) -> void { m_ref_con = con; }

        [[nodiscard]] lua_api auto interface_list() const -> int16_t { return m_interface_list; }
        lua_api auto set_interface_list(int16_t id) -> void { m_interface_list = id; }

        [[nodiscard]] lua_api auto auto_position() const -> uint16_t { return m_auto_position; }
        lua_api auto set_auto_position(uint16_t position) -> void { m_auto_position = position; }

        [[nodiscard]] lua_api auto title() const -> std::string { return m_title; }
        lua_api auto set_title(const std::string& title) -> void { m_title = title; }

    private:
        std::string m_title;
        math::rect m_bounds;
        int16_t m_proc_id { 0 };
        bool m_visible { true };
        bool m_go_away { true };
        int32_t m_ref_con { 0 };
        int16_t m_interface_list { 0 };
        uint16_t m_auto_position { 0 };
    };
}