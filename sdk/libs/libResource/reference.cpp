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

#include <libResource/reference.hpp>

// MARK: - Construction

resource::reference::reference(identifier id, const std::string& type_name, const std::string& container)
    : m_id(id), m_type_name(type_name), m_container(container)
{}

auto resource::reference::auto_assigning() -> reference
{
    return reference(auto_value);
}

auto resource::reference::any() -> reference
{
    return reference(any_value);
}

// MARK: - Accessors

auto resource::reference::id() const -> identifier
{
    return m_id;
}

auto resource::reference::type_name() const -> std::string
{
    return m_type_name;
}

auto resource::reference::container_name() const -> std::string
{
    return m_container;
}


auto resource::reference::has_type_name() const -> bool
{
    return !m_type_name.empty();
}

auto resource::reference::has_container() const -> bool
{
    return !m_container.empty();
}

auto resource::reference::is_auto() const -> bool
{
    return m_id == auto_value;
}

auto resource::reference::is_any() const -> bool
{
    return m_id == any_value;
}

auto resource::reference::is_type_name(const std::string& type_name) const -> bool
{
    return type_name == m_type_name;
}

auto resource::reference::is_container(const std::string& container) const -> bool
{
    return container == m_container;
}


auto resource::reference::with_id(identifier id) const -> reference
{
    return reference(id, m_type_name, m_container);
}

auto resource::reference::with_type_name(const std::string& type_name) const -> reference
{
    return reference(m_id, type_name, m_container);
}

auto resource::reference::with_container(const std::string& container) const -> reference
{
    return reference(m_id, m_type_name, container);
}
