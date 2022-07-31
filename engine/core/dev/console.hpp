// Copyright (c) 2021 Tom Hancocks
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

#include <functional>
#include "core/event/event.hpp"
#include "core/event/text_entry.hpp"
#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/entity.hpp"

namespace dev
{
    class console
    {
    public:
        console();

        auto set_size(const math::size& sz) -> void;
        auto set_visible(bool f) -> void;
        auto toggle_visibility() -> void;
        [[nodiscard]] auto is_visible() const -> bool;

        auto on_command(std::function<auto(const std::string&)->void> callback) -> void;

        auto write(const std::string& message) -> void;
        auto receive(const event& e) -> void;
        auto update() -> void;

        [[nodiscard]] auto entries() const -> const std::vector<std::string>&;
        auto send_command(const std::string& command) -> void;

        [[nodiscard]] auto entity() -> std::shared_ptr<graphics::entity>;

    private:
        std::function<auto(const std::string&)->void> m_on_command;
        math::size m_size;
        std::unique_ptr<graphics::canvas> m_canvas;
        std::shared_ptr<graphics::entity> m_entity;
        std::vector<std::string> m_history;
        text_entry_event m_input;
        ui::font::reference::lua_reference m_font { nullptr };
        bool m_dirty { true };
        bool m_visible { true };
    };
}
