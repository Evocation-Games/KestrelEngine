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
#include <libKDL/sema/directive/variable.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

auto kdl::sema::directive::variable::parse(foundation::stream<tokenizer::token> &stream, sema::context& ctx) -> void
{
    if (!stream.expect_any({
        expectation(tokenizer::variable_directive).be_true(), expectation(tokenizer::constant_directive).be_true()
    })) {
        throw diagnostic(stream.peek(), diagnostic::reason::KDL000);
    }
    auto directive = stream.read();

    if (!stream.expect_any({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::variable).be_true() })) {
        throw diagnostic(stream.peek(), diagnostic::reason::KDL027);
    }
    auto name = stream.read();

    stream.ensure({ expectation(tokenizer::equals).be_true() });
    auto first_tk = stream.peek();
    auto stmt = script::parse_statement(stream, ctx);
    auto result = stmt.evaluate(ctx.create_scope());
    if (result.status == interpreter::script::statement::result::error) {
        throw diagnostic(first_tk, diagnostic::reason::KDL018);
    }
    ctx.active_scope()->add_variable(name.string_value(), result.value);
}
