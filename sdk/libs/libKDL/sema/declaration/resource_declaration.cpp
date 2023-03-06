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

#include <libKDL/sema/declaration/resource_declaration.hpp>
#include <libKDL/sema/declaration/field_declaration.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/spec/keywords.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libResource/reference.hpp>

auto kdl::sema::declaration::resource::test(const foundation::stream<tokenizer::token>& stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::declare_keyword).be_true(),
    });
}

auto kdl::sema::declaration::resource::parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void
{
    stream.ensure({ expectation(tokenizer::declare_keyword).be_true() });

    if (!stream.expect_any({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::identifier_path).be_true() })) {
        throw std::runtime_error("");
    }
    auto declaration_type = stream.read();
    auto type = ctx.type_named(declaration_type.string_value());
    ctx.current_type = type;

    stream.ensure({ expectation(tokenizer::l_brace).be_true() });
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (stream.expect_any({
            expectation(tokenizer::new_keyword).be_true(),
            expectation(tokenizer::override_keyword).be_true(),
            expectation(tokenizer::duplicate_keyword).be_true(),
        })) {
            // New/Override/Duplicate Resource
            auto resource = parse_resource(stream, ctx);
            if (declaration_type.is(tokenizer::identifier_path) && !resource.reference().has_container()) {
                auto container = declaration_type.associated_values().front();
                resource.set_reference(resource.reference().with_container(container));
            }
            ctx.resources.emplace_back(resource);
        }
        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}

auto kdl::sema::declaration::resource::parse_resource(foundation::stream<tokenizer::token> &stream, context &ctx, bool is_inline) -> ::resource::instance
{
    auto scope = ctx.create_scope();
    if (is_inline && stream.expect_any({
        expectation(tokenizer::override_keyword).be_true(),
        expectation(tokenizer::duplicate_keyword).be_true(),
    })) {
        throw std::runtime_error("");
    }
    auto action = stream.read();

    // Override and Duplicate actions are not allowed on inline declarations...
    if (is_inline && (action.is(tokenizer::override_keyword) || action.is(tokenizer::duplicate_keyword))) {
        throw std::runtime_error("");
    }

    ::resource::reference id;
    ::resource::reference source_id;
    std::string name;

    if (!is_inline || stream.expect({ expectation(tokenizer::l_paren).be_true() })) {
        stream.ensure({ expectation(tokenizer::l_paren).be_true() });
        while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
            // Extract the expression to use
            auto statement = sema::script::parse_statement(stream, ctx);
            auto result = statement.evaluate(scope);

            if (result.value.is(interpreter::token::integer)) {
                // This was likely a reference, and should be transmuted back to one.
                id = id.with_id(result.value.integer_value());
            }
            else if (result.value.is(interpreter::token::string)) {
                name = result.value.string_value();
            }
            else {
                throw std::runtime_error("");
            }

            if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                stream.advance();
                continue;
            }
            else if (stream.expect({ expectation(tokenizer::r_paren).be_true() })) {
                break;
            }
            else {
                throw std::runtime_error("");
            }
        }
        stream.ensure({ expectation(tokenizer::r_paren).be_true() });
    }

    // Setup the resource instance
    ::resource::instance resource(id.with_type_name(ctx.current_type->name()));
    resource.set_name(name);

    // Start parsing the body of the declaration
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (field::test(stream)) {
            resource.set_values(field::parse(stream, ctx));
        }
        else {
            throw std::runtime_error("");
        }
        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });

    return std::move(resource);
}