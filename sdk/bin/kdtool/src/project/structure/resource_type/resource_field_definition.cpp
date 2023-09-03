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

#include "project/structure/resource_type/resource_field_definition.hpp"
#include "project/structure/symbol.hpp"

// MARK: - Accessors

auto kdtool::project::structure::resource_field_definition::name() const -> std::string
{
    return symbol()->display_name();
}

auto kdtool::project::structure::resource_field_definition::basename() const -> std::string
{
    return symbol()->basename();
}

// MARK: - Instance

auto kdtool::project::structure::resource_field_definition::set_instance(const ::resource::definition::type::field &instance) -> void
{
    m_instance = instance;
}

auto kdtool::project::structure::resource_field_definition::instance() const -> const ::resource::definition::type::field &
{
    return m_instance;
}

// MARK: - Value Management

auto kdtool::project::structure::resource_field_definition::add_value(const std::shared_ptr<resource_value_definition> &value) -> void
{
    m_values.emplace_back(value);
}

auto kdtool::project::structure::resource_field_definition::all_values() const -> std::vector<std::shared_ptr<resource_value_definition>>
{
    return m_values;
}