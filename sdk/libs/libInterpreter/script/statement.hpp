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

#pragma once

#include <libInterpreter/token/token.hpp>
#include <libInterpreter/scope/scope.hpp>
#include <libFoundation/stream/stream.hpp>

namespace interpreter::script
{
    struct statement
    {
    public:
        struct result
        {
            enum { ok, error, returned } status { ok };
            token value { 0LL };
            enum token::type type { token::integer };
        };

    public:
        statement() = default;
        explicit statement(const foundation::stream<token>& tokens);

        auto evaluate(scope *scope) -> result;

    private:
        struct context {
            struct result result;
            std::vector<token> output_queue;
            std::vector<token> operator_stack;
            scope *scope { nullptr };
        };

        foundation::stream<token> m_tokens;

        auto evaluate_single_token_statement(struct context& context) -> result;
        auto evaluate_function_call(struct context& context) -> void;
        auto evaluate_variable(struct context& context) -> void;

        auto iterate_shunting_yard(struct context& context) -> void;
        auto evaluate_shunting_yard(struct context& context) -> void;
    };
}