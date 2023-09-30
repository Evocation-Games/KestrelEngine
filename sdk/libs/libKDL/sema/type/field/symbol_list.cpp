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
#include <libKDL/sema/type/field/symbol_list.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/decorator/decorator.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

auto kdl::sema::type_definition::field_definition::symbol_list::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::l_bracket).be_true()
    });
}

auto kdl::sema::type_definition::field_definition::symbol_list::parse(foundation::stream<tokenizer::token> &stream, context &ctx, resource::definition::type::field_value &field) -> void
{
    auto scope = ctx.create_scope();

    stream.ensure({ expectation(tokenizer::l_bracket).be_true() });
    while (true) {
        if (sema::decorator::test(stream)) {
            ctx.current_decorators = sema::decorator::parse(stream);
            continue;
        }
        else if (stream.expect({
            expectation(tokenizer::documentation).optional(),
            expectation(tokenizer::identifier).be_true(),
            expectation(tokenizer::equals).be_true()
        })) {
            std::string documentation;
            if (stream.expect({ expectation(tokenizer::documentation).be_true() })) {
                documentation = stream.read().string_value();
            }

            auto name = stream.read(); stream.advance();
            auto value_statement = script::parse_statement(stream, ctx);
            auto value_result = value_statement.evaluate(scope);
            if (value_result.status == interpreter::script::statement::result::error) {
                throw diagnostic(name, diagnostic::reason::KDL018);
            }

            resource::definition::type::symbol *symbol = nullptr;
            switch (value_result.value.type()) {
                case interpreter::token::integer: {
                    symbol = &field.add_symbol(name.string_value(), resource::value_container(value_result.value.integer_value()));
                    break;
                }
                case interpreter::token::string: {
                    symbol = &field.add_symbol(name.string_value(), resource::value_container(value_result.value.string_value()));
                    break;
                }
                case interpreter::token::reference: {
                    symbol = &field.add_symbol(name.string_value(), resource::value_container(value_result.value.reference_value()));
                    break;
                }
                default: break;
            }

            if (symbol) {
                symbol->add_decorators(ctx.current_decorators.decorators);
                symbol->add_decorator(decorator::name::documentation, documentation);
            }
            ctx.current_decorators.decorators.clear();
        }
        else if (stream.expect({ expectation(tokenizer::r_bracket).be_true() })) {
            break;
        }
        else {
            throw diagnostic(stream.peek(), diagnostic::reason::KDL006);
        }

        if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
            stream.advance();
            continue;
        }
        break;
    }
    stream.ensure({ expectation(tokenizer::r_bracket).be_true() });

    ctx.pop_scope();
}