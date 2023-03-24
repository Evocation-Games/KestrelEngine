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

#include <libKDL/sema/type/type_definition.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/type/template/template_definition.hpp>
#include <libKDL/sema/type/constructor/constructor_definition.hpp>
#include <libKDL/sema/type/field/field_definition.hpp>

auto kdl::sema::type_definition::test(const foundation::stream<tokenizer::token>& stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::type_keyword).be_true(),
        expectation(tokenizer::identifier).be_true(),
        expectation(tokenizer::colon).be_true(),
        expectation(tokenizer::string).be_true()
    });
}

auto kdl::sema::type_definition::parse(foundation::stream<tokenizer::token> &stream, context& ctx) -> void
{
    stream.ensure({ expectation(tokenizer::type_keyword).be_true() });
    const auto type_identifier = stream.read();
    stream.ensure({ expectation(tokenizer::colon).be_true() });
    const auto code_string = stream.read();
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    resource::definition::type::instance raw_type(type_identifier.string_value(), code_string.string_value());
    raw_type.add_decorators(ctx.current_decorators.decorators);
    auto type = const_cast<resource::definition::type::instance *>(ctx.current_type = ctx.register_type(raw_type));

    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (sema::decorator::test(stream)) {
            ctx.current_decorators = sema::decorator::parse(stream);
            continue;
        }
        else if (stream.expect({ expectation(tokenizer::template_keyword).be_true() })) {
            auto tmpl = template_definition::parse(stream, ctx);
            type->set_binary_template(tmpl);
            ctx.current_decorators.decorators.clear();
        }
        else if (stream.expect({ expectation(tokenizer::constructor_keyword).be_true() })) {
            auto constructor = constructor::parse(stream, ctx);
            constructor.add_decorators(ctx.current_decorators.decorators);
            type->add_constructor(constructor);
            ctx.current_decorators.decorators.clear();
        }
        else if (stream.expect({ expectation(tokenizer::field_keyword).be_true() })) {
            auto field = field_definition::parse(stream, ctx);
            type->add_field(field);
            ctx.current_decorators.decorators.clear();
        }
        else if (stream.expect({ expectation(tokenizer::assert_keyword).be_true() })) {
            ctx.current_decorators.decorators.clear();
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }

    stream.ensure({ expectation(tokenizer::r_brace).be_true() });

}
