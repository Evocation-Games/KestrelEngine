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

#include <libKDL/modules/module/module_definition.hpp>

// MARK: - Construction

kdl::module_definition::module_definition(const std::string &name)
    : m_name(name)
{}

// MARK: - Accessors

auto kdl::module_definition::name() const -> std::string
{
    return m_name;
}

auto kdl::module_definition::dependencies() const -> std::vector<std::string>
{
    return m_dependencies;
}

auto kdl::module_definition::imports() const -> std::vector<foundation::filesystem::path>
{
    return m_imports;
}

// MARK: - Configuration

auto kdl::module_definition::add_dependency(const std::string &name) -> void
{
    m_dependencies.emplace_back(name);
}

auto kdl::module_definition::add_import(const std::string &path) -> void
{
    m_imports.emplace_back(path);
}