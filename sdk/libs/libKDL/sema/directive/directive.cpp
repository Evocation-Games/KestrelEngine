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
#include <libKDL/unit/file.hpp>
#include <libKDL/exception/invalid_attribute_exception.hpp>

#include <libKDL/sema/directive/out.hpp>
#include <libKDL/sema/directive/metadata.hpp>
#include <libKDL/sema/directive/format.hpp>
#include <libKDL/sema/directive/variable.hpp>
#include <libKDL/sema/directive/function.hpp>

#include <libKDL/modules/kestrel/kestrel.hpp>
#include <libKDL/modules/macintosh/macintosh.hpp>
#include <libKDL/modules/spriteworld/spriteworld.hpp>

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
        expectation(tokenizer::format_directive).be_true(),
        expectation(tokenizer::import_directive).be_true(),
        expectation(tokenizer::variable_directive).be_true(),
        expectation(tokenizer::constant_directive).be_true(),
        expectation(tokenizer::function_directive).be_true()
    });
}

auto kdl::sema::directive::parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void
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
    else if (stream.expect_any({ expectation(tokenizer::variable_directive).be_true(), expectation(tokenizer::constant_directive).be_true() })) {
        variable::parse(stream, ctx);
    }
    else if (stream.expect({ expectation(tokenizer::function_directive).be_true() })) {
        function::parse(stream, ctx);
    }
    else if (stream.expect({ expectation(tokenizer::import_directive).be_true(), expectation(tokenizer::identifier).be_true() })) {
        // Importing a module.
        stream.advance();
        auto name_tk = stream.read();
        auto name = name_tk.string_value();
        if (name == "Kestrel") {
            modules::kestrel::construct(ctx);
        }
        else if (name == "Macintosh") {
            modules::macintosh::construct(ctx);
        }
        else if (name == "SpriteWorld") {
            modules::spriteworld::construct(ctx);
        }
        else if (auto module = ctx.module_named(name)) {
            for (const auto& dependency : module->dependencies()) {
                if (dependency == "Kestrel") {
                    modules::kestrel::construct(ctx);
                }
                else if (dependency == "Macintosh") {
                    modules::macintosh::construct(ctx);
                }
                else if (dependency == "SpriteWorld") {
                    modules::spriteworld::construct(ctx);
                }
            }

            for (const auto& raw_path : module->imports()) {
                auto path = ctx.resolve_path(raw_path, name_tk.source().source_directory());
                auto lexical_result = unit::file::import_and_tokenize_file(path.string(), std::vector<std::string>(
                    ctx.definitions.begin(), ctx.definitions.end()
                ), ctx);
                stream.insert(lexical_result);
                stream.push(tokenizer::token(tokenizer::semi));
            }
        }
    }
    else if (stream.expect({ expectation(tokenizer::import_directive).be_true(), expectation(tokenizer::string).be_true() })) {
        // Import a script.
        stream.advance();
        auto raw_path = stream.read();
        auto path = ctx.resolve_path(raw_path.string_value(), raw_path.source().source_directory());

        auto lexical_result = unit::file::import_and_tokenize_file(path.string(), std::vector<std::string>(
            ctx.definitions.begin(), ctx.definitions.end()
        ), ctx);
        stream.insert(lexical_result);
        stream.push(tokenizer::token(tokenizer::semi));
    }
    else {
        throw invalid_attribute_exception("Unrecognised attribute '" + stream.peek().string_value() + "'.", stream.peek().source());
    }
}
