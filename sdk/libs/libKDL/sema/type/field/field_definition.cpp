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

#include <libKDL/sema/type/field/field_definition.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/type/descriptor.hpp>

auto kdl::sema::type_definition::field_definition::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::field_keyword).be_true(),
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::string).be_true(),
        expectation(tokenizer::r_paren).be_true()
    });
}

auto kdl::sema::type_definition::field_definition::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> resource::definition::type::field
{
    stream.ensure({
        expectation(tokenizer::field_keyword).be_true(),
        expectation(tokenizer::l_paren).be_true(),
    });
    auto field_name = stream.read();
    resource::definition::type::field field(field_name.string_value());
    stream.ensure({ expectation(tokenizer::r_paren).be_true() });

    // Repeatable?
    if (stream.expect({ expectation(tokenizer::repeatable_keyword).be_true() })) {

    }

    // Body
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (sema::decorator::test(stream)) {
            ctx.current_decorators = sema::decorator::parse(stream);
            continue;
        }
        else if (stream.expect_any({
            expectation(tokenizer::identifier).be_true(),
            expectation(tokenizer::identifier_path).be_true()
        })) {
            auto value = parse_value(stream, ctx);
            value.add_decorators(ctx.current_decorators.decorators);
            field.add_value(value);
            ctx.current_decorators.decorators.clear();
        }
        else {
            throw std::runtime_error("");
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });

    return field;
}

auto kdl::sema::type_definition::field_definition::parse_value(foundation::stream<tokenizer::token> &stream, context &ctx) -> resource::definition::type::field_value
{
    const auto value_identifier = stream.read();
    resource::definition::type::descriptor explicit_type(false);
    ctx.current_binary_field = &ctx.current_type->binary_template()->field_named(value_identifier.path_value("."));
    resource::definition::type::field_value value(ctx.current_binary_field);

    if (stream.expect({ expectation(tokenizer::l_angle).be_true() })) {
        // We have name extensions
        stream.advance();
        while (stream.expect({ expectation(tokenizer::r_angle).be_false() })) {
            if (stream.expect({ expectation(tokenizer::variable).be_true() })) {
                value.add_name_extension(stream.read().string_value());
            }
            else {
                throw std::runtime_error("");
            }

            if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                stream.advance();
                continue;
            }
            break;
        }
        stream.ensure({ expectation(tokenizer::r_angle).be_true() });
    }

    if (stream.expect({ expectation(tokenizer::as_keyword).be_true() })) {
        stream.advance();
        value.set_type(descriptor::parse(stream, ctx), true);
    }
    else {
        value.set_type(descriptor::infer_type(ctx), false);
    }

    if (stream.expect({ expectation(tokenizer::equals).be_true() })) {
        stream.advance();

        // We have a default value specified.
    }

    return value;
}
