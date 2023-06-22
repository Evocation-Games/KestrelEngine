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

#include "project/structure/function/function_definition.hpp"

// MARK: - Parameter Management

auto kdtool::project::structure::function_definition::add(const std::shared_ptr<struct parameter_definition> &parameter) -> void
{
    // Check for documentation on the function, and see if it includes the parameter.
    // If it does, then apply the parameter description.
    if (auto docs = symbol()->documentation().lock()) {
        for (const auto& param_doc : docs->parameters()) {
            if (param_doc.name() == parameter->symbol()->name()) {
                parameter->set_description(param_doc.description());
                break;
            }
        }
    }
    m_parameters.emplace_back(parameter);
    update_display_name();
}

auto kdtool::project::structure::function_definition::all_parameters() const -> const std::vector<std::shared_ptr<struct parameter_definition>>&
{
    return m_parameters;
}

// MARK: - Return Type

auto kdtool::project::structure::function_definition::return_type() const -> std::shared_ptr<struct structure::type::base>
{
    return m_return_type;
}

auto kdtool::project::structure::function_definition::set_return_type(const std::shared_ptr<struct structure::type::base>& type) -> void
{
    m_return_type = type;
    update_display_name();
}

// MARK: - Display Name

auto kdtool::project::structure::function_definition::update_display_name() -> void
{
    std::string name = symbol()->name() + "(";
    for (const auto& param : m_parameters) {
        if (name.back() != '(') {
            name += ", ";
        }
        name += param->symbol()->display_name();
    }
    symbol()->set_display_name(name + ")");
}