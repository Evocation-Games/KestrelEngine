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

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "math/rect.hpp"

namespace ui
{

    struct dialog_layout;

    class control_definition : public scripting::lua::object
    {
    public:
        enum class type : uint8_t
        {
            none, button, label, text_area, image, text_field, checkbox, list,
            scroll_area, grid, labeled_list, canvas, sprite, popup_button, slider,
            table, box, radio, tabbar, separator
        };

        enum class anchor : uint8_t
        {
            left = 0x01,
            top = 0x02,
            right = 0x04,
            bottom = 0x08,

            vertical = 0x0A,
            horizontal = 0x05
        };

        typedef luabridge::RefCountedPtr<control_definition> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        control_definition(const dialog_layout *layout, const std::string& name, enum type type);

        auto set_element_index_vector(const std::vector<std::uint16_t>& v) -> void;
        auto add_element_index_vector(std::uint16_t idx) -> void;

        [[nodiscard]] lua_api auto name() const -> std::string;
        [[nodiscard]] lua_api auto anchor() const -> std::uint8_t;
        [[nodiscard]] lua_api auto frame() const -> math::rect;
        [[nodiscard]] lua_api auto type() const -> std::uint8_t;
        [[nodiscard]] lua_api auto suggested_value() const -> std::string;

        lua_api auto set_anchor(std::uint8_t anchor) -> void;
        lua_api auto set_frame(const math::rect& frame) -> void;
        lua_api auto set_type(std::uint8_t type) -> void;

        auto recalculate_frame() -> void;

    private:
        const dialog_layout *m_layout { nullptr };
        std::string m_name;
        enum type m_type { type::none };
        enum anchor m_anchor { 0 };
        math::rect m_frame;
        std::vector<std::uint16_t> m_element_index_vec;
    };

    // MARK: - Bitwise Operators

    inline auto operator& (enum control_definition::anchor lhs, enum control_definition::anchor rhs) -> enum control_definition::anchor
    {
        return static_cast<enum control_definition::anchor>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

}
