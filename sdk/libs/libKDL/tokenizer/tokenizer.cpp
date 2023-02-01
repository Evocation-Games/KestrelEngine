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

#include <libKDL/tokenizer/tokenizer.hpp>
#include <libLexer/expect/expectation.hpp>
#include <libKDL/spec/directives.hpp>
#include <libKDL/spec/keywords.hpp>
#include <libKDL/spec/binary_types.hpp>
#include <libKDL/spec/types.hpp>
#include <libKDL/spec/decorators.hpp>
#include <libKDL/tokenizer/token.hpp>
#include <libResource/reference.hpp>

// MARK: - Construction

kdl::tokenizer::tokenizer::tokenizer(const foundation::stream<lexer::lexeme> &input)
    : m_input(input)
{}

// MARK: - Processing

auto kdl::tokenizer::tokenizer::process() -> foundation::stream<token>
{
    foundation::stream<token> output;

    // Consume the stream of lexemes from the lexer and convert them into tokens.
    // Each token may represent more than one lexeme.
    while (!m_input.finished()) {

        // Constructs --------------------------------------------------------------------------------------------------
        if (m_input.expect({
            lexer::expectation(lexer::at).be_true(),
            lexer::expectation(lexer::identifier, spec::directives::vector()).be_true()
        })) {
            m_input.advance();
            output.append(directive_named(m_input.read()));
        }
        else if (m_input.expect({ lexer::expectation(lexer::identifier, spec::keywords::vector()).be_true() })) {
            output.append(keyword_named(m_input.read()));
        }
        else if (m_input.expect({
            lexer::expectation(lexer::at).be_true(), lexer::expectation(lexer::identifier).be_true()
        })) {
            m_input.advance();
            auto decorator_name = m_input.read();

            // Check for arguments attached to the decorator
            std::vector<lexer::lexeme> arguments;
            if (m_input.expect({ lexer::expectation(lexer::l_paren).be_true() })) {
                m_input.advance();
                while (m_input.expect({ lexer::expectation(lexer::r_paren).be_false() })) {
                    if (m_input.expect_any({
                        lexer::expectation(lexer::identifier).be_true(),
                        lexer::expectation(lexer::string).be_true(),
                        lexer::expectation(lexer::integer).be_true(),
                    })) {
                        arguments.emplace_back(m_input.read());
                    }
                    else {
                        throw std::runtime_error("");
                    }

                    if (m_input.expect({ lexer::expectation(lexer::comma).be_true() })) {
                        m_input.advance();
                        continue;
                    }
                    else if (m_input.expect({ lexer::expectation(lexer::r_paren).be_true() })) {
                        break;
                    }
                    else {
                        throw std::runtime_error("");
                    }
                }
                m_input.advance();
            }

            output.append(token(decorator_name, arguments, decorator_named(decorator_name)));
        }

        // Types -------------------------------------------------------------------------------------------------------
        else if (m_input.expect({ lexer::expectation(lexer::identifier, spec::binary_types::vector()).be_true() })) {
            auto binary_type = m_input.read();
            if (binary_type.is(spec::binary_types::NESTED)) {
                if (m_input.expect({
                    lexer::expectation(lexer::l_angle).be_true(),
                    lexer::expectation(lexer::identifier).be_true(),
                    lexer::expectation(lexer::r_angle).be_true()
                })) {
                    m_input.advance();
                    auto nested_type = m_input.read(); m_input.advance();
                    output.append(token(nested_type, token_type::NESTED));
                }
                else {
                    throw std::runtime_error("");
                }
            }
            else {
                output.append(binary_type_named(binary_type));
            };
        }
        else if (m_input.expect({ lexer::expectation(lexer::identifier, spec::types::vector()).be_true() })) {
            auto type = m_input.read();
            output.append(type_named(type));
        }
        else if (m_input.expect({ lexer::expectation(lexer::identifier).be_true(), lexer::expectation(lexer::amp).be_true() })) {
            auto reference_type_name = m_input.read(); m_input.advance();
            output.append(token(reference_type_name, token_type::named_reference_type));
        }

        // Identifiers -------------------------------------------------------------------------------------------------
        else if (m_input.expect({ lexer::expectation(lexer::identifier).be_true(), lexer::expectation(lexer::dot).be_true() })) {
            std::vector<lexer::lexeme> path;
            path.emplace_back(m_input.read());
            while (m_input.expect({ lexer::expectation(lexer::dot).be_true(), lexer::expectation(lexer::identifier).be_true() })) {
                m_input.advance();
                path.emplace_back(m_input.read());
            }
            output.append(token(path.back(), path, token_type::identifier_path));
        }
        else if (m_input.expect({ lexer::expectation(lexer::identifier).be_true() })) {
            output.append(token(m_input.read(), token_type::identifier));
        }

        // Literals ----------------------------------------------------------------------------------------------------
        else if (m_input.expect({ lexer::expectation(lexer::string).be_true() })) {
            output.append(token(m_input.read(), token_type::string));
        }
        else if (m_input.expect({ lexer::expectation(lexer::integer).be_true(), lexer::expectation(lexer::percent).be_true() })) {
            output.append(token(m_input.read(), token_type::percentage));
        }
        else if (m_input.expect({ lexer::expectation(lexer::integer).be_true() })) {
            output.append(token(m_input.read(), token_type::integer));
        }
        else if (m_input.expect({ lexer::expectation(lexer::hash).be_true() })) {
            auto hash = m_input.read();

            if (m_input.expect({
                lexer::expectation(lexer::identifier).be_true(),
                lexer::expectation(lexer::dot).be_true(),
                lexer::expectation(lexer::integer).be_true()
            })) {
                // Reference : #Type.id
                auto type_name = m_input.read(); m_input.advance();
                auto id = m_input.read();
                output.append(token(hash, resource::reference(
                    id.value<resource::reference::identifier>(), type_name.text()
                )));
            }
            else if (m_input.expect({
                lexer::expectation(lexer::identifier).be_true(),
                lexer::expectation(lexer::dot).be_true(),
                lexer::expectation(lexer::identifier).be_true(),
                lexer::expectation(lexer::dot).be_true(),
                lexer::expectation(lexer::integer).be_true()
            })) {
                // Reference : #Container.Type.id
                auto container = m_input.read(); m_input.advance();
                auto type_name = m_input.read(); m_input.advance();
                auto id = m_input.read();
                output.append(token(hash, resource::reference(
                    id.value<resource::reference::identifier>(), type_name.text(), container.text()
                )));
            }
            else if (m_input.expect({
                lexer::expectation(lexer::integer).be_true()
            })) {
                // Reference : #id
                auto id = m_input.read();
                output.append(token(hash, resource::reference(
                    id.value<resource::reference::identifier>()
                )));
            }
            else if (m_input.expect({
                lexer::expectation(lexer::identifier, "auto").be_true()
            })) {
                // Reference : #auto
                m_input.advance();
                output.append(token(hash, resource::reference::auto_assigning()));
            }
            else if (m_input.expect({
                lexer::expectation(lexer::question).be_true()
            })) {
                // Reference #? (any)
                output.append(token(hash, resource::reference::any()));
            }
            else {
                output.append({ hash, token_type::hash });
            }
        }

        // Operators / Symbols -----------------------------------------------------------------------------------------
        else if (m_input.expect({ lexer::expectation(lexer::l_paren).be_true() })) {
            m_input.advance();
            output.append(token_type::l_paren);
        }
        else if (m_input.expect({ lexer::expectation(lexer::r_paren).be_true() })) {
            m_input.advance();
            output.append(token_type::r_paren);
        }
        else if (m_input.expect({ lexer::expectation(lexer::l_angle).be_true() })) {
            m_input.advance();
            output.append(token_type::l_angle);
        }
        else if (m_input.expect({ lexer::expectation(lexer::r_angle).be_true() })) {
            m_input.advance();
            output.append(token_type::r_angle);
        }
        else if (m_input.expect({ lexer::expectation(lexer::l_brace).be_true() })) {
            m_input.advance();
            output.append(token_type::l_brace);
        }
        else if (m_input.expect({ lexer::expectation(lexer::r_brace).be_true() })) {
            m_input.advance();
            output.append(token_type::r_brace);
        }
        else if (m_input.expect({ lexer::expectation(lexer::l_bracket).be_true() })) {
            m_input.advance();
            output.append(token_type::l_bracket);
        }
        else if (m_input.expect({ lexer::expectation(lexer::r_bracket).be_true() })) {
            m_input.advance();
            output.append(token_type::r_bracket);
        }
        else if (m_input.expect({ lexer::expectation(lexer::comma).be_true() })) {
            m_input.advance();
            output.append(token_type::comma);
        }
        else if (m_input.expect({ lexer::expectation(lexer::dot).be_true() })) {
            m_input.advance();
            output.append(token_type::dot);
        }
        else if (m_input.expect({ lexer::expectation(lexer::pipe).be_true() })) {
            m_input.advance();
            output.append(token_type::pipe);
        }
        else if (m_input.expect({ lexer::expectation(lexer::slash).be_true() })) {
            m_input.advance();
            output.append(token_type::slash);
        }
        else if (m_input.expect({ lexer::expectation(lexer::amp).be_true() })) {
            m_input.advance();
            output.append(token_type::amp);
        }
        else if (m_input.expect({ lexer::expectation(lexer::colon).be_true() })) {
            m_input.advance();
            output.append(token_type::colon);
        }
        else if (m_input.expect({ lexer::expectation(lexer::semi).be_true() })) {
            m_input.advance();
            output.append(token_type::semi);
        }
        else if (m_input.expect({ lexer::expectation(lexer::plus).be_true() })) {
            m_input.advance();
            output.append(token_type::plus);
        }
        else if (m_input.expect({ lexer::expectation(lexer::minus).be_true() })) {
            m_input.advance();
            output.append(token_type::minus);
        }
        else if (m_input.expect({ lexer::expectation(lexer::star).be_true() })) {
            m_input.advance();
            output.append(token_type::star);
        }
        else if (m_input.expect({ lexer::expectation(lexer::equals).be_true() })) {
            m_input.advance();
            output.append(token_type::equals);
        }
        else if (m_input.expect({ lexer::expectation(lexer::carat).be_true() })) {
            m_input.advance();
            output.append(token_type::carat);
        }
        else if (m_input.expect({ lexer::expectation(lexer::left_shift).be_true() })) {
            m_input.advance();
            output.append(token_type::left_shift);
        }
        else if (m_input.expect({ lexer::expectation(lexer::right_shift).be_true() })) {
            m_input.advance();
            output.append(token_type::right_shift);
        }
        else if (m_input.expect({ lexer::expectation(lexer::tilde).be_true() })) {
            m_input.advance();
            output.append(token_type::tilde);
        }
        else if (m_input.expect({ lexer::expectation(lexer::at).be_true() })) {
            m_input.advance();
            output.append(token_type::at);
        }
        else if (m_input.expect({ lexer::expectation(lexer::hash).be_true() })) {
            m_input.advance();
            output.append(token_type::hash);
        }
        else if (m_input.expect({ lexer::expectation(lexer::percent).be_true() })) {
            m_input.advance();
            output.append(token_type::percent);
        }
        else if (m_input.expect({ lexer::expectation(lexer::exclaim).be_true() })) {
            m_input.advance();
            output.append(token_type::exclaim);
        }
        else if (m_input.expect({ lexer::expectation(lexer::question).be_true() })) {
            m_input.advance();
            output.append(token_type::question);
        }
        else if (m_input.expect({ lexer::expectation(lexer::dollar).be_true() })) {
            m_input.advance();
            output.append(token_type::dollar);
        }

        // Unknown type...
        else {
            m_input.advance();
            output.append(token_type::error);
        }
    }

    return output;
}

