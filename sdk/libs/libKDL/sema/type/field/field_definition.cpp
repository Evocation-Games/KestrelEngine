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

#include <stdexcept>
#include <libKDL/sema/type/field/field_definition.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/type/descriptor.hpp>
#include <libKDL/sema/type/field/symbol_list.hpp>
#include <libKDL/spec/decorators.hpp>
#include <libKDL/spec/types.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libKDL/exception/invalid_attribute_exception.hpp>
#include <libKDL/exception/unexpected_token_exception.hpp>
#include <libKDL/exception/unrecognised_variable_exception.hpp>
#include <libKDL/exception/unrecognised_binary_template_field_exception.hpp>

auto kdl::sema::type_definition::field_definition::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::documentation).optional(),
        expectation(tokenizer::field_keyword).be_true(),
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::string).be_true(),
        expectation(tokenizer::r_paren).be_true()
    });
}

auto kdl::sema::type_definition::field_definition::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> resource::definition::type::field
{
    std::string documentation;
    if (stream.expect({ expectation(tokenizer::documentation).be_true() })) {
        documentation = stream.read().string_value();
    }

    stream.ensure({
        expectation(tokenizer::field_keyword).be_true(),
        expectation(tokenizer::l_paren).be_true(),
    });
    auto field_keyword = stream.read(-2);
    auto field_name = stream.read();
    resource::definition::type::field field(field_name.string_value());
    field.add_decorators(ctx.current_decorators.decorators);
    field.add_decorator(decorator::name::documentation, documentation);
    ctx.current_decorators.decorators.clear();

    stream.ensure({ expectation(tokenizer::r_paren).be_true() });

    // Repeatable?
    if (stream.expect({
        expectation(tokenizer::repeatable_keyword).be_true(),
        expectation(tokenizer::l_angle).be_true()
    })) {
        stream.advance(2);

        if (stream.expect({
            expectation(tokenizer::integer).be_true(), expectation(tokenizer::comma).be_true(),
            expectation(tokenizer::integer).be_true(), expectation(tokenizer::comma).be_true(),
            expectation(tokenizer::identifier).be_true()
        })) {
            auto lower = stream.read(); stream.advance();
            auto upper = stream.read(); stream.advance();
            auto field_list = stream.read();
            auto tmpl = ctx.current_type->binary_template();
            field.make_repeatable(lower.value<std::int32_t>(), upper.value<std::int32_t>());
            field.repeatable().set_count_field(&tmpl->field_named(field_list.string_value()));
        }
        else if (stream.expect({
            expectation(tokenizer::identifier).be_true(), expectation(tokenizer::comma).be_true(),
            expectation(tokenizer::identifier).be_true(), expectation(tokenizer::comma).be_true(),
            expectation(tokenizer::identifier).be_true()
        })) {
            auto lower = stream.read(); stream.advance();
            auto upper = stream.read(); stream.advance();
            auto field_list = stream.read();
            auto& bin_field = ctx.current_type->binary_template()->field_named(field_list.string_value());

            if (!ctx.active_scope()->has_variable(lower.string_value())) {
                throw unrecognised_variable_exception("Unrecognised variable found.", lower.source());
            }

            if (!ctx.active_scope()->has_variable(upper.string_value())) {
                throw unrecognised_variable_exception("Unrecognised variable found.", upper.source());
            }

            auto lower_var = ctx.active_scope()->variable(lower.string_value());
            auto upper_var = ctx.active_scope()->variable(upper.string_value());

            field.make_repeatable(lower_var.integer_value(), upper_var.integer_value());
            field.repeatable().set_count_field(&bin_field);
        }
        else if (stream.expect({
            expectation(tokenizer::integer).be_true(), expectation(tokenizer::comma).be_true(),
            expectation(tokenizer::integer).be_true()
        })) {
            auto lower = stream.read(); stream.advance();
            auto upper = stream.read();
            field.make_repeatable(lower.value<std::int32_t>(), upper.value<std::int32_t>());
        }
        else if (stream.expect({
            expectation(tokenizer::identifier).be_true(), expectation(tokenizer::comma).be_true(),
            expectation(tokenizer::identifier).be_true()
        })) {
            auto lower = stream.read(); stream.advance();
            auto upper = stream.read();

            if (!ctx.active_scope()->has_variable(lower.string_value())) {
                throw unrecognised_variable_exception("Unrecognised variable found.", lower.source());
            }

            if (!ctx.active_scope()->has_variable(upper.string_value())) {
                throw unrecognised_variable_exception("Unrecognised variable found.", upper.source());
            }

            auto lower_var = ctx.active_scope()->variable(lower.string_value());
            auto upper_var = ctx.active_scope()->variable(upper.string_value());

            field.make_repeatable(lower_var.integer_value(), upper_var.integer_value());
        }
        else {
            throw std::runtime_error("");
        }

        stream.ensure({ expectation(tokenizer::r_angle).be_true() });
    }

    // Body
    if (!field.has_decorator(spec::decorators::synthesize)) {
        stream.ensure({ expectation(tokenizer::l_brace).be_true() });
        while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
            if (sema::decorator::test(stream)) {
                ctx.current_decorators = sema::decorator::parse(stream);
                continue;
            }
            else if (stream.expect_any({
                expectation(tokenizer::identifier).be_true(),
                expectation(tokenizer::identifier_path).be_true()
            })) {
                std::string prefix;
                if (field.repeatable().enabled() && field.repeatable().has_count_field()) {
                    prefix = field.repeatable().count_field()->label();
                }

                ctx.current_field = &field;
                auto decorators = ctx.current_decorators;
                auto value = parse_value(stream, ctx, prefix);
                ctx.current_decorators = decorators;
                value.add_decorators(ctx.current_decorators.decorators);
                ctx.current_decorators.decorators.clear();

                // Check if we need to merge bitmasks.
                if (field.has_decorator(spec::decorators::merge_bitmask)) {
                    if (value.type().name() != spec::types::bitmask) {
                        throw invalid_attribute_exception("The 'MergeBitmask' attribute requires that field only contains 'Bitmask' values.", field_name.source());
                    }

                    if (field.value_count() == 0) {
                        field.add_value(value);
                    }
                    else {
                        const_cast<resource::definition::type::field_value&>(field.value_at(0)).add_joined_value(value);
                    }
                }
                else {
                    field.add_value(value);
                }
            }
            else {
                throw unexpected_token_exception("Expected either 'Attribute' or value 'Identifier' with field body.", stream.peek());
            }

            stream.ensure({ expectation(tokenizer::semi).be_true() });
        }
        stream.ensure({ expectation(tokenizer::r_brace).be_true() });
    }
    else {
        // Synthesize the field - find the matching binary field.
        if (ctx.current_type->binary_template()->has_field_named(field.name())) {
            ctx.current_binary_field = &ctx.current_type->binary_template()->field_named(field.name());
            resource::definition::type::field_value value(ctx.current_binary_field);
            value.set_type(descriptor::infer_type(ctx), false);

            // The field was synthesized, so determine if a default value has been provided.
            if (stream.expect({ expectation(tokenizer::equals).be_true() })) {
                stream.advance();
                auto default_value = script::parse_statement(stream, ctx);
                value.set_default_value(default_value);
            }

            field.add_value(value);
        }
        else {
            throw unrecognised_binary_template_field_exception("Attempted to synthesize field with unknown template field.", field_name.source());
        }
    }

    return field;
}

