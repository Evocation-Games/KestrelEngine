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

#include <iostream>
#include <libKDL/sema/directive/out.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

auto kdl::sema::directive::out::parse(foundation::stream<tokenizer::token> &stream) -> void
{
    stream.ensure({ expectation(tokenizer::out_directive).be_true() });

    if (!stream.expect({ expectation(tokenizer::string).be_true() })) {
        throw diagnostic(stream.peek(), diagnostic::reason::KDL026);
    }

    auto str = stream.read();
    std::cout << str.string_value() << std::endl;
}
