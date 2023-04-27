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
#include <vector>
#include <libUI/controls/control.hpp>

namespace ui
{
    struct popup_button: public control{
    public:
        explicit popup_button(const rect& frame, const std::vector<std::string>& items);

        [[nodiscard]] virtual auto selected_item() const -> std::int32_t;
        [[nodiscard]] virtual auto title_of_selected_item() const -> std::string;
        [[nodiscard]] virtual auto items() const -> std::vector<std::string>;

        auto select_item(const std::string& item) -> void;
        auto select_item(std::int32_t index) -> void;

        auto set_items(const std::vector<std::string>& items) -> void;
        auto add_item(const std::string& item) -> void;
        auto remove_all_items() -> void;

    private:
        std::vector<std::string> m_items;
        std::int32_t m_selected_item { 0 };
    };
}