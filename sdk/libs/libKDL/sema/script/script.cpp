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

#include <libKDL/sema/script/script.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libInterpreter/token/token.hpp>

auto kdl::sema::script::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> interpreter::script::script
{
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    std::vector<interpreter::script::statement> statements;

    // Keep looping until we have a closing brace denoting the end of the script.
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        auto statement = parse_statement(stream, ctx);
        statements.emplace_back(statement);
        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }

    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
    return interpreter::script::script(statements);
}

auto kdl::sema::script::parse_statement(foundation::stream<tokenizer::token> &stream, context &ctx) -> interpreter::script::statement
{
    foundation::stream<interpreter::token> statement_tokens;
    std::int32_t nesting = 0;

    // Keep looping until we have a semicolon, denoting the end of the statement
    while (true) {
        auto tk = stream.read();

        // Perform the mapping from the KDL tokens, into the interpreter tokens.
        if (tk.is(tokenizer::integer)) {
            statement_tokens.append(interpreter::token(tk.value<std::int64_t>()));
        }
        else if (tk.is(tokenizer::reference)) {
            statement_tokens.append(interpreter::token(tk.value<std::int64_t>()));
        }
        else if (tk.is(tokenizer::string)) {
            statement_tokens.append(interpreter::token(tk.string_value()));
        }
        else if (tk.is(tokenizer::identifier, "true")) {
            statement_tokens.append(interpreter::token(true));
        }
        else if (tk.is(tokenizer::identifier, "false")) {
            statement_tokens.append(interpreter::token(false));
        }
        else if (tk.is(tokenizer::plus)) {
            statement_tokens.append(interpreter::token(interpreter::token::plus));
        }
        else if (tk.is(tokenizer::minus)) {
            statement_tokens.append(interpreter::token(interpreter::token::minus));
        }
        else if (tk.is(tokenizer::star)) {
            statement_tokens.append(interpreter::token(interpreter::token::multiply));
        }
        else if (tk.is(tokenizer::slash)) {
            statement_tokens.append(interpreter::token(interpreter::token::divide));
        }
        else if (tk.is(tokenizer::amp)) {
            statement_tokens.append(interpreter::token(interpreter::token::amp));
        }
        else if (tk.is(tokenizer::pipe)) {
            statement_tokens.append(interpreter::token(interpreter::token::pipe));
        }
        else if (tk.is(tokenizer::tilde)) {
            statement_tokens.append(interpreter::token(interpreter::token::tilde));
        }
        else if (tk.is(tokenizer::exclaim)) {
            statement_tokens.append(interpreter::token(interpreter::token::exclaim));
        }
        else if (tk.is(tokenizer::left_shift)) {
            statement_tokens.append(interpreter::token(interpreter::token::left_shift));
        }
        else if (tk.is(tokenizer::right_shift)) {
            statement_tokens.append(interpreter::token(interpreter::token::right_shift));
        }
        else if (tk.is(tokenizer::identifier)) {
            statement_tokens.append(interpreter::token::id(tk.string_value()));
        }
        else if (tk.is(tokenizer::identifier_path)) {
            statement_tokens.append(interpreter::token::id(tk.associated_values()));
        }
        else if (tk.is(tokenizer::l_paren)) {
            statement_tokens.append(interpreter::token(interpreter::token::l_paren));
            nesting++;
        }
        else if (tk.is(tokenizer::r_paren) && nesting > 0) {
            statement_tokens.append(interpreter::token(interpreter::token::r_paren));
            nesting--;
        }
        else if (tk.is(tokenizer::comma) && nesting > 0) {
            statement_tokens.append(interpreter::token(interpreter::token::comma));
        }
        else if (tk.is(tokenizer::equals)) {
            statement_tokens.append(interpreter::token(interpreter::token::equals));
        }

        if (stream.expect({ expectation(tokenizer::comma).be_true() }) && nesting == 0) {
            break;
        }
        else if (stream.expect({ expectation(tokenizer::semi).be_true() })) {
            break;
        }
        else if (stream.expect({ expectation(tokenizer::r_brace).be_true() }) && nesting == 0) {
            break;
        }
        else if (stream.expect({ expectation(tokenizer::r_paren).be_true() }) && nesting == 0) {
            break;
        }
    }

    return interpreter::script::statement(statement_tokens);
}