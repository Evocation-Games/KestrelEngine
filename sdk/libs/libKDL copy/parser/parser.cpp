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

#include <libKDL/parser/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/parser/sema/type_definition/parser.hpp>
#include <libKDL/parser/sema/directive/metadata_directive.hpp>
#include <libKDL/parser/sema/directive/command_directive.hpp>
#include <libKDL/parser/sema/directive/import_directive.hpp>
#include <libKDL/parser/sema/decorator/parser.hpp>
#include <libKDL/parser/sema/declaration/parser.hpp>
#include <libKDL/session/decorators/decorator.hpp>
#include <libLexer/expect/expectation.hpp>


// MARK: - Base Parsing

auto kdl::parse_file_stream(
    foundation::stream<lexer::lexeme>& stream,
    session::session *session,
    session::interpreter::scope& scope
)
    -> void
{
    std::vector<session::decorator::instance> decorators;

    while (!stream.finished()) {
        // Look for decorators and collect them into a vector until the next construct.
        // Each decorator candidate might also be a directive, so handle those first.
        if (stream.expect({ lexer::expectation(lexer::lexeme::directive).be_true() })){
            // Check for a metadata directive.
            if (sema::directive::is_metadata(stream)) {
                sema::directive::parse(stream, session, scope);
            }
            else if (sema::directive::is_command(stream)) {
                sema::directive::run(stream, session, scope);
            }
            else if (sema::directive::is_import(stream)) {
                sema::directive::import_file(stream, session, scope);
            }
            else {
                decorators.emplace_back(sema::decorator::parse(stream, scope));
                continue;
            }
        }

        // Type Definitions
        else if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "type").be_true() })) {
            auto definition_scope = scope.shadow();
            auto definition = sema::type_definition::parse(stream, session, definition_scope);
            definition.set_decorators(decorators);
            session->add_type_definition(definition);
            decorators.clear();
        }

        // Resource Declarations
        else if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "declare").be_true() })) {
            auto declaration_scope = scope.shadow();
            sema::declaration::parse(stream, session, declaration_scope);
            decorators.clear();
        }

        // Project Components
        else if (stream.expect_any({ lexer::expectation(lexer::lexeme::identifier, "component").be_true() })) {

        }

        // Unknowns
        else {
            throw unexpected_lexeme_exception("Unexpected lexeme encountered", stream.peek());
        }

        stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });
    }
}