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

#include "project/structure/class/class_definition.hpp"
#include "project/structure/symbol.hpp"

// MARK: - Enrollment Management

auto kdtool::project::structure::class_definition::set(const std::shared_ptr<struct enrollment> &enrollment) -> void
{
    construct_definition::set(enrollment);
}

// MARK: - Constructor Management

auto kdtool::project::structure::class_definition::set(const std::shared_ptr<struct constructor_definition>& constructor) -> void
{
    m_constructor = constructor;
}

auto kdtool::project::structure::class_definition::constructor() const -> std::shared_ptr<struct constructor_definition>
{
    return m_constructor;
}

// MARK: - Function Management

auto kdtool::project::structure::class_definition::add(const std::shared_ptr<function_definition>& function) -> void
{
    m_functions.emplace_back(function);
}

auto kdtool::project::structure::class_definition::all_functions() const -> std::vector<std::shared_ptr<struct function_definition>>
{
    return m_functions;
}

// MARK: - Property Management

auto kdtool::project::structure::class_definition::property(const std::shared_ptr<struct symbol> &symbol) -> std::shared_ptr<struct property_definition>
{
    auto it = m_properties.find(symbol->name());
    if (it == m_properties.end()) {
        auto property = std::make_shared<struct property_definition>(symbol);
        symbol->set_definition(property);
        symbol->set_display_name(symbol->name());
        m_properties.emplace(symbol->name(), property);
        return property;
    }
    return it->second;
}


auto kdtool::project::structure::class_definition::all_properties() const -> std::vector<std::shared_ptr<struct property_definition>>
{
    std::vector<std::shared_ptr<struct property_definition>> out;
    out.reserve(m_properties.size());
    for (const auto& property : m_properties) {
        out.emplace_back(property.second);
    }
    return std::move(out);
}