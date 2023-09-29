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

#include "project/structure/class/property/property_definition.hpp"

// MARK: - Getter Management

auto kdtool::project::structure::property_definition::getter() const -> std::shared_ptr<struct function_definition>
{
    return m_getter;
}

auto kdtool::project::structure::property_definition::add_getter(const std::shared_ptr<struct function_definition>& function) -> void
{
    m_getter = function;
}

// MARK: - Setter Management

auto kdtool::project::structure::property_definition::setter() const -> std::shared_ptr<struct function_definition>
{
    return m_setter;
}

auto kdtool::project::structure::property_definition::add_setter(const std::shared_ptr<struct function_definition>& function) -> void
{
    m_setter = function;
}