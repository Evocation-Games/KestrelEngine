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

#include <iostream>
#include <libKDL/sema/analyser.hpp>
#include <libKDL/tokenizer/token.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/directive/directive.hpp>
#include <libKDL/sema/decorator/decorator.hpp>
#include <libKDL/sema/type/type_definition.hpp>
#include <libKDL/sema/declaration/resource_declaration.hpp>
#include <libKDL/sema/components/component.hpp>
#include <libKDL/sema/context.hpp>

// MARK: - Construction

kdl::sema::analyser::analyser(const foundation::stream<tokenizer::token> &tokens)
    : m_tokens(tokens)
{}

// MARK: - Semantic Analysis Base

auto kdl::sema::analyser::process() -> context
{
    context ctx;

    // Setup a global scope.
    ctx.create_scope();

    while (!m_tokens.finished()) {
        if (sema::directive::test(m_tokens)) {
            sema::directive::parse(m_tokens);
        }
        else if (sema::decorator::test(m_tokens)) {
            ctx.current_decorators = sema::decorator::parse(m_tokens);
            continue;
        }
        else if (sema::type_definition::test(m_tokens)) {
            sema::type_definition::parse(m_tokens, ctx);
        }
        else if (sema::declaration::resource::test(m_tokens)) {
            sema::declaration::resource::parse(m_tokens, ctx);
        }
        else if (sema::component::test(m_tokens)) {
            sema::component::parse(m_tokens, ctx);
        }

        m_tokens.ensure({ expectation(tokenizer::semi).be_true() });
        ctx.current_decorators.decorators.clear();
    }

    return std::move(ctx);
}