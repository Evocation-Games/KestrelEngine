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

#include <vector>
#include <libKDL/sema/directive/function.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/script/script.hpp>

auto kdl::sema::directive::function::parse(foundation::stream<tokenizer::token> &stream, sema::context& ctx) -> void
{
    if (!stream.expect_any({ expectation(tokenizer::function_directive).be_true() })) {
        throw std::runtime_error("");
    }
    auto directive = stream.read();

    if (!stream.expect({ expectation(tokenizer::identifier).be_true() })) {
        throw std::runtime_error("");
    }
    auto name = stream.read();

    stream.ensure({ expectation(tokenizer::equals).be_true() });
    auto stmt_stream = script::parse_statement(stream, ctx).token_stream();
    interpreter::function funtion(name.string_value(), stmt_stream);

    ctx.active_scope()->add_function({ name.string_value(), stmt_stream });
}
