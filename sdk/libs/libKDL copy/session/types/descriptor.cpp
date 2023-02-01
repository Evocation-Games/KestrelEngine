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

#include <libKDL/session/resources/reference.hpp>
#include <libKDL/session/types/descriptor.hpp>
#include <libKDL/session/types/definition.hpp>
#include <libKDL/session/types/binary_template/binary_template.hpp>
#include <libKDL/session/session.hpp>
#include <libKDL/session/interpreter/function.hpp>

// MARK: - Helpers

static auto is_unsigned_integer_type(enum kdl::session::binary_template::data_type type) -> bool
{
    return (
        type == kdl::session::binary_template::HBYT ||
        type == kdl::session::binary_template::HWRD ||
        type == kdl::session::binary_template::HLNG ||
        type == kdl::session::binary_template::HQAD
    );
}

static auto is_signed_integer_type(enum kdl::session::binary_template::data_type type) -> bool
{
    return (
        type == kdl::session::binary_template::DBYT ||
        type == kdl::session::binary_template::DWRD ||
        type == kdl::session::binary_template::DLNG ||
        type == kdl::session::binary_template::DQAD
    );
}

static auto is_integer_type(enum kdl::session::binary_template::data_type type) -> bool
{
    return is_unsigned_integer_type(type) || is_signed_integer_type(type);
}

static auto is_string_type(enum kdl::session::binary_template::data_type type) -> bool
{
    return (
        type == kdl::session::binary_template::CSTR ||
        (type & ~0xFFFUL) == kdl::session::binary_template::Cnnn ||
        type == kdl::session::binary_template::PSTR
    );
}

// MARK: - Validation

auto kdl::session::type_descriptor::verify_binary_type(enum kdl::session::binary_template::data_type type, const lexer::lexeme &nested_type_name) const -> void
{
    // Built-in types that are Nested complex types.
    if (m_name.is(nested_type_name)) {
        if (m_reference) {
            throw std::runtime_error("");
        }
        if (type != binary_template::NESTED) {
            throw std::runtime_error("");
        }
    }
    // Simple/Scalar Types
    else if (m_name.is(built_in_type_names::integer)) {
        if (m_reference) {
            throw std::runtime_error("");
        }
        if (!is_integer_type(type)) {
            throw std::runtime_error("");
        }
    }
    else if (m_name.is(built_in_type_names::range)) {
        if (m_reference) {
            throw std::runtime_error("");
        }

        if (m_hints.size() != 2) {
            throw std::runtime_error("");
        }

        if (is_integer_type(type)) {
            if (
                (m_hints.at(0).type() != m_hints.at(1).type()) ||
                !(
                    (m_hints.at(0).type() == lexer::lexeme::integer) ||
                    (m_hints.at(0).type() == lexer::lexeme::percentage)
                )
            ) {
                throw std::runtime_error("");
            }
        }
        else if (type == binary_template::RSRC) {
            if (
                (m_hints.at(0).type() != m_hints.at(1).type()) ||
                (m_hints.at(0).type() != lexer::lexeme::reference)
            ) {
                throw std::runtime_error("");
            }
        }
        else {
            throw std::runtime_error("");
        }
    }
    else if (m_name.is(built_in_type_names::bitmask)) {
        if (m_reference) {
            throw std::runtime_error("");
        }
        if (!is_unsigned_integer_type(type)) {
            throw std::runtime_error("");
        }
    }
    else if (m_name.is(built_in_type_names::string)) {
        if (m_reference) {
            throw std::runtime_error("");
        }
        if (!is_string_type(type)) {
            throw std::runtime_error("");
        }

        // String Length checks
    }
    // Complex Types
    else if (m_name.is(built_in_type_names::file)) {
        if (m_reference) {
            throw std::runtime_error("");
        }
    }
    else if (m_name.is(built_in_type_names::image)) {
        if (m_reference) {
            throw std::runtime_error("");
        }
    }
    // References
    else if (m_reference) {
        // The underlying type for a reference type must be 'RSRC'
        if (type != binary_template::RSRC) {
            throw std::runtime_error("");
        }

        if (!nested_type_name.is("") && !nested_type_name.is(m_name)) {
            throw std::runtime_error("");
        }
    }
}

// MARK: - Lexeme Type Verification

auto kdl::session::type_descriptor::type_matches(const lexer::lexeme &lx) const -> bool
{
    if (m_reference && lx.is(lexer::lexeme::reference)) {
        resources::reference ref(lx);
        const auto *type = ref.type();
        if (has_name()) {
            if (ref.has_type() && !m_name.is(type->name())) {
                return false;
            }
        }
    }
    else if (lx.is(lexer::lexeme::integer) || lx.is(lexer::lexeme::percentage)) {

    }
    return true;
}

// MARK: - Scope Injection

auto kdl::session::type_descriptor::inject(interpreter::scope &scope, const session *session) const -> void
{
    if (!session->has_type_definition(m_name.text())) {
        return;
    }

    const auto *type = session->type_definition(m_name.text());

    // Type Constructor
    const auto& constructors = type->constructors();
    if (!constructors.empty()) {
        scope.add_function(interpreter::function(type->name(), &constructors.back()));
    }
}