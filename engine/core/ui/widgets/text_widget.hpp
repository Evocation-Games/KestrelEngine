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

#include <memory>
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/event/event.hpp"
#include "core/event/text_entry.hpp"
#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/color.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "core/event/responder/responder_chain.hpp"
#include "core/ui/entity/scene_entity.hpp"
#include "core/ui/font/font.hpp"

namespace ui::widgets
{
    struct text_widget: public scripting::lua::object, public responder_chain::responder
    {
    public:
        typedef luabridge::RefCountedPtr<text_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit lua_api text_widget(double width);

        auto draw() -> void;

        [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;

        [[nodiscard]] lua_api auto text() const -> std::string;
        [[nodiscard]] lua_api auto font() const -> ui::font::reference::lua_reference ;
        [[nodiscard]] lua_api auto color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto background_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto border_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto selection_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto cursor_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto position() const -> math::point;
        [[nodiscard]] lua_api auto size() const -> math::size;
        [[nodiscard]] lua_api auto frame() const -> math::rect;

        auto set_text(const std::string& v) -> void;
        auto set_font(const ui::font::reference::lua_reference& font) -> void;
        auto set_color(const graphics::color::lua_reference& v) -> void;
        auto set_background_color(const graphics::color::lua_reference& v) -> void;
        auto set_border_color(const graphics::color::lua_reference& v) -> void;
        auto set_selection_color(const graphics::color::lua_reference& v) -> void;
        auto set_cursor_color(const graphics::color::lua_reference& v) -> void;
        auto set_position(const math::point& v) -> void;
        auto set_size(const math::size& v) -> void;
        auto set_frame(const math::rect& v) -> void;

        auto did_become_first_responder() -> void override;
        auto did_resign_first_responder() -> void override;
        auto receive_event(const event& e) -> bool override;

    private:
        bool m_dirty { true };
        ui::font::reference::lua_reference m_font { nullptr };
        text_entry_event m_input;
        std::unique_ptr<graphics::canvas> m_canvas;
        scene_entity::lua_reference m_entity { nullptr };
        graphics::color m_color { graphics::color::black_color() };
        graphics::color m_border_color { graphics::color::black_color() };
        graphics::color m_selection_color { graphics::color::blue_color() };
        graphics::color m_cursor_color { graphics::color::black_color() };
        graphics::color m_background_color { graphics::color::white_color() };

        auto redraw_entity() -> void;
    };
}
