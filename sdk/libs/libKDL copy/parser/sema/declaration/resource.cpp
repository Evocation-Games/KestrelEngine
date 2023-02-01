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

#include <libKDL/parser/sema/declaration/resource.hpp>
#include <libKDL/parser/sema/list/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/exception/resource_already_exists_exception.hpp>
#include <libKDL/session/resources/reference.hpp>
#include <libKDL/parser/sema/declaration/field.hpp>

auto kdl::sema::declaration::resource::parse(
    lexer::lexeme_stream &stream,
    session::session *session,
    session::interpreter::scope &scope,
    session::resources::type_container& type,
    session::decorator::collection& decorators
)
    -> void
{
    if (!stream.expect_any({
        lexer::expectation(lexer::lexeme::identifier, "new").be_true(),
        lexer::expectation(lexer::lexeme::identifier, "override").be_true(),
        lexer::expectation(lexer::lexeme::identifier, "duplicate").be_true(),
    })) {
        throw unexpected_lexeme_exception("Resource declaration must begin with either 'new', 'override' or 'duplicate'.", stream.peek());
    }
    const auto& keyword = stream.read();

    // Resource Metadata
    session::resources::reference source_id;
    session::resources::reference id;
    std::string name;

    // Establish a source id that can be used for duplication if required.
    if (keyword.is("duplicate") && stream.expect({ lexer::expectation(lexer::lexeme::l_paren).be_true() })) {

    }
    else if (stream.expect({ lexer::expectation(lexer::lexeme::l_paren).be_true() })) {
        list::parser list(stream);

        list.set_list_start(lexer::lexeme::l_paren);
        list.set_list_end(lexer::lexeme::r_paren);
        list.set_delimiter(lexer::lexeme::comma);
        list.add_valid_list_item(lexer::lexeme::reference);
        list.add_valid_list_item(lexer::lexeme::string);

        for (const auto& arg : list.parse(scope)) {
            if (arg.is(lexer::lexeme::reference)) {
                id = session::resources::reference(arg, session);
            }
            else if (arg.is(lexer::lexeme::string)) {
                name = arg.text();
            }
        }

        // 'new' doesn't care about 'source_id', and 'override' requires it to be equal to 'id'
        source_id = id;
    }

    // Make sure we are working with a valid resource id, this means promoting the "auto_id" or '#auto' to
    // and actual unique id.
    if (id.is_auto()) {
        id = id.with(type.next_id());
    }

    // Request a new blank resource instance that we can prepare to populate.
    auto instance = type.construct(id, name, scope);
    auto instance_scope = scope.shadow();
    instance_scope.add_variable("id", id.lexeme());
    instance_scope.add_variable("name", name, lexer::lexeme::string);

    // Is this resource that is overriding another? If it is then we need to pre-populate the resource with the data
    // of the original (if it exists).
    if (keyword.is("override") || keyword.is("duplicate")) {
        // Check if the existing resource exists...
        if (type.instance_exists(id)) {
            throw resource_already_exists_exception("Attempting to " + keyword.text() + " resource that does not exist.", id);
        }

        // TODO: populate from existing resource
    }
    else {
        // Apply defaults to the instance from the type definition.
        // TODO: Work this into a better solution than requiring a const_cast.
        for (auto& field : const_cast<std::vector<session::type_field>&>(type.definition()->all_fields())) {
            field::apply_defaults(instance, field, stream, instance_scope, session);
        }
    }
}
