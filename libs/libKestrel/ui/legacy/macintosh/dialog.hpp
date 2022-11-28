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

#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/ui/legacy/macintosh/item_list.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/macro.hpp>

namespace kestrel::ui::legacy::macintosh::toolbox
{
    struct lua_api(Legacy.Macintosh.Dialog, Available_0_8) dialog
    {
    public:
        is_resource_type("DLOG");
        has_constructable_lua_api(dialog);

        lua_constructor(Available_0_8) explicit dialog(const resource::descriptor::lua_reference& ref);
        lua_function(load, Available_0_8) static auto load(const resource::descriptor::lua_reference& ref) -> lua_reference;

        lua_getter(bounds, Available_0_8) [[nodiscard]] auto bounds() const -> math::rect { return m_bounds; }
        lua_setter(bounds, Available_0_8) auto set_bounds(const math::rect& bounds) -> void { m_bounds = bounds; }

        lua_getter(procId, Available_0_8) [[nodiscard]] auto proc_id() const -> std::int16_t { return m_proc_id; }
        lua_setter(procId, Available_0_8) auto set_proc_id(std::int16_t id) -> void { m_proc_id = id; }

        lua_getter(visible, Available_0_8) [[nodiscard]] auto visible() const -> bool { return m_visible; }
        lua_setter(visible, Available_0_8) auto set_visible(bool f) -> void { m_visible = f; }

        lua_getter(goAway, Available_0_8) [[nodiscard]] auto go_away() const -> bool { return m_go_away; }
        lua_setter(goAway, Available_0_8) auto set_go_away(bool f) -> void { m_go_away = f; }

        lua_getter(ref, Available_0_8) [[nodiscard]] auto ref_con() const -> std::int32_t { return m_ref_con; }
        lua_setter(ref, Available_0_8) auto set_ref_con(std::int32_t con) -> void { m_ref_con = con; }

        lua_getter(interfaceList, Available_0_8) [[nodiscard]] auto interface_list() const -> std::int16_t { return m_interface_list; }
        lua_setter(interfaceList, Available_0_8) auto set_interface_list(std::int16_t id) -> void { m_interface_list = id; }

        lua_getter(autoPosition, Available_0_8) [[nodiscard]] auto auto_position() const -> std::uint16_t { return m_auto_position; }
        lua_setter(autoPosition, Available_0_8) auto set_auto_position(std::uint16_t position) -> void { m_auto_position = position; }

        lua_getter(title, Available_0_8) [[nodiscard]] auto title() const -> std::string { return m_title; }
        lua_setter(title, Available_0_8) auto set_title(const std::string& title) -> void { m_title = title; }

    private:
        std::string m_title;
        math::rect m_bounds;
        std::int16_t m_proc_id { 0 };
        bool m_visible { true };
        bool m_go_away { true };
        std::int32_t m_ref_con { 0 };
        std::int16_t m_interface_list { 0 };
        std::uint16_t m_auto_position { 0 };
    };
}