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

#include <libKDL/sema/directive/directive.hpp>
#include <libKDL/sema/expectation/expectation.hpp>

#include <libKDL/sema/directive/out.hpp>
#include <libKDL/sema/directive/metadata.hpp>
#include <libKDL/sema/directive/format.hpp>

auto kdl::sema::directive::test(const foundation::stream<tokenizer::token>& stream) -> bool
{
    return stream.expect_any({
        expectation(tokenizer::project_directive).be_true(),
        expectation(tokenizer::author_directive).be_true(),
        expectation(tokenizer::version_directive).be_true(),
        expectation(tokenizer::license_directive).be_true(),
        expectation(tokenizer::copyright_directive).be_true(),
        expectation(tokenizer::email_directive).be_true(),
        expectation(tokenizer::website_directive).be_true(),
        expectation(tokenizer::out_directive).be_true(),
        expectation(tokenizer::format_directive).be_true()
    });
}

auto kdl::sema::directive::parse(foundation::stream<tokenizer::token>& stream) -> void
{
    if (stream.expect({ expectation(tokenizer::out_directive).be_true() })) {
        out::parse(stream);
    }
    else if (stream.expect_any({
        expectation(tokenizer::project_directive).be_true(),
        expectation(tokenizer::author_directive).be_true(),
        expectation(tokenizer::version_directive).be_true(),
        expectation(tokenizer::license_directive).be_true(),
        expectation(tokenizer::copyright_directive).be_true(),
        expectation(tokenizer::email_directive).be_true(),
        expectation(tokenizer::website_directive).be_true(),
    })) {
        metadata::parse(stream);
    }
    else if (stream.expect({ expectation(tokenizer::format_directive).be_true() })) {
        format::parse(stream);
    }
    else {
        throw std::runtime_error("");
    }
}
