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

#include <libKDL/session/decorators/collection.hpp>
#include <libKDL/session/decorators/decorator.hpp>

// MARK: - Construction

kdl::session::decorator::collection::collection(const std::vector<instance> &decorators)
{
    for (const auto& decorator : decorators) {
        m_decorators.emplace(std::pair(decorator.name().text(), decorator));
    }
}

// MARK: - Accessors

auto kdl::session::decorator::collection::has_decorator_named(const std::string &name) const -> bool
{
    return m_decorators.find(name) != m_decorators.end();
}

auto kdl::session::decorator::collection::decorator_named(const std::string &name) const -> const instance&
{
    return m_decorators.find(name)->second;
}

// MARK: - Injection

auto kdl::session::decorator::collection::inject(const instance &decorator) -> void
{
    auto it = m_decorators.find(decorator.name().text());
    if (it == m_decorators.end()) {
        m_decorators.emplace(std::pair(decorator.name().text(), decorator));
    }
    else {
        it->second = decorator;
    }
}

