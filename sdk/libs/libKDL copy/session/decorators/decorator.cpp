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

#include <libKDL/session/decorators/decorator.hpp>

// MARK: - Construction

kdl::session::decorator::instance::instance(const lexer::lexeme &name, const std::vector<lexer::lexeme> &values)
    : m_name(name), m_associated_values(values)
{}

// MARK: - Accessors

auto kdl::session::decorator::instance::name() const -> lexer::lexeme
{
    return m_name;
}

auto kdl::session::decorator::instance::has_associated_values() const -> bool
{
    return !m_associated_values.empty();
}

auto kdl::session::decorator::instance::associated_value_count() const -> std::size_t
{
    return m_associated_values.size();
}

auto kdl::session::decorator::instance::associated_value(std::int32_t idx) const -> lexer::lexeme
{
    return m_associated_values[idx];
}

auto kdl::session::decorator::instance::has_associated_value(lexer::lexeme lx) const -> bool
{
    for (const auto& value : m_associated_values) {
        if (value.is(lx)) {
            return true;
        }
    }
    return false;
}

auto kdl::session::decorator::instance::has_associated_value(const std::string& value) const -> bool
{
    for (const auto& assoc : m_associated_values) {
        if (assoc.is(value)) {
            return true;
        }
    }
    return false;
}