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
#include <stdexcept>
#include <libKDL/sema/directive/format.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

static auto format_names() -> std::vector<std::string>
{
    return std::vector<std::string>({
        kdl::sema::directive::format::types::extended,
        kdl::sema::directive::format::types::kestrel,
        kdl::sema::directive::format::types::rsrc,
        kdl::sema::directive::format::types::classic,
        kdl::sema::directive::format::types::rez,
    });
}

auto kdl::sema::directive::format::parse(foundation::stream<tokenizer::token> &stream) -> void
{
    stream.ensure({ expectation(tokenizer::format_directive).be_true() });

    if (stream.expect({ expectation(tokenizer::identifier, "set").be_true(), expectation(tokenizer::identifier, format_names()).be_true() })) {
        stream.advance(2);
    }
    else if (stream.expect({ expectation(tokenizer::identifier, "requires").be_true(), expectation(tokenizer::identifier, format_names()).be_true() })) {
        stream.advance(2);
    }
    else {
        throw diagnostic(stream.peek(), diagnostic::reason::KDL024);
    }
}
