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

#include <libKDL/parser/sema/directive/metadata_directive.hpp>
#include <libKDL/exception/unrecognised_module_format_exception.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>

// MARK: - Base

auto kdl::sema::directive::is_metadata(lexer::lexeme_stream &stream) -> bool
{
    return metadata<key::project>::accepts(stream)
        || metadata<key::author>::accepts(stream)
        || metadata<key::version>::accepts(stream)
        || metadata<key::copyright>::accepts(stream)
        || metadata<key::email>::accepts(stream)
        || metadata<key::format>::accepts(stream)
        || metadata<key::game>::accepts(stream);
}

auto kdl::sema::directive::parse(lexer::lexeme_stream &stream, session::session *session, session::interpreter::scope &scope) -> void
{
    if (metadata<key::project>::accepts(stream)) {
        metadata<key::project>::parse(stream, session, scope);
    }
    else if (metadata<key::author>::accepts(stream)) {
        metadata<key::author>::parse(stream, session, scope);
    }
    else if (metadata<key::version>::accepts(stream)) {
        metadata<key::version>::parse(stream, session, scope);
    }
    else if (metadata<key::copyright>::accepts(stream)) {
        metadata<key::copyright>::parse(stream, session, scope);
    }
    else if (metadata<key::email>::accepts(stream)) {
        metadata<key::email>::parse(stream, session, scope);
    }
    else if (metadata<key::format>::accepts(stream)) {
        metadata<key::format>::parse(stream, session, scope);
    }
    else if (metadata<key::game>::accepts(stream)) {
        metadata<key::game>::parse(stream, session, scope);
    }
}

// MARK: - Metadata

template<const char *K>
auto kdl::sema::directive::metadata<K>::accepts(lexer::lexeme_stream &stream) -> bool
{
    return stream.expect({ lexer::expectation(lexer::lexeme::directive, std::string(K)).be_true() });
}

template<const char *K>
auto kdl::sema::directive::metadata<K>::parse(lexer::lexeme_stream &stream, session::session *session, session::interpreter::scope &scope) -> void
{
    stream.ensure({ lexer::expectation(lexer::lexeme::directive, std::string(K)).be_true() });

    // Do we have a specialised handler for the metadata?
    if (K == key::format) {
        auto command = stream.read();
        auto format_name = stream.read();
        auto format = session->format();

        // Verify the format is recognised.
        if (format_name.is(lexer::lexeme::identifier, "extended") || format_name.is(lexer::lexeme::identifier, "kestrel")) {
            format = session::format::extended;
        }
        else if (format_name.is(lexer::lexeme::identifier, "classic") || format_name.is(lexer::lexeme::identifier, "rsrc")) {
            format = session::format::classic;
        }
        else if (format_name.is(lexer::lexeme::identifier, "rez")) {
            format = session::format::rez;
        }
        else {
            throw unrecognised_module_format_exception("Unrecognised module format specified.", format_name);
        }

        // Module Output Format
        if (command.is(lexer::lexeme::identifier, "requires")) {
            // Check the output format of the current module and verify that it matches?
            if (session->format() != format) {
                // Mismatch!
                throw unrecognised_module_format_exception("Current module format does not match the required module format.", format_name);
            }
        }
        else if (command.is(lexer::lexeme::identifier, "set")) {
            // Change the output format of the current module to the specified format.
        }
        else {
            throw unexpected_lexeme_exception("Unexpected format command specified.", command);
        }
    }
    else {
        // Read lexemes until the terminating ';'
        std::vector<lexer::lexeme> args;
        while (stream.expect({ lexer::expectation(lexer::lexeme::semi).be_false() })) {
            args.emplace_back(stream.read());
        }

        lexer::lexeme_stream argument_stream(args);
    }
}