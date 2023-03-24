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

#include <libKDL/sema/type/template/template_definition.hpp>
#include <libKDL/sema/expectation/expectation.hpp>

auto kdl::sema::type_definition::template_definition::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::template_keyword).be_true(),
        expectation(tokenizer::l_brace).be_true()
    });
}

auto kdl::sema::type_definition::template_definition::parse(foundation::stream<tokenizer::token> &stream, context& ctx) -> resource::definition::binary_template::instance
{
    stream.ensure({ expectation(tokenizer::template_keyword).be_true(), expectation(tokenizer::l_brace).be_true() });

    resource::definition::binary_template::instance tmpl;

    while (test_binary_type(stream)) {
        auto type = binary_type(stream.read(), ctx);
        auto field_name = stream.read();
        stream.ensure({ expectation(tokenizer::semi).be_true() });
        tmpl.add_field(type, field_name.string_value());
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });

    return tmpl;
}

// MARK: - Binary Types

auto kdl::sema::type_definition::template_definition::test_binary_type(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect_any({
        expectation(tokenizer::DBYT).be_true(), expectation(tokenizer::DWRD).be_true(),
        expectation(tokenizer::DLNG).be_true(), expectation(tokenizer::DQWD).be_true(),
        expectation(tokenizer::HBYT).be_true(), expectation(tokenizer::HWRD).be_true(),
        expectation(tokenizer::HLNG).be_true(), expectation(tokenizer::HQWD).be_true(),
        expectation(tokenizer::RECT).be_true(), expectation(tokenizer::PSTR).be_true(),
        expectation(tokenizer::CSTR).be_true(), expectation(tokenizer::Cnnn).be_true(),
        expectation(tokenizer::LSTR).be_true(), expectation(tokenizer::OSTR).be_true(),
        expectation(tokenizer::CHAR).be_true(), expectation(tokenizer::HEXD).be_true(),
        expectation(tokenizer::OCNT).be_true(), expectation(tokenizer::LSTC).be_true(),
        expectation(tokenizer::LSTE).be_true(), expectation(tokenizer::BBIT).be_true(),
        expectation(tokenizer::BOOL).be_true(), expectation(tokenizer::RSRC).be_true(),
        expectation(tokenizer::NESTED).be_true(), expectation(tokenizer::BYTE_CODE).be_true()
    });
}

auto kdl::sema::type_definition::template_definition::binary_type(const tokenizer::token &token, context& ctx) -> resource::definition::binary_template::type
{
    if (token.is(tokenizer::DBYT)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::DBYT);
    }
    else if (token.is(tokenizer::DWRD)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::DWRD);
    }
    else if (token.is(tokenizer::DLNG)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::DLNG);
    }
    else if (token.is(tokenizer::DQWD)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::DQWD);
    }
    else if (token.is(tokenizer::HBYT)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::HBYT);
    }
    else if (token.is(tokenizer::HWRD)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::HWRD);
    }
    else if (token.is(tokenizer::HLNG)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::HLNG);
    }
    else if (token.is(tokenizer::HQWD)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::HQWD);
    }
    else if (token.is(tokenizer::RECT)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::RECT);
    }
    else if (token.is(tokenizer::PSTR)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::PSTR);
    }
    else if (token.is(tokenizer::CSTR)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::CSTR);
    }
    else if (token.is(tokenizer::Cnnn)) {
        return { resource::definition::binary_template::type::Cnnn, token.size_value() };
    }
    else if (token.is(tokenizer::LSTR)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::LSTR);
    }
    else if (token.is(tokenizer::OSTR)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::OSTR);
    }
    else if (token.is(tokenizer::CHAR)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::CHAR);
    }
    else if (token.is(tokenizer::HEXD)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::HEXD);
    }
    else if (token.is(tokenizer::OCNT)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::OCNT);
    }
    else if (token.is(tokenizer::LSTC)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::LSTC);
    }
    else if (token.is(tokenizer::LSTE)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::LSTE);
    }
    else if (token.is(tokenizer::BBIT)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::BBIT);
    }
    else if (token.is(tokenizer::BOOL)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::BOOL);
    }
    else if (token.is(tokenizer::RSRC)) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::RSRC);
    }
    else if (token.is(tokenizer::NESTED)) {
        // TODO: Lookup the actual type definition here and pass it instead.
        auto type = ctx.type_named(token.string_value());
        return { type->binary_template(), type->name() };
    }
    else if (token.is(tokenizer::BYTE_CODE, "Lua")) {
        return resource::definition::binary_template::type(resource::definition::binary_template::type::LUA_BYTE_CODE);
    }
    else {
        throw std::runtime_error("");
    }
}