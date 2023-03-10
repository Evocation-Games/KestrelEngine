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

#include <libKDL/sema/decorator/decorator.hpp>
#include <libKDL/sema/expectation/expectation.hpp>

auto kdl::sema::decorator::test(foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect_any({
        expectation(tokenizer::builtin_decorator).be_true(),
        expectation(tokenizer::no_declaration_decorator).be_true(),
        expectation(tokenizer::synthesize_decorator).be_true(),
        expectation(tokenizer::constexpression_decorator).be_true(),
        expectation(tokenizer::api_decorator).be_true(),
        expectation(tokenizer::example_decorator).be_true(),
        expectation(tokenizer::deprecated_decorator).be_true(),
        expectation(tokenizer::condition_decorator).be_true(),
        expectation(tokenizer::decorator).be_true()
    });
}

auto kdl::sema::decorator::parse(foundation::stream<tokenizer::token> &stream) -> collection
{
    collection collection;

    // We need to keep consuming decorators and build up the collection for use with
    // the next construct.
    while (stream.expect_any({
        expectation(tokenizer::builtin_decorator).be_true(),
        expectation(tokenizer::no_declaration_decorator).be_true(),
        expectation(tokenizer::synthesize_decorator).be_true(),
        expectation(tokenizer::constexpression_decorator).be_true(),
        expectation(tokenizer::api_decorator).be_true(),
        expectation(tokenizer::example_decorator).be_true(),
        expectation(tokenizer::deprecated_decorator).be_true(),
        expectation(tokenizer::condition_decorator).be_true(),
        expectation(tokenizer::decorator).be_true()
    })) {
        auto token = stream.read();
        collection.decorators.emplace_back(token.source().text(), token.associated_values());
    }
    return collection;
}

// MARK: - Decorators

auto kdl::sema::decorator::collection::has_decorator(const std::string &name, const std::vector<std::string> &hints) const -> bool
{
    for (const auto& d : decorators) {
        if (d.name() == name) {
            if (d.has_associated_values()) {
                if (d.associated_value_count() == hints.size()) {
                    for (auto i = 0; i < hints.size(); ++i) {
                        if (hints[i] != d.associated_value_at(i)) {
                            return false;
                        }
                    }
                    return true;
                }
            }
            else {
                return true;
            }
        }
    }
    return false;
}