// MARK: - Validators

auto kdl::tokenizer::tokenizer::directive_named(const lexer::lexeme &name) -> token_type
{
    if (name.is(spec::directives::project_directive))           return token_type::project_directive;
    else if (name.is(spec::directives::author_directive))       return token_type::author_directive;
    else if (name.is(spec::directives::version_directive))      return token_type::version_directive;
    else if (name.is(spec::directives::license_directive))      return token_type::license_directive;
    else if (name.is(spec::directives::copyright_directive))    return token_type::copyright_directive;
    else if (name.is(spec::directives::email_directive))        return token_type::email_directive;
    else if (name.is(spec::directives::website_directive))      return token_type::website_directive;
    else if (name.is(spec::directives::out_directive))          return token_type::out_directive;
    else if (name.is(spec::directives::format_directive))       return token_type::format_directive;
    else if (name.is(spec::directives::import_directive))       return token_type::import_directive;
    throw std::runtime_error("");
}

auto kdl::tokenizer::tokenizer::keyword_named(const lexer::lexeme &name) -> token_type
{
    if (name.is(spec::keywords::type_keyword))              return token_type::type_keyword;
    else if (name.is(spec::keywords::declare_keyword))      return token_type::declare_keyword;
    else if (name.is(spec::keywords::new_keyword))          return token_type::new_keyword;
    else if (name.is(spec::keywords::override_keyword))     return token_type::override_keyword;
    else if (name.is(spec::keywords::duplicate_keyword))    return token_type::duplicate_keyword;
    else if (name.is(spec::keywords::field_keyword))        return token_type::field_keyword;
    else if (name.is(spec::keywords::template_keyword))     return token_type::template_keyword;
    else if (name.is(spec::keywords::assert_keyword))       return token_type::assert_keyword;
    else if (name.is(spec::keywords::component_keyword))    return token_type::component_keyword;
    else if (name.is(spec::keywords::files_keyword))        return token_type::files_keyword;
    else if (name.is(spec::keywords::types_keyword))        return token_type::types_keyword;
    else if (name.is(spec::keywords::repeatable_keyword))   return token_type::repeatable_keyword;
    else if (name.is(spec::keywords::as_keyword))           return token_type::as_keyword;
    else if (name.is(spec::keywords::constructor_keyword))  return token_type::constructor_keyword;
    throw std::runtime_error("");
}

