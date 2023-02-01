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

#include <libKDL/sema/components/component.hpp>
#include <libKDL/sema/expectation/expectation.hpp>

auto kdl::sema::component::test(const foundation::stream<tokenizer::token>& stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::component_keyword).be_true(),
        expectation(tokenizer::l_angle).be_true()
    });
}

auto kdl::sema::component::parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void
{
    stream.ensure({ expectation(tokenizer::component_keyword).be_true(), expectation(tokenizer::l_angle).be_true() });

    if (!stream.expect_any({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::identifier_path).be_true() })) {
        throw std::runtime_error("");
    }
    auto component_type = stream.read();

    if (!stream.expect_any({ expectation(tokenizer::comma).be_true(), expectation(tokenizer::reference).be_true() })) {
        throw std::runtime_error("");
    }
    stream.advance();
    auto reference = stream.read();

    stream.ensure({ expectation(tokenizer::r_angle).be_true() });

    if (!stream.expect_any({ expectation(tokenizer::identifier).be_true() })) {
        throw std::runtime_error("");
    }
    auto component_name = stream.read();

    stream.ensure({ expectation(tokenizer::l_brace).be_true() });


    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}

