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

#include <libInterpreter/script/statement.hpp>
#include <libInterpreter/token/expectation.hpp>

// MARK: - Construction

interpreter::script::statement::statement(const foundation::stream<token> &tokens)
    : m_tokens(tokens)
{}

// MARK: - Accessors

auto interpreter::script::statement::token_stream() const -> foundation::stream<token>
{
    return m_tokens;
}

// MARK: - Evaluation

auto interpreter::script::statement::evaluate(scope *scope) -> result
{
    m_tokens.reset();
    struct context context { .scope = scope };
    auto assignment_var = token::id("");


    if (m_tokens.size() == 1) {
        context.result = evaluate_single_token_statement(context);
        if (context.result.status != result::error) {
            goto STATEMENT_END;
        }
    }

    if (m_tokens.expect({ expectation(token::identifier, "return").be_true() })) {
        // This statement is considered a terminator.
        context.result.status = result::returned;
        m_tokens.advance();
    }
    else if (m_tokens.expect({ expectation(token::identifier).be_true(), expectation(token::equals).be_true() })) {
        assignment_var = m_tokens.read(); m_tokens.advance();
    }
    else if (m_tokens.expect({ expectation(token::identifier_path).be_true(), expectation(token::equals).be_true() })) {
        assignment_var = m_tokens.read(); m_tokens.advance();
    }

    while (!m_tokens.finished()) {
        // Determine what course of action needs to be taken.
        evaluate_variable(context);
        evaluate_function_call(context);

        iterate_shunting_yard(context);
    }
    evaluate_shunting_yard(context);

STATEMENT_END:
    if (!assignment_var.is("") && (assignment_var.type() == token::identifier || assignment_var.type() == token::identifier_path)) {
        scope->add_variable(assignment_var.string_value(), context.result.value);
    }

    return context.result;
}

// MARK: -

auto interpreter::script::statement::evaluate_single_token_statement(struct context &context) -> result
{
    result r;
    r.status = result::error;

    if (m_tokens.expect_any({
        expectation(token::integer).be_true(),
        expectation(token::string).be_true(),
        expectation(token::decimal).be_true(),
        expectation(token::boolean).be_true(),
        expectation(token::reference).be_true()
    })) {
         r.value = m_tokens.read();
         r.type = r.value.type();
         r.status = result::ok;
    }
    else if (m_tokens.expect({
        expectation(token::identifier).be_true()
    })) {
        evaluate_variable(context);
        r.value = m_tokens.read();
        r.type = r.value.type();
        r.status = result::ok;
    }
    else if (m_tokens.expect({
        expectation(token::function_result).be_true()
    })) {
        r.value = m_tokens.read();
        r.type = r.value.type();
        r.status = r.value.bool_value() ? result::ok : result::error;
    }

    return r;
}

auto interpreter::script::statement::evaluate_argument(struct context &context) -> result
{
    result result;
    std::int32_t nesting = 0;

    std::vector<token> argument_tokens;

    while (true) {
        if (m_tokens.expect({ expectation(token::l_paren).be_true() })) {
            nesting++;
            argument_tokens.emplace_back(m_tokens.read());
        }
        else if (nesting > 0 && m_tokens.expect({ expectation(token::r_paren).be_true() })) {
            nesting--;
            argument_tokens.emplace_back(m_tokens.read());
        }
        else if (nesting <= 0 && m_tokens.expect_any({
            expectation(token::r_paren).be_true(),
            expectation(token::comma).be_true()
        })) {
            break;
        }
        else {
            argument_tokens.emplace_back(m_tokens.read());
        }
    }

    foundation::stream<token> argument_stream(argument_tokens);
    statement argument_statement(argument_stream);

    return argument_statement.evaluate(context.scope);
}

auto interpreter::script::statement::evaluate_function_call(struct context& context) -> void
{
    if (m_tokens.expect({
        expectation(token::identifier).be_true(),
        expectation(token::l_paren).be_true()
    })) {
        auto function_name = m_tokens.peek();
        if (context.scope->has_function(function_name.string_value(), true)) {
            m_tokens.advance(2);

            // Extract arguments for the function
            std::vector<token> arguments;
            while (m_tokens.expect({ expectation(token::r_paren).be_false() })) {
                auto argument = evaluate_argument(context);
                arguments.emplace_back(argument.value);

                if (m_tokens.expect({ expectation(token::comma).be_true() })) {
                    m_tokens.advance();
                    continue;
                }

                m_tokens.ensure({ expectation(token::r_paren).be_true() });
                break;
            }

            // Call the function and pass in the arguments to it. Push the result of the function,
            // into the stream.
            auto result = context.scope->call(function_name.string_value(), arguments);
            m_tokens.push(result);
        }
    }
}

