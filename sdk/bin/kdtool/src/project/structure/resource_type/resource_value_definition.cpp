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

#include "project/structure/resource_type/resource_value_definition.hpp"
#include "project/structure/symbol.hpp"

// MARK: - Accessors

auto kdtool::project::structure::resource_value_definition::name() const -> std::string
{
    return symbol()->display_name();
}

auto kdtool::project::structure::resource_value_definition::basename() const -> std::string
{
    return symbol()->basename();
}

auto kdtool::project::structure::resource_value_definition::type_name() const -> std::string
{
    return m_instance.type().spelling();
}

auto kdtool::project::structure::resource_value_definition::has_default_value() const -> bool
{
    return m_instance.has_default_value();
}

//MARK: - Instance

auto kdtool::project::structure::resource_value_definition::set_instance(const ::resource::definition::type::field_value& instance) -> void
{
    m_instance = instance;
}

auto kdtool::project::structure::resource_value_definition::instance() const -> const ::resource::definition::type::field_value&
{
    return m_instance;
}

// MARK: - Symbols

auto kdtool::project::structure::resource_value_definition::add_symbol(const std::shared_ptr<resource_value_symbol_definition>& symbol) -> void
{
    m_symbols.emplace_back(symbol);
}

auto kdtool::project::structure::resource_value_definition::all_symbols() const -> std::vector<std::shared_ptr<resource_value_symbol_definition>>
{
    return m_symbols;
}