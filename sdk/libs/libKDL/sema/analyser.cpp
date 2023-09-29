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
#include <libKDL/sema/scene/interface.hpp>
#include <libKDL/sema/context.hpp>
#include <libKDL/sema/module/module.hpp>
#include <libKDL/spec/decorators.hpp>

// MARK: - Construction

kdl::sema::analyser::analyser(const foundation::stream<tokenizer::token> &tokens, const std::vector<std::string>& definitions)
    : m_tokens(tokens), m_definitions(definitions)
{}

// MARK: - Semantic Analysis Base

auto kdl::sema::analyser::process(context& ctx) -> void
{
    // Setup definitions in the context
    for (const auto& definition : m_definitions) {
        ctx.definitions.insert(definition);
    }

    // Setup a global scope.
    ctx.create_scope();

    while (!m_tokens.finished()) {
        if (sema::directive::test(m_tokens)) {
            sema::directive::parse(m_tokens, ctx);
        }
        else if (sema::decorator::test(m_tokens)) {
            ctx.current_decorators = sema::decorator::parse(m_tokens);
            continue;
        }
        else if (sema::module_definition::test(m_tokens)) {
            sema::module_definition::parse(m_tokens, ctx);
        }
        else if (sema::type_definition::test(m_tokens)) {
            sema::type_definition::parse(m_tokens, ctx);
        }
        else if (sema::declaration::resource::test(m_tokens)) {
            ctx.flags.surpress_resource_creation = !ctx.evaluate_decorators();
            ctx.field_repeat_counts.clear();
            sema::declaration::resource::parse(m_tokens, ctx);
            ctx.flags.surpress_resource_creation = false;
        }
        else if (sema::component::test(m_tokens)) {
            ctx.flags.surpress_resource_creation = !ctx.evaluate_decorators();
            sema::component::parse(m_tokens, ctx);
            ctx.flags.surpress_resource_creation = false;
        }
        else if (sema::scene_interface::test(m_tokens)) {
            ctx.flags.surpress_resource_creation = !ctx.evaluate_decorators();
            sema::scene_interface::parse(m_tokens, ctx);
            ctx.flags.surpress_resource_creation = false;
        }

        m_tokens.ensure({ expectation(tokenizer::semi).be_true() });
        ctx.current_decorators.decorators.clear();
    }
}