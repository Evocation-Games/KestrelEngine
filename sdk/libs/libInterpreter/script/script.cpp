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

#include <libInterpreter/script/script.hpp>

// MARK: - Construction

interpreter::script::script::script(const std::initializer_list<statement> &statements)
    : m_statements(statements)
{}

interpreter::script::script::script(const std::vector<statement> &statements)
    : m_statements(statements)
{}

// MARK: - Statement Management

auto interpreter::script::script::add_statement(const statement &stmt) -> void
{
    m_statements.emplace_back(stmt);
}

// MARK: - Watch Variables

auto interpreter::script::script::add_variable_watch(const std::string &var) -> void
{
    m_output_variables.insert(var);
}

// MARK: - Executions

auto interpreter::script::script::execute(scope *scope) const -> result
{
    struct result result;
    auto script_scope = scope->subscope();

    // Listen for watched variables.
    script_scope.notify_changes([&] (const variable& var, scope::notify_reason reason) {
        if (m_output_variables.contains(var.name())) {
            auto it = result.variables.find(var.name());
            if (it != result.variables.end()) {
                it->second = var.value();
            }
            else {
                result.variables.emplace(var.name(), var.value());
            }
        }
    });

    for (auto statement : m_statements) {
        auto statement_result = statement.evaluate(&script_scope);
        result.result = statement_result.value;

        switch (statement_result.status) {
            case statement::result::error: {
                result.result = token(static_cast<std::int64_t>(0));
                result.status = result::status::error;
                break;
            }
            case statement::result::ok: {
                result.status = result::status::success;
                break;
            }
            case statement::result::returned: {
                return result;
            }
        }
    }

    return result;
}