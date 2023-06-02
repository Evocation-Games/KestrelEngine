// Copyright (c) 2022 Tom Hancocks
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

#include "lua/annotations/tag.hpp"
#include "analyzer/string_helpers.hpp"

// MARK: - Construction

kdtool::lua_api::annotation_tag::annotation_tag(const std::string &tag_string)
{
    auto components = helpers::strings::split(tag_string, "=");
    if (components.empty()) {
        m_name = name::unknown;
        return;
    }

    m_name = annotation_tag::name(components.at(0));

    std::string s(tag_string);
    s.erase(0, components.at(0).size() + 1);
    m_value = s;
}

kdtool::lua_api::annotation_tag::annotation_tag(enum name tag_name, const std::string &tag_value)
    : m_name(tag_name), m_value(tag_value)
{}

kdtool::lua_api::annotation_tag::annotation_tag(const std::string &tag_name, const std::string &tag_value)
    : m_name(name(tag_name)), m_value(tag_value)
{}

// MARK: - Name

auto kdtool::lua_api::annotation_tag::name(const std::string &value) -> enum name
{
    if (value == constants::symbol) {
        return name::symbol;
    }
    else if (value == constants::available) {
        return name::available;
    }
    else if (value == constants::deprecated) {
        return name::deprecated;
    }
    else if (value == constants::undocumented) {
        return name::undocumented;
    }
    else if (value == constants::getter) {
        return name::getter;
    }
    else if (value == constants::setter) {
        return name::setter;
    }
    else if (value == constants::constructor) {
        return name::constructor;
    }
    else if (value == constants::is_mutable) {
        return name::is_mutable;
    }
    else if (value == constants::as_namespace) {
        return name::as_namespace;
    }
    else if (value == constants::enrollment) {
        return name::enrollment;
    }
    else if (value == constants::reference) {
        return name::reference;
    }
    else if (value == constants::named) {
        return name::named;
    }
    else if (value == constants::type_fix) {
        return name::type_fix;
    }
    else if (value == constants::template_variant) {
        return name::template_variant;
    }
    else {
        return name::unknown;
    }
}

// MARK: - Accessors

auto kdtool::lua_api::annotation_tag::name() const -> enum name
{
    return m_name;
}

auto kdtool::lua_api::annotation_tag::value() const -> std::string
{
    return m_value;
}