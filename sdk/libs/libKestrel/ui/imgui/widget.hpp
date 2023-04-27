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

#include <string>
#include <imgui/imgui.h>
#include <libKestrel/util/uid.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::imgui
{
    class widget
    {
    public:
        explicit widget(util::uid id = {})
            : m_id(static_cast<ImGuiID>(id))
        {
            m_id_string = "##" + std::to_string(m_id);
        }

        [[nodiscard]] inline auto identifier_string() const -> const char *
        {
            return m_id_string.c_str();
        }

        [[nodiscard]] virtual auto position() const -> math::point
        {
            return m_position;
        }

        virtual auto set_position(const math::point& position) -> void
        {
            m_position = position;
            m_has_position = true;
        }

        [[nodiscard]] auto has_position() const -> bool
        {
            return m_has_position;
        }

        [[nodiscard]] virtual auto size() const -> math::size
        {
            return m_size;
        }

        virtual auto set_size(const math::size& size) -> void
        {
            m_size = size;
            m_has_size = true;
        }

        [[nodiscard]] auto has_size() const -> bool
        {
            return m_has_size;
        }

        virtual auto draw() -> void = 0;

    private:
        ImGuiID m_id;
        std::string m_id_string;
        math::point m_position;
        math::size m_size;
        bool m_has_position { false };
        bool m_has_size { false };
    };
}