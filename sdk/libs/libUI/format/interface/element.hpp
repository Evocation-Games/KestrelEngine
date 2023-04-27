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

#include <cstdint>
#include <string>
#include <vector>
#include <libUI/format/interface/attribute.hpp>

namespace ui::format
{
    struct element
    {
    public:
        enum class type : std::uint16_t {
            unused = 0x0000,
            button = 0x0001,
            label = 0x0002,
            checkbox = 0x0003,
            popup_button = 0x0004,
            canvas = 0x0005,
            image = 0x0006,
            sprite = 0x0007,
            text_field = 0x0008,
            text_area = 0x0009,
            grid = 0x000a,
            list = 0x000b,
            separator = 0x000c,
            slider = 0x000d,
            radio_button = 0x000e,
            timed_trigger = 0x1000,
            key_trigger = 0x1001,
            vertical_spacer = 0x2000,
            horizontal_spacer = 0x2001,
            position = 0x2002
        };

        explicit element(enum type type);

        auto add_attribute(enum attribute::code code, const std::string& name, const attribute_value& value) -> void;
        auto add_attribute(enum attribute::code code, const std::string& name, const std::vector<attribute_value>& values) -> void;
        auto add_attribute(const struct attribute& attribute) -> void;

        [[nodiscard]] auto type() const -> enum type;
        [[nodiscard]] auto attribute_count() const -> std::size_t;
        [[nodiscard]] auto attribute() const -> struct attribute;
        [[nodiscard]] auto attributes() const -> std::vector<struct attribute>;

    private:
        enum type m_type { type::unused };
        std::vector<struct attribute> m_attributes;
    };
}
