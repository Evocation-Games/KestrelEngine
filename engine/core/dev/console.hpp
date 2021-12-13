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

#if !defined(CONSOLE_HPP)
#define CONSOLE_HPP

#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/event/text_entry.hpp"
#include "core/event/key.hpp"

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

        auto receive(const event::key& key) -> void;
        auto update() -> void;

        [[nodiscard]] auto entity() -> graphics::entity::lua_reference;

    private:
        math::size m_size;
        std::unique_ptr<graphics::canvas> m_canvas;
        graphics::entity::lua_reference m_entity;
        std::vector<std::string> m_history;
        event::control::text_entry m_input;
        bool m_dirty { true };
        bool m_visible { true };
    };
}


#endif //CONSOLE_HPP
