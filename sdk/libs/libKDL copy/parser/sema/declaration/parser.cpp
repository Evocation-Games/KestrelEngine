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

#include <libKDL/session/session.hpp>
#include <libKDL/session/resources/reference.hpp>
#include <libKDL/parser/sema/declaration/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/exception/unrecognised_type_definition_exception.hpp>
#include <libKDL/parser/sema/declaration/resource.hpp>
#include <libKDL/parser/sema/decorator/parser.hpp>

auto kdl::sema::declaration::parse(
    lexer::lexeme_stream &stream,
    session::session *session,
    session::interpreter::scope &scope
)
    -> void
{
    stream.ensure({ lexer::expectation(lexer::lexeme::identifier, "declare").be_true() });

    lexer::lexeme type_name("", lexer::lexeme::any);
    lexer::lexeme ns("", lexer::lexeme::any);

    if (stream.expect({
        lexer::expectation(lexer::lexeme::identifier).be_true(),
        lexer::expectation(lexer::lexeme::dot).be_true(),
        lexer::expectation(lexer::lexeme::identifier).be_true()
    })) {
        ns = stream.read(); stream.advance();
        type_name = stream.read();
    }
    else if (stream.expect({
        lexer::expectation(lexer::lexeme::identifier).be_true(),
        lexer::expectation(lexer::lexeme::dot).be_false()
    })) {
        type_name = stream.read();
    }
    else {
        throw unexpected_lexeme_exception("Resource declaration block must specify a type name.", stream.peek());
    }

    // Look up the type definition to be sure that it exists
    if (!session->has_type_definition(type_name.text())) {
        throw unrecognised_type_definition_exception("Attempted to declare resources using unknown type.", type_name);
    }
    auto& type = session->type_container(type_name.text(), ns.text());

    // Finally construct the resource instances themselves.
    std::vector<session::decorator::instance> decorators;
    stream.ensure({ lexer::expectation(lexer::lexeme::l_brace).be_true() });
    while (stream.expect({ lexer::expectation(lexer::lexeme::r_brace).be_false() })) {
        if (stream.expect({ lexer::expectation(lexer::lexeme::directive).be_true() })) {
            decorators.emplace_back(decorator::parse(stream, scope));
            continue;
        }
        else if (stream.expect_any({
            lexer::expectation(lexer::lexeme::identifier, "new").be_true(),
            lexer::expectation(lexer::lexeme::identifier, "override").be_true(),
            lexer::expectation(lexer::lexeme::identifier, "duplicate").be_true()
        })) {
            auto resource_scope = scope.shadow();
            session::decorator::collection collection(decorators);
            resource::parse(stream, session, resource_scope, type, collection);
            decorators.clear();
        }
        else {
            throw unexpected_lexeme_exception("Unexpected lexeme encountered in resource declaration block", stream.peek());
        }

        stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });
    }
    stream.ensure({ lexer::expectation(lexer::lexeme::r_brace).be_true() });
}
