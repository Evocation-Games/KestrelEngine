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

#include <libResource/declaration/instance.hpp>

// MARK: - Construction

resource::instance::instance(const resource::reference& reference)
    : m_reference(reference)
{}

// MARK: - Accessors

auto resource::instance::reference() const -> resource::reference
{
    return m_reference;
}

auto resource::instance::set_reference(const resource::reference &reference) -> void
{
    m_reference = reference;
}

auto resource::instance::name() const -> std::string
{
    return m_name;
}

auto resource::instance::set_name(const std::string &name) -> void
{
    m_name = name;
}

auto resource::instance::type() const -> const definition::type::instance *
{
    return m_type;
}

auto resource::instance::values() const -> const std::unordered_map<std::string, value_container> &
{
    return m_values;
}

// MARK: - Value Management

auto resource::instance::set_values(const std::unordered_map<std::string, value_container> &values) -> void
{
    for (const auto& it : values) {
        set_value(it.first, it.second);
    }
}

auto resource::instance::set_value(const std::string &key, const value_container &value) -> void
{
    auto it = m_values.find(key);
    if (it == m_values.end()) {
        m_values.emplace(key, value);
    }
    else {
        it->second = value;
    }
}