auto kdl::tokenizer::tokenizer::binary_type_named(const lexer::lexeme &name) -> token_type
{
    if (name.is(spec::binary_types::DBYT))                  return token_type::DBYT;
    else if (name.is(spec::binary_types::DWRD))             return token_type::DWRD;
    else if (name.is(spec::binary_types::DLNG))             return token_type::DLNG;
    else if (name.is(spec::binary_types::DQWD))             return token_type::DQWD;
    else if (name.is(spec::binary_types::HBYT))             return token_type::HBYT;
    else if (name.is(spec::binary_types::HWRD))             return token_type::HWRD;
    else if (name.is(spec::binary_types::HLNG))             return token_type::HLNG;
    else if (name.is(spec::binary_types::HQWD))             return token_type::HQWD;
    else if (name.is(spec::binary_types::RECT))             return token_type::RECT;
    else if (name.is(spec::binary_types::PSTR))             return token_type::PSTR;
    else if (name.is(spec::binary_types::CSTR))             return token_type::CSTR;
    else if (name.is(spec::binary_types::Cnnn))             return token_type::Cnnn;
    else if (name.is(spec::binary_types::LSTR))             return token_type::LSTR;
    else if (name.is(spec::binary_types::OSTR))             return token_type::OSTR;
    else if (name.is(spec::binary_types::CHAR))             return token_type::CHAR;
    else if (name.is(spec::binary_types::OCNT))             return token_type::OCNT;
    else if (name.is(spec::binary_types::LSTC))             return token_type::LSTC;
    else if (name.is(spec::binary_types::LSTE))             return token_type::LSTE;
    else if (name.is(spec::binary_types::BBIT))             return token_type::BBIT;
    else if (name.is(spec::binary_types::BOOL))             return token_type::BOOL;
    else if (name.is(spec::binary_types::RSRC))             return token_type::RSRC;
    else if (name.is(spec::binary_types::HEXD))             return token_type::HEXD;
    else if (name.is(spec::binary_types::NESTED))           return token_type::NESTED;
    throw std::runtime_error("");
}

