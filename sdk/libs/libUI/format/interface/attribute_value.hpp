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
#include <variant>
#include <libUI/format/interface/action.hpp>
#include <libUI/format/interface/value.hpp>
#include <libUI/types/rect.hpp>

namespace ui::format
{
    struct attribute_value
    {
    public:
        enum class type : std::uint16_t
        {
            string = 0x0001,
            integer = 0x0002,
            unsigned_integer = 0x0003,
            boolean = 0x0004,
            rect = 0x0005,
            action = 0x0006,
            value = 0x0007,
            color_code = 0x0008,
        };

        explicit attribute_value(const std::string& value);
        explicit attribute_value(std::int64_t value);
        explicit attribute_value(std::uint64_t value);
        explicit attribute_value(std::uint32_t value);
        explicit attribute_value(bool value);
        explicit attribute_value(const rect& value);
        explicit attribute_value(const struct action& value);
        explicit attribute_value(const struct value& value);

        [[nodiscard]] auto type() const -> enum type;

        template<typename T>
        [[nodiscard]] auto value() const -> T
        {
            return std::get<T>(m_value);
        }

    private:
        std::variant<std::string, std::int64_t, std::uint64_t, bool, rect, struct action, struct value, std::uint32_t> m_value;
    };
}