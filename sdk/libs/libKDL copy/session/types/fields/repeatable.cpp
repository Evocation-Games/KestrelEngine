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

#include <libKDL/session/types/fields/repeatable.hpp>

// MARK: - Construction

kdl::session::type_field_repeatable::type_field_repeatable(bool enabled)
    : m_enabled(enabled)
{}

// MARK: - Accessors

auto kdl::session::type_field_repeatable::enabled() const -> bool
{
    return m_enabled;
}

auto kdl::session::type_field_repeatable::lower_bound() const -> std::int32_t
{
    return m_lower_bound;
}

auto kdl::session::type_field_repeatable::upper_bound() const -> std::int32_t
{
    return m_upper_bound;
}

auto kdl::session::type_field_repeatable::count_field() const -> const lexer::lexeme&
{
    return m_count_field;
}

auto kdl::session::type_field_repeatable::has_count_field() const -> bool
{
    return !m_count_field.is("");
}

// MARK: - Bounds Configuration

auto kdl::session::type_field_repeatable::set_bounds(std::int32_t lower, std::int32_t upper) -> void
{
    m_lower_bound = lower;
    m_upper_bound = upper;
}

auto kdl::session::type_field_repeatable::set_count_field(const lexer::lexeme &field) -> void
{
    m_count_field = field;
}