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

#include <libResource/definition/type/instance.hpp>

// MARK: - Construction

resource::definition::type::instance::instance(const std::string& name, const std::string& code)
    : m_name(name), m_code(code)
{}

// MARK: - Accessors

auto resource::definition::type::instance::name() const -> std::string
{
    return m_name;
}

auto resource::definition::type::instance::code() const -> std::string
{
    return m_code;
}

// MARK: - Template

auto resource::definition::type::instance::binary_template() const -> const binary_template::instance *
{
    return &m_template;
}

auto resource::definition::type::instance::set_binary_template(const binary_template::instance& tmpl) -> void
{
    m_template = tmpl;
}

// MARK: - Type Constructors

auto resource::definition::type::instance::constructors() const -> const std::vector<constructor>&
{
    return m_constructors;
}

auto resource::definition::type::instance::add_constructor(const constructor& constructor) -> void
{
    m_constructors.emplace_back(constructor);
}

// MARK: - Field Management

auto resource::definition::type::instance::all_fields() const -> std::vector<field>
{
    std::vector<field> out;
    for (const auto& [hash, field] : m_fields) {
        out.emplace_back(field);
    }
    return out;
}

auto resource::definition::type::instance::has_field_named(const std::string& name) const -> bool
{
    return m_fields.find(foundation::hashing::string(name)) != m_fields.end();
}

auto resource::definition::type::instance::field_named(const std::string& name) const -> const field&
{
    return m_fields.find(foundation::hashing::string(name))->second;
}

auto resource::definition::type::instance::add_field(const field& field) -> void
{
    auto it = m_fields.find(foundation::hashing::string(field.name()));
}

auto resource::definition::type::instance::add_field(const std::string& name) -> field&
{

}
