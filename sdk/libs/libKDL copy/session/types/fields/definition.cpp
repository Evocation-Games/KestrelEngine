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

#include <libKDL/session/types/fields/definition.hpp>
#include <libKDL/exception/unknown_value_exception.hpp>

// MARK: - Construction

kdl::session::type_field::type_field(const lexer::lexeme &name)
    : m_name(name), m_decorators({})
{}

kdl::session::type_field::type_field(const std::string &name)
    : m_name(name, lexer::lexeme::identifier), m_decorators({})
{}

// MARK: - Accessors

auto kdl::session::type_field::name() const -> lexer::lexeme
{
    return m_name;
}

// MARK: - Decorators

auto kdl::session::type_field::decorators() const -> const decorator::collection&
{
    return m_decorators;
}

auto kdl::session::type_field::set_decorators(const std::vector<decorator::instance> &decorators) -> void
{
    m_decorators = decorator::collection(decorators);
}

auto kdl::session::type_field::set_decorators(const decorator::collection &collection) -> void
{
    m_decorators = collection;
}

// MARK: - Repeatable

auto kdl::session::type_field::repeatable() -> type_field_repeatable&
{
    return m_repeatable;
}

auto kdl::session::type_field::make_repeatable(std::int32_t lower, std::int32_t upper) -> type_field_repeatable&
{
    m_repeatable = type_field_repeatable(true);
    m_repeatable.set_bounds(lower, upper);
    return m_repeatable;
}

// MARK: - Command Encoder

auto kdl::session::type_field::is_command_encoder() const -> bool
{
    // TODO: Implement this...
    return false;
}

// MARK: - Lua Setter

auto kdl::session::type_field::wants_lua_setter() const -> bool
{
    if (m_decorators.has_decorator_named(decorator::key::api_name)) {
        return m_decorators.decorator_named(decorator::key::api_name)
                           .has_associated_value("setter");
    }
    return false;
}

// MARK: - Synthesize

auto kdl::session::type_field::should_synthesize() const -> bool
{
    return m_decorators.has_decorator_named(decorator::key::synthesize_name);
}

// MARK: - Values

auto kdl::session::type_field::add_value(const type_field_value& value) -> void
{
    m_values.emplace_back(value);
}

auto kdl::session::type_field::expected_values() const -> std::size_t
{
    return m_values.size();
}

auto kdl::session::type_field::value_at(std::int32_t idx) -> type_field_value&
{
    return m_values.at(idx);
}

auto kdl::session::type_field::value_named(const lexer::lexeme& name) -> type_field_value&
{
    for (auto& value : m_values) {
        if (value.base_name().is(name.text())) {
            return value;
        }
        else if (value.export_name().is(name)) {
            return value;
        }
    }
    throw unknown_value_exception("Could not find value named '" + name.text() + "' in field '" + this->name().text() + "'", name);
}
