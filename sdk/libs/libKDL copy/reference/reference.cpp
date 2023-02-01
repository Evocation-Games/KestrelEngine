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

#include <libKDL/session/session.hpp>
#include <libKDL/session/resources/reference.hpp>
#include <libKDL/session/types/definition.hpp>

// MARK: - Construction

kdl::session::resources::reference::reference(identifier id, type_definition *type, const std::string &name)
    : m_id(id), m_type(type), m_namespace(name)
{}

kdl::session::resources::reference::reference(const lexer::lexeme &lx, const session *session)
    : m_id(lx.is("auto") ? auto_value : lx.value<identifier>())
{
    if (session) {
        const auto& components = lx.components();
        if (!components.empty()) {
            for (auto i = 0; i < components.size() - 1; ++i) {
                const auto& value = components[i];
                if (session->has_type_definition(value)) {
                    m_type = session->type_definition(value);
                }
                else {
                    m_namespace = value;
                }
            }
        }
    }
}

// MARK: - Accessors

auto kdl::session::resources::reference::id() const -> identifier
{
    return m_id;
}

auto kdl::session::resources::reference::type() const -> const type_definition *
{
    return m_type;
}

auto kdl::session::resources::reference::namespace_name() const -> std::string
{
    return m_namespace;
}

auto kdl::session::resources::reference::has_type() const -> bool
{
    return m_type != nullptr;
}

auto kdl::session::resources::reference::has_namespace() const -> bool
{
    return !m_namespace.empty();
}

auto kdl::session::resources::reference::is_auto() const -> bool
{
    return m_id == reference::auto_value;
}

auto kdl::session::resources::reference::is_namespace(const std::string &name) const -> bool
{
    return m_namespace == name;
}

auto kdl::session::resources::reference::is_type(const std::string &type) const -> bool
{
    return (m_type != nullptr) && (m_type->name() == type || m_type->code() == type);
}

// MARK: -

auto kdl::session::resources::reference::with(identifier id) const -> reference
{
    reference ref(*this);
    ref.m_id = id;
    return ref;
}

// MARK: -

auto kdl::session::resources::reference::lexeme() const -> lexer::lexeme
{
    std::vector<std::string> components;

    if (has_namespace()) {
        components.emplace_back(namespace_name());
    }

    if (has_type()) {
        components.emplace_back(type()->name());
    }

    components.emplace_back(std::to_string(id()));

    return { components, lexer::lexeme::identifier, 0, 0, 0, {} };
}