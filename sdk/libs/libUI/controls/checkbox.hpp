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
#include <libUI/controls/control.hpp>

namespace ui
{
    struct checkbox: public control
    {
    public:
        explicit checkbox(const rect& frame, const std::string& label, bool value = false);

        [[nodiscard]] virtual auto label() const -> std::string;
        [[nodiscard]] virtual auto value() const -> bool;

        auto set_label(const std::string& label) -> void;
        auto set_value(bool value) -> void;

        virtual auto toggle() -> void;

        auto mouse_down(const event& event) -> void override;
        auto mouse_up(const event& event) -> void override;

    private:
        std::string m_label;
        bool m_value { false };
    };
}