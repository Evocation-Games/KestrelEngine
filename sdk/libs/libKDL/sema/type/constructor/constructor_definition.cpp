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

#include <libKDL/sema/type/constructor/constructor_definition.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

auto kdl::sema::type_definition::constructor::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::documentation).optional(),
        expectation(tokenizer::constructor_keyword).be_true(),
        expectation(tokenizer::l_paren).be_true(),
    });
}

auto kdl::sema::type_definition::constructor::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> resource::definition::type::constructor
{
    std::string documentation;
    if (stream.expect({ expectation(tokenizer::documentation).be_true() })) {
        documentation = stream.read().string_value();
    }

    stream.ensure({ expectation(tokenizer::constructor_keyword).be_true(), expectation(tokenizer::l_paren).be_true() });

    // Get parameter names.
    std::vector<std::string> parameter_names;
    while (stream.expect({ expectation(tokenizer::r_paren).be_false() })) {
        if (stream.expect({ expectation(tokenizer::identifier).be_true() })) {
            parameter_names.emplace_back(stream.read().string_value());
        }

        if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
            stream.advance();
            continue;
        }
        else if (stream.expect({ expectation(tokenizer::r_paren).be_true() })) {
            break;
        }
        else {
            throw diagnostic(stream.peek(), diagnostic::reason::KDL006);
        }
    }

    stream.ensure({ expectation(tokenizer::r_paren).be_true() });

    // Read out the expressions/statements from the body of the constructor
    auto constructor = resource::definition::type::constructor(parameter_names);
    constructor.add_decorator(decorator::name::documentation, documentation);
    constructor.set_script(sema::script::parse(stream, ctx));
    return constructor;
}