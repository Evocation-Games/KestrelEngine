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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/rect.hpp>

namespace kestrel::ui
{
    struct dialog_layout;

    enum lua_api(UI.Anchor, Available_0_8) anchor : std::uint8_t
    {
        left lua_case(Left, Available_0_8) = 0x01,
        top lua_case(Top, Available_0_8) = 0x02,
        right lua_case(Right, Available_0_8) = 0x04,
        bottom lua_case(Bottom, Available_0_8) = 0x08,
        vertical lua_case(Vertical, Available_0_8) = 0x0A,
        horizontal lua_case(Horizontal, Available_0_8) = 0x05
    };

    enum lua_api(UI.ControlType, Available_0_8) control_type : std::uint8_t
    {
        none lua_case(None, Available_0_8),
        button lua_case(Button, Available_0_8),
        label lua_case(Label, Available_0_8),
        text_area lua_case(TextArea, Available_0_8),
        image lua_case(Image, Available_0_8),
        text_field lua_case(TextField, Available_0_8),
        checkbox lua_case(CheckBox, Available_0_8),
        list lua_case(List, Available_0_8),
        scroll_area lua_case(ScrollArea, Available_0_8),
        grid lua_case(Grid, Available_0_8),
        labeled_list lua_case(LabeledList, Available_0_8),
        canvas lua_case(Canvas, Available_0_8),
        sprite lua_case(Sprite, Available_0_8),
        popup_button lua_case(PopupButton, Available_0_8),
        slider lua_case(Slider, Available_0_8),
        table lua_case(Table, Available_0_8),
        box lua_case(Box, Available_0_8),
        radio lua_case(Radio, Available_0_8),
        tabbar lua_case(TabBar, Available_0_8),
        separator lua_case(Separator, Available_0_8),
        spacer lua_case(Spacer, Available_0_9),
        position lua_case(Position, Available_0_9)
    };

    class lua_api(UI.ControlDefinition, Available_0_8) control_definition
    {
    public:
        has_constructable_lua_api(control_definition);

        control_definition(const dialog_layout *layout, const std::string& name, enum control_type type);

        auto set_element_index_vector(const std::vector<std::uint16_t>& v) -> void;
        auto add_element_index_vector(std::uint16_t idx) -> void;

        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_getter(anchor, Available_0_8) [[nodiscard]] auto anchor() const -> std::uint8_t;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(type, Available_0_8) [[nodiscard]] auto type() const -> std::uint8_t;
        lua_getter(suggestedValue, Available_0_8) [[nodiscard]] auto suggested_value() const -> std::string;

        lua_setter(anchor, Available_0_8) auto set_anchor(std::uint8_t anchor) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;
        lua_setter(type, Available_0_8) auto set_type(std::uint8_t type) -> void;

        auto recalculate_frame() -> void;

    private:
        const dialog_layout *m_layout { nullptr };
        std::string m_name;
        enum control_type m_type { control_type::none };
        enum anchor m_anchor { 0 };
        math::rect m_frame;
        std::vector<std::uint16_t> m_element_index_vec;
    };


}
