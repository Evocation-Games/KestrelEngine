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

#include <libKDL/sema/module/module.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/documentation/documentation.hpp>
#include <libKDL/modules/module/module_definition.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

auto kdl::sema::module_definition::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::documentation).optional(),
        expectation(tokenizer::module_keyword).be_true(),
        expectation(tokenizer::identifier).be_true()
    });
}

auto kdl::sema::module_definition::parse(foundation::stream<tokenizer::token> &stream, kdl::sema::context &ctx) -> void
{
    std::string documentation;
    if (stream.expect({ expectation(tokenizer::documentation).be_true() })) {
        documentation = stream.read().string_value();
    }

    stream.ensure({ expectation(tokenizer::module_keyword).be_true() });
    const auto module_name = stream.read();

    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    kdl::module_definition raw_module(module_name.string_value());
    auto module = const_cast<kdl::module_definition *>(ctx.register_module(raw_module));

    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (sema::decorator::test(stream)) {
            ctx.current_decorators = sema::decorator::parse(stream);
            continue;
        }
        else if (stream.expect({ expectation(tokenizer::import_keyword).be_true() })) {
            stream.advance();
            if (stream.expect({ expectation(tokenizer::string).be_true() })) {
                auto path = stream.read();
                module->add_import(path.string_value());
            }
            else if (stream.expect({ expectation(tokenizer::identifier).be_true() })) {
                auto name = stream.read();
                module->add_dependency(name.string_value());
            }
            else {
                throw diagnostic(stream.peek(), diagnostic::reason::KDL006);
            }
        }
        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}
