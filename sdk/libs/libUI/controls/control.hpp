// Copyright (c) 2023 Tom Hancocks
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
#include <concepts>
#include <functional>
#include <libUI/types/rect.hpp>
#include <libUI/event/event.hpp>
#include <libUI/format/interface/action.hpp>
#include <libUI/responder/responder.hpp>
#include <libUI/cells/cell.hpp>

namespace ui
{
    struct control: public responder
    {
    public:
        typedef std::function<auto(struct control *)->void> event_function;

        explicit control(const ui::rect& frame);

        [[nodiscard]] virtual auto frame() const -> ui::rect;
        [[nodiscard]] virtual auto disabled() const -> bool;
        [[nodiscard]] virtual auto hidden() const -> bool;
        [[nodiscard]] virtual auto mouse_inside() const -> bool;
        [[nodiscard]] virtual auto mouse_pressed() const -> bool;
        [[nodiscard]] virtual auto continuous_action() const -> bool;
        [[nodiscard]] virtual auto action() const -> ui::format::action;

        [[nodiscard]] virtual auto hit_test(const point& point) const -> bool;
        [[nodiscard]] virtual auto send_event(const event& event) -> bool override;

        virtual auto set_frame(const ui::rect& frame) -> void;
        virtual auto set_disabled(bool disabled) -> void;
        virtual auto set_hidden(bool hidden) -> void;
        virtual auto set_continuous(bool continuous) -> void;
        virtual auto set_action(const ui::format::action& action) -> void;

        virtual auto mouse_enter(const event& event) -> void;
        virtual auto mouse_exit(const event& event) -> void;
        virtual auto mouse_down(const event& event) -> void;
        virtual auto mouse_up(const event& event) -> void;
        virtual auto key_down(const event& event) -> void;
        virtual auto key_up(const event& event) -> void;
        virtual auto key_repeat(const event& event) -> void;

        virtual auto draw() -> void;
        virtual auto needs_redraw() -> void;

        virtual auto perform_action() -> void;

        virtual auto prepare_for_use() -> void;

        template<typename T, typename std::enable_if<std::is_base_of<ui::cell, T>::value>::type* = nullptr>
        auto set_cell() -> void
        {
            m_cell = std::make_unique<T>();
        }

    private:
        ui::rect m_frame { 0, 0, 0, 0 };
        bool m_dirty { true };
        bool m_disabled { false };
        bool m_hidden { false };
        bool m_pressed { false };
        bool m_inside { false };
        bool m_continuous { false };
        ui::format::action m_action;
        std::unique_ptr<ui::cell> m_cell;
    };
}