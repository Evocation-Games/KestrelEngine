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

#include <libKDL/sema/declaration/field_declaration.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/spec/keywords.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libResource/reference.hpp>

auto kdl::sema::declaration::resource::field::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::equals).be_true() });
}

auto kdl::sema::declaration::resource::field::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> void
{
    if (!test(stream)) {
        throw std::runtime_error("");
    }
    auto field_name = stream.read(); stream.advance();

    // Look up the field in the type definition.
    if (!ctx.current_type->has_field_named(field_name.string_value())) {
        throw std::runtime_error("");
    }
    auto field = ctx.current_type->field_named(field_name.string_value());

    // We need to parse out values, for each expected value.
    std::int32_t value_index = 0;
    while (stream.expect({ expectation(tokenizer::semi).be_false() })) {
//        auto expected_value = field.
    }

}