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

#include <libResource/decorator/decoratable.hpp>

// MARK: - Decorator Management

auto resource::decoratable::add_decorator(const std::string &name, const std::vector<std::string> &values) -> void
{
    add_decorator(decorator(name, values));
}

auto resource::decoratable::add_decorator(const decorator &decorator) -> void
{
    m_decorators.insert(decorator);
}

auto resource::decoratable::add_decorators(const std::vector<decorator> &decorators) -> void
{
    for (const auto& decorator : decorators) {
        add_decorator(decorator);
    }
}

// MARK: - Decorator Look Up

auto resource::decoratable::all_decorators() const -> std::vector<decorator>
{
    std::vector<decorator> out;
    for (const auto& decorator : m_decorators) {
        out.emplace_back(decorator);
    }
    return out;
}

auto resource::decoratable::decorators_named(const std::string &name) const -> std::vector<decorator>
{
    std::vector<decorator> out;
    for (const auto& decorator : m_decorators) {
        if (decorator.name() == name) {
            out.emplace_back(decorator);
        }
    }
    return out;
}

auto resource::decoratable::has_decorator(const std::string &name, const std::vector<std::string> &values) const -> bool
{
    return m_decorators.contains(decorator(name, values));
}

auto resource::decoratable::has_decorator(const std::string &name) const -> bool
{
    for (const auto& decorator : m_decorators) {
        if (decorator.name() == name) {
            return true;
        }
    }
    return false;
}