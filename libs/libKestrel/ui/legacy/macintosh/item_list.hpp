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

#include <vector>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/macro.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::legacy::macintosh::toolbox
{
    enum class lua_api(Legacy.Macintosh.Toolbox.DialogItemKind, Available_0_8) item_type : std::uint8_t
    {
        user_item lua_case(UserItem, Available_0_8) = 0,
        help_item lua_case(HelpItem, Available_0_8) = 1,
        button lua_case(Button, Available_0_8) = 4,
        checkbox lua_case(Checkbox, Available_0_8) = 5,
        radio lua_case(Radio, Available_0_8) = 6,
        control lua_case(Control, Available_0_8) = 7,
        static_text lua_case(StaticText, Available_0_8) = 8,
        edit_text lua_case(EditText, Available_0_8) = 16,
        icon lua_case(Icon, Available_0_8) = 32,
        picture lua_case(Picture, Available_0_8) = 64,
        disable lua_case(Disable, Available_0_8) = 128,
    };

    struct lua_api(Legacy.Macintosh.Toolbox.DialogItem, Available_0_8) item
    {
        has_lua_api;
        lua_data(frame, const, Available_0_8) math::rect frame { 0, 0, 0, 0 };
        lua_data(type, const, Available_0_8) item_type type { item_type::user_item };
        lua_data(info, const, Available_0_8) std::string info { "" };
    };

    struct lua_api(Legacy.Macintosh.Toolbox.DialogItemList, Available_0_8) item_list
    {
    public:
        has_constructable_lua_api(item_list);
        is_resource_type("DITL");

        lua_constructor(Available_0_8) explicit item_list(const resource::descriptor::lua_reference& ref);
        lua_function(load, Available_0_8) static auto load(const resource::descriptor::lua_reference& ref) -> lua_reference;

        lua_getter(itemCount, Available_0_8) [[nodiscard]] auto item_count() const -> std::uint16_t { return m_item_count; }
        lua_function(itemAt, Available_0_8) [[nodiscard]] auto at(std::uint16_t idx) const -> const item& { return m_items[idx - 1]; }

    private:
        std::uint16_t m_item_count { 0 };
        std::vector<item> m_items;
    };
}

