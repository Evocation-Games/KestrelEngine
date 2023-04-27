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
#include <libUI/format/interface/attribute_value.hpp>

namespace ui::format
{
    struct attribute
    {
    public:
        enum class code : std::uint16_t {
            custom = 0x0000,
            name = 0x0001,
            frame = 0x0002,
            color = 0x0003,
            background_color = 0x0004,
            selection_color = 0x0005,
            border_color = 0x0006,
            secondary_color = 0x0007,
            font = 0x0008,
            font_size = 0x0009,
            hidden = 0x000a,
            disabled = 0x000b,
            weight = 0x000c,
            horizontal_alignment = 0x000d,
            vertical_alignment = 0x000e,
            axis_origin = 0x000f,
            x = 0x0010,
            y = 0x0011,
            size = 0x0012,
            action = 0x0020,
            value = 0x0030,
        };

        explicit attribute(enum code code, const std::string& name, const attribute_value& value);
        explicit attribute(enum code code, const std::string& name, const std::vector<attribute_value>& values);

        auto add_value(const attribute_value& value) -> void;

        [[nodiscard]] auto code() const -> enum code;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto value_count() const -> std::size_t;
        [[nodiscard]] auto value() const -> attribute_value;
        [[nodiscard]] auto values() const -> std::vector<attribute_value>;

    private:
        enum code m_code { code::custom };
        std::string m_name;
        std::vector<attribute_value> m_values;
    };
}