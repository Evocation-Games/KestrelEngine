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
#include <libResource/definition/type/constructor.hpp>
#include <libInterpreter/script/script.hpp>
#include <libResource/definition/template/instance.hpp>

// MARK: - Construction

resource::definition::type::constructor::constructor(const std::vector<std::string> &arguments)
    : m_arguments(arguments)
{}

// MARK: - Argument Management

auto resource::definition::type::constructor::arguments() const -> std::vector<std::string>
{
    return m_arguments;
}

auto resource::definition::type::constructor::argument_count() const -> std::size_t
{
    return m_arguments.size();
}

auto resource::definition::type::constructor::has_arguments() const -> bool
{
    return !m_arguments.empty();
}

auto resource::definition::type::constructor::argument_at(std::int32_t idx) const -> std::string
{
    return m_arguments.at(idx);
}

// MARK: - Script

auto resource::definition::type::constructor::script() const -> const interpreter::script::script &
{
    return m_script;
}

auto resource::definition::type::constructor::set_script(const interpreter::script::script &script) -> void
{
    m_script = script;
}

auto resource::definition::type::constructor::set_script(const std::vector<interpreter::script::statement> &statements) -> void
{
    m_script = interpreter::script::script(statements);
}

// MARK: - Execution

auto resource::definition::type::constructor::execute(
    interpreter::scope &scope,
    const std::vector<interpreter::token> &arguments,
    const resource::definition::binary_template::instance& tmpl
)
    const -> result
{
    auto script = m_script;
    for (const auto& field : tmpl.all_fields()) {
        script.add_variable_watch("this." + field.label());
    }

    // Make sure the provided argument count matches the expected argument count.
    if (arguments.size() != m_arguments.size()) {
        throw std::runtime_error("");
    }

    // Inject the arguments into the scope.
    auto constructor_scope = scope.subscope();
    for (auto i = 0; i < m_arguments.size(); ++i) {
        constructor_scope.add_variable(m_arguments.at(i), arguments.at(i));
    }

    // Execute the script, and receive the results of the execution.
    result constructor_result;
    auto script_result = script.execute(&constructor_scope);

    switch (script_result.status) {
        case interpreter::script::script::result::status::success: {
            constructor_result.variables = script_result.variables;
            break;
        }
        case interpreter::script::script::result::status::error: {
            break;
        }
    }

    return constructor_result;
}