auto kdl::tokenizer::tokenizer::type_named(const lexer::lexeme &name) -> token_type
{
    if (name.is(spec::types::integer))                      return token_type::integer_type;
    else if (name.is(spec::types::string))                  return token_type::string_type;
    else if (name.is(spec::types::bitmask))                 return token_type::bitmask_type;
    else if (name.is(spec::types::file))                    return token_type::file_type;
    else if (name.is(spec::types::image))                   return token_type::image_type;
    throw std::runtime_error("");
}

auto kdl::tokenizer::tokenizer::decorator_named(const lexer::lexeme &name) -> token_type
{
    if (name.is(spec::decorators::builtin))                 return token_type::builtin_decorator;
    else if (name.is(spec::decorators::no_declaration))     return token_type::no_declaration_decorator;
    else if (name.is(spec::decorators::synthesize))         return token_type::synthesize_decorator;
    else if (name.is(spec::decorators::constexpression))    return token_type::constexpression_decorator;
    else if (name.is(spec::decorators::api))                return token_type::api_decorator;
    else if (name.is(spec::decorators::example))            return token_type::example_decorator;
    else if (name.is(spec::decorators::deprecated))         return token_type::deprecated_decorator;
    else if (name.is(spec::decorators::condition))          return token_type::condition_decorator;
    else                                                    return token_type::decorator;
}