auto interpreter::script::statement::evaluate_variable(struct context& context) -> void
{
    if (m_tokens.expect_any({
        expectation(token::identifier).be_true()
    })) {
        // Variable
        const auto variable = m_tokens.peek();
        if (context.scope->has_variable(variable.string_value(), true)) {
            auto value = context.scope->variable(variable.string_value());
            value.set_source_variable(variable.string_value());
            m_tokens.advance();
            m_tokens.push(value);
        }
    }
}

// MARK: - Shunting Yard

auto interpreter::script::statement::iterate_shunting_yard(struct context& context) -> void
{
    const auto& token = m_tokens.read();

    if (token.is(token::string) || token.is(token::integer) || token.is(token::decimal) || token.is(token::boolean) || token.is(token::reference)) {
        context.output_queue.emplace_back(token);
    }
    else if (token.is(token::function_result)) {
        context.output_queue.emplace_back(token);
    }
    else if (
        token.is(token::plus) || token.is(token::minus) || token.is(token::multiply) || token.is(token::divide) ||
        token.is(token::carat) || token.is(token::amp) || token.is(token::pipe) || token.is(token::tilde) ||
        token.is(token::left_shift) || token.is(token::right_shift) || token.is(token::increment) || token.is(token::decrement)
    ) {
        while (!context.operator_stack.empty()) {
            const auto& o2 = context.operator_stack.back();
            if ((!o2.is(token::l_paren)) && (o2.operator_value() >= token.operator_value() && token.left_associative())) {
                context.output_queue.emplace_back(context.operator_stack.back());
                context.operator_stack.pop_back();
            }
            else {
                break;
            }
        }
        context.operator_stack.emplace_back(token);
    }
    else if (token.is(token::l_paren)) {
        context.operator_stack.emplace_back(token::l_paren);
    }
    else if (token.is(token::l_paren)) {
        while (!context.operator_stack.empty() && context.operator_stack.back().is(token::l_paren)) {
            context.output_queue.emplace_back(context.operator_stack.back());
            context.operator_stack.pop_back();
        }

        if (!context.operator_stack.back().is(token::l_paren)) {
            throw std::runtime_error("");
        }

        context.operator_stack.pop_back();
    }
}

auto interpreter::script::statement::evaluate_shunting_yard(struct context &context) -> void
{
    while (!context.operator_stack.empty()) {
        context.output_queue.emplace_back(context.operator_stack.back());
        context.operator_stack.pop_back();
    }

    std::vector<token> working_stack;
    for (const auto& tk : context.output_queue) {
        if (tk.is(token::integer) || tk.is(token::string) || tk.is(token::decimal) || tk.is(token::boolean) || tk.is(token::reference)) {
            working_stack.emplace_back(tk);
        }
        else if (tk.is(token::function_result)) {
            working_stack.emplace_back(tk);
        }
        else if (tk.is(token::plus)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs + rhs);
        }
        else if (tk.is(token::minus)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs - rhs);
        }
        else if (tk.is(token::multiply)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs * rhs);
        }
        else if (tk.is(token::divide)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs / rhs);
        }
        else if (tk.is(token::carat)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs ^ rhs);
        }
        else if (tk.is(token::amp)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs & rhs);
        }
        else if (tk.is(token::tilde)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(~lhs);
        }
        else if (tk.is(token::pipe)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs | rhs);
        }
        else if (tk.is(token::left_shift)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs << rhs);
        }
        else if (tk.is(token::right_shift)) {
            const auto rhs = working_stack.back(); working_stack.pop_back();
            const auto lhs = working_stack.back(); working_stack.pop_back();
            working_stack.emplace_back(lhs >> rhs);
        }
        else if (tk.is(token::increment)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            if (lhs.is_variable()) {
                auto& var = context.scope->get_variable(lhs.source_variable());
                var.set_value(lhs + token(1LL));
            }
            working_stack.emplace_back(lhs + token(1LL));
        }
        else if (tk.is(token::decrement)) {
            const auto lhs = working_stack.back(); working_stack.pop_back();
            if (lhs.is_variable()) {
                auto& var = context.scope->get_variable(lhs.source_variable());
                var.set_value(lhs - token(1LL));
            }
            working_stack.emplace_back(lhs - token(1LL));
        }
    }

    if (working_stack.empty() || working_stack.size() > 1) {
        throw std::runtime_error("");
    }

    context.result.value = token(working_stack.back());

    // TODO: Validate the result matches the expected result type?
    context.result.type = context.result.value.type();
}
