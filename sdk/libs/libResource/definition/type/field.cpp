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

#include <libResource/definition/type/field.hpp>

// MARK: - Construction

resource::definition::type::field::field(const std::string &name)
    : m_name(name), m_hash(foundation::hashing::string(name))
{}

// MARK: - Accessors

auto resource::definition::type::field::name() const -> std::string
{
    return m_name;
}

auto resource::definition::type::field::hash() const -> hash_value
{
    return m_hash;
}

// MARK: - Repeatable

resource::definition::type::field::repeatable_info::repeatable_info(bool enabled)
    : m_enabled(enabled)
{}

auto resource::definition::type::field::repeatable_info::set_bounds(std::int32_t lower, std::int32_t upper) -> void
{
    m_lower_bound = lower;
    m_upper_bound = upper;
}

auto resource::definition::type::field::repeatable_info::set_count_field(const binary_template::field *field) -> void
{
    m_count_field = field;
}

auto resource::definition::type::field::repeatable_info::lower_bound() const -> std::int32_t
{
    return m_lower_bound;
}

auto resource::definition::type::field::repeatable_info::upper_bound() const -> std::int32_t
{
    return m_upper_bound;
}

auto resource::definition::type::field::repeatable_info::enabled() const -> bool
{
    return m_enabled && (m_count_field != nullptr);
}

auto resource::definition::type::field::repeatable_info::count_field() const -> const binary_template::field*
{
    return m_count_field;
}


auto resource::definition::type::field::repeatable_info::has_count_field() const -> bool
{
    return (m_count_field != nullptr);
}

auto resource::definition::type::field::repeatable() -> repeatable_info&
{
    return m_repeatable;
}

auto resource::definition::type::field::make_repeatable(std::int32_t lower, std::int32_t upper) -> repeatable_info &
{
    m_repeatable = repeatable_info(true);
    m_repeatable.set_bounds(lower, upper);
    return m_repeatable;
}

// MARK: - Values

auto resource::definition::type::field::values() const -> const std::vector<field_value> &
{
    return m_values;
}

auto resource::definition::type::field::value_count() const -> std::size_t
{
    return m_values.size();
}

auto resource::definition::type::field::value_at(std::int32_t idx) const -> const field_value &
{
    return m_values[idx];
}

auto resource::definition::type::field::add_value(const field_value &value) -> void
{
    m_values.emplace_back(value);
}