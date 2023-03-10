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

#include <libResource/decorator/decorator.hpp>

// MARK: - Construction

resource::decorator::decorator::decorator(const std::string& name, const std::vector<std::string>& assoc_values)
    : m_name(name), m_assoc_values(assoc_values)
{}

// MARK: - Accessors

auto resource::decorator::name() const -> std::string
{
    return m_name;
}

auto resource::decorator::hash() const -> foundation::hashing::value
{
    return foundation::hashing::string(description());
}

auto resource::decorator::description() const -> std::string
{
    std::string out = m_name;
    for (const auto& value : m_assoc_values) {
        out += ":" + value;
    }
    return out;
}

auto resource::decorator::associated_values() const -> std::vector<std::string>
{
    return m_assoc_values;
}

auto resource::decorator::has_associated_values() const -> bool
{
    return !m_assoc_values.empty();
}

auto resource::decorator::associated_value_count() const -> std::size_t
{
    return m_assoc_values.size();
}

auto resource::decorator::associated_value_at(std::int32_t i) const -> std::string
{
    return m_assoc_values.at(i);
}

auto resource::decorator::has(const std::string& value) const -> bool
{
    for (const auto& it : m_assoc_values) {
        if (it == value) {
            return true;
        }
    }
    return false;
}

auto resource::decorator::operator<(const decorator& other) const -> bool
{
    return hash() < other.hash();
}
