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

#include <stdexcept>
#include <libInterpreter/construct/function.hpp>
#include <libInterpreter/token/token.hpp>
#include <libInterpreter/scope/scope.hpp>
#include <libInterpreter/script/statement.hpp>

// MARK: - Construction

interpreter::function::function(const std::string &name, const native_function_body& body)
    : m_name(name), m_body(body)
{}

interpreter::function::function(const std::string &name, const foundation::stream<token>& stream)
    : m_name(name), m_body(stream)
{}

// MARK: - Accessors

auto interpreter::function::name() const -> std::string
{
    return m_name;
}

auto interpreter::function::hash() const -> foundation::hashing::value
{
    return foundation::hashing::string(m_name);
}

// MARK: - Argument Management

auto interpreter::function::expected_argument_count() const -> std::size_t
{
    return m_parameters.size();
}

auto interpreter::function::argument_name_at(std::int32_t idx) const -> std::string
{
    return std::get<0>(m_parameters.at(idx));
}

auto interpreter::function::argument_type_at(std::int32_t idx) const -> enum token::type
{
    return std::get<1>(m_parameters.at(idx));
}

auto interpreter::function::add_parameter(const std::string &name, enum token::type type) -> void
{
    m_parameters.emplace_back(name, type);
}

// MARK: - Execution

auto interpreter::function::execute(scope &scope, const std::vector<token> &arguments) const -> token
{
    if (arguments.size() < m_parameters.size()) {
        throw std::runtime_error("");
    }

    auto function_scope = scope.subscope();
    for (auto i = 0; i < expected_argument_count(); ++i) {
        function_scope.add_variable(argument_name_at(i), arguments.at(i));
    }

    for (auto i = 0; i < arguments.size(); ++i) {
        function_scope.add_variable(std::to_string(i + 1), arguments.at(i));
    }

    if (m_commit_variables) {
        function_scope.notify_changes([&] (const variable& var, scope::notify_reason reason) {
            scope.add_variable(var);
        });
    }

    if (m_body.index() == 0) {
        // Native Function
        return std::get<native_function_body>(m_body)(&function_scope, arguments);
    }
    else if (m_body.index() == 1) {
        // Token Stream
        auto stream = std::get<foundation::stream<token>>(m_body);
        interpreter::script::statement stmt(stream);
        return stmt.evaluate(&function_scope).value;
    }

    return token(0LL);
}