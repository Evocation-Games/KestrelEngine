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

#include <libUI/controls/popupbutton.hpp>

// MARK: - Construction

ui::popup_button::popup_button(const ui::rect &frame, const std::vector<std::string> &items)
    : ui::control(frame), m_items(items)
{}

// MARK: - Accessors

auto ui::popup_button::items() const -> std::vector<std::string>
{
    return m_items;
}

auto ui::popup_button::set_items(const std::vector<std::string> &items) -> void
{
    m_items = items;
}

auto ui::popup_button::add_item(const std::string &item) -> void
{
    m_items.emplace_back(item);
}

auto ui::popup_button::remove_all_items() -> void
{
    m_items.clear();
}

auto ui::popup_button::selected_item() const -> std::int32_t
{
    return m_selected_item;
}

auto ui::popup_button::select_item(const std::string &item) -> void
{
    for (auto i = 0; i < m_items.size(); ++i) {
        if (m_items[i] == item) {
            select_item(i);
            return;
        }
    }
    m_selected_item = -1;
}

auto ui::popup_button::select_item(std::int32_t index) -> void
{
    m_selected_item = index;
    if (index < 0 || index >= m_items.size()) {
        m_selected_item = -1;
    }
}

auto ui::popup_button::title_of_selected_item() const -> std::string
{
    if (m_selected_item != -1) {
        return m_items[m_selected_item];
    }
    return "";
}