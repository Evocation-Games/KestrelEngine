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

#include <libKDL/session/resources/container.hpp>

// MARK: - Construction

kdl::session::resources::type_container::type_container(const type_definition *type)
    : m_definition(type)
{}

// MARK: - Accessors

auto kdl::session::resources::type_container::definition() const -> const type_definition *
{
    return m_definition;
}

// MARK: - Automatic ID Lookup

auto kdl::session::resources::type_container::next_id() const -> reference::identifier
{
    // TODO: Implement this
    return 0;
}

// MARK: - Instance Management

auto kdl::session::resources::type_container::instance_exists(reference::identifier id) const -> bool
{
    return instance_exists(reference(id));
}

auto kdl::session::resources::type_container::instance_exists(const resources::reference &ref) const -> bool
{
    auto it = m_instances.find(ref.id());
    return it != m_instances.end();
}

auto kdl::session::resources::type_container::construct(reference::identifier id, const std::string &name, interpreter::scope& scope) -> instance&
{
    return construct(reference(id), name, scope);
}

auto kdl::session::resources::type_container::construct(const resources::reference &ref, const std::string &name, interpreter::scope& scope) -> instance&
{
    auto it = m_instances.find(ref.id());
    if (it != m_instances.end()) {
        // TODO: Throw an appropriate error here...
        throw std::runtime_error("");
    }

    instance resource(m_definition, ref.id(), name, scope);
    m_instances.insert(std::pair(ref.id(), resource));
    it = m_instances.find(ref.id());
    if (it == m_instances.end()) {
        // Throw an appropriate error here...
        throw std::runtime_error("");
    }
    return it->second;
}

