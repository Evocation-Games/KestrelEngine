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

#include <libFoundation/string/split.hpp>
#include <libResource/definition/template/instance.hpp>

// MARK: - Construction


// MARK: - Field Management

auto resource::definition::binary_template::instance::add_field(struct field &field) -> void
{
    m_fields.emplace(field.hash(), field);
    m_field_order.emplace_back(field.hash());
}

auto resource::definition::binary_template::instance::add_field(struct type &type, const std::string &label) -> void
{
    field new_field(type, label);
    add_field(new_field);
}

auto resource::definition::binary_template::instance::add_field(enum type::$type type, const std::string &label) -> void
{
    field new_field(type, label);
    add_field(new_field);
}

auto resource::definition::binary_template::instance::add_field(enum type::$type type, std::uint16_t size, const std::string &label) -> void
{
    field new_field(type, size, label);
    add_field(new_field);
}

auto resource::definition::binary_template::instance::field_count() const -> std::size_t
{
    return m_fields.size();
}

auto resource::definition::binary_template::instance::field_named(const std::string &name) const -> const field&
{
    return field_named(foundation::string::split(name, "."));
}

auto resource::definition::binary_template::instance::field_named(const std::vector<std::string> &name) const -> const field &
{
    auto it = m_fields.find(foundation::hashing::string(name.at(0)));
    if (it == m_fields.end()) {
        throw std::runtime_error("");
    }

    if (name.size() == 1) {
        return it->second;
    }
    else if (it->second.nested_type()) {
        const auto *nested = it->second.nested_type();
        return nested->field_named(std::vector<std::string>(name.begin() + 1, name.end()));
    }
    else if (it->second.is_list()) {
        return *it->second.list_field_named(name.at(1));
    }
    else {
        throw std::runtime_error("");
    }
}

auto resource::definition::binary_template::instance::has_field_named(const std::string& name) const -> bool
{
    return m_fields.find(foundation::hashing::string(name)) != m_fields.end();
}

auto resource::definition::binary_template::instance::all_fields() const -> std::vector<field>
{
    std::vector<field> out;
    for (const auto& field : m_field_order) {
        auto it = m_fields.find(field);
        if (it != m_fields.end()) {
            out.emplace_back(it->second);
        }
    }
    return out;
}