auto kdl::sema::type_definition::field_definition::parse_value(foundation::stream<tokenizer::token> &stream, context &ctx, const std::string& prefix) -> resource::definition::type::field_value
{
    std::string documentation;
    if (stream.expect({ expectation(tokenizer::documentation).be_true() })) {
        documentation = stream.read().string_value();
    }

    const auto value_identifier = stream.read();
    resource::definition::type::descriptor explicit_type(false);

    std::vector<std::string> name_extensions;
    if (stream.expect({ expectation(tokenizer::l_angle).be_true() })) {
        // We have name extensions
        stream.advance();
        while (stream.expect({ expectation(tokenizer::r_angle).be_false() })) {
            if (stream.expect({ expectation(tokenizer::variable).be_true() })) {
                name_extensions.emplace_back(stream.read().string_value());
            }
            else {
                throw unexpected_token_exception("'Variable' expected as name extension.");
            }

            if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                stream.advance();
                continue;
            }
            break;
        }
        stream.ensure({ expectation(tokenizer::r_angle).be_true() });
    }

    auto binary_field_name = value_identifier.path_value(".");
    if (const_cast<resource::definition::type::field *>(ctx.current_field)->repeatable().enabled()) {
        for (const auto& ext : name_extensions) {
            if (ext == "FieldNumber") {
                binary_field_name += std::to_string(const_cast<resource::definition::type::field *>(ctx.current_field)->repeatable().lower_bound());
            }
        }
    }

    if (!prefix.empty()) {
        binary_field_name = prefix + "." + binary_field_name;
    }

    ctx.current_binary_field = &ctx.current_type->binary_template()->field_named(binary_field_name);
    resource::definition::type::field_value value(ctx.current_binary_field, value_identifier.path_value("."));
    value.add_decorator(decorator::name::documentation, documentation);

    for (const auto& ext : name_extensions) {
        value.add_name_extension(ext);
    }

    if (stream.expect({ expectation(tokenizer::as_keyword).be_true() })) {
        stream.advance();
        value.set_type(descriptor::parse(stream, ctx), true);
    }
    else {
        value.set_type(descriptor::infer_type(ctx), false);
    }

    if (stream.expect({ expectation(tokenizer::equals).be_true() })) {
        stream.advance();
        value.set_default_value(script::parse_statement(stream, ctx));
    }

    if (sema::type_definition::field_definition::symbol_list::test(stream)) {
        sema::type_definition::field_definition::symbol_list::parse(stream, ctx, value);
    }

    return value;
}
