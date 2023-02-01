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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/event/text_entry.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/event/responder/responder_chain.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/font.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Text, Available_0_8) text_widget: public responder_chain::mouse_responder,  public responder_chain::responder
    {
    public:
        has_constructable_lua_api(text_widget);

        lua_constructor(Available_0_8) explicit text_widget(double width);

        lua_function(draw, Available_0_8) auto draw() -> void;
        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;

        lua_getter(text, Available_0_8) [[nodiscard]] auto text() const -> std::string;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference ;
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto color() const -> graphics::color::lua_reference;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_getter(borderColor, Available_0_8) [[nodiscard]] auto border_color() const -> graphics::color::lua_reference;
        lua_getter(selectionColor, Available_0_8) [[nodiscard]] auto selection_color() const -> graphics::color::lua_reference;
        lua_getter(cursorColor, Available_0_8) [[nodiscard]] auto cursor_color() const -> graphics::color::lua_reference;
        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point;
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;

        lua_setter(text, Available_0_8) auto set_text(const std::string& v) -> void;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;
        lua_setter(textColor, Available_0_8) auto set_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(borderColor, Available_0_8) auto set_border_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(selectionColor, Available_0_8) auto set_selection_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(cursorColor, Available_0_8) auto set_cursor_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(position, Available_0_8) auto set_position(const math::point& v) -> void;
        lua_setter(size, Available_0_8) auto set_size(const math::size& v) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& v) -> void;

        auto did_become_first_responder() -> void override;
        auto did_resign_first_responder() -> void override;
        auto receive_event(const event& e) -> bool override;

    private:
        bool m_is_first_responder { false };
        bool m_dirty { true };
        font::reference::lua_reference m_font { nullptr };
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
