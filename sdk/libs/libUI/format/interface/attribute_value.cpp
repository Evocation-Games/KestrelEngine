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

#include <stdexcept>
#include <libUI/format/interface/attribute_value.hpp>

// MARK: - Construction

ui::format::attribute_value::attribute_value(const std::string& value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(std::int64_t value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(std::uint64_t value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(std::uint32_t value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(bool value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(const rect& value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(const struct action& value)
    : m_value(value)
{}

ui::format::attribute_value::attribute_value(const struct value& value)
    : m_value(value)
{}

// MARK: - Accessors

auto ui::format::attribute_value::type() const -> enum type
{
    switch (m_value.index()) {
        case 0:     return type::string;
        case 1:     return type::integer;
        case 2:     return type::unsigned_integer;
        case 3:     return type::boolean;
        case 4:     return type::rect;
        case 5:     return type::action;
        case 6:     return type::value;
        case 7:     return type::color_code;
    }
    throw std::runtime_error("Unrecognised attribute value type.");
}
