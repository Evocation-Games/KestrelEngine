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

#include <libKDL/sema/declaration/field_declaration.hpp>
#include <libKDL/sema/declaration/resource_declaration.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/spec/types.hpp>
#include <libResource/reference.hpp>
#include <libResource/definition/template/instance.hpp>
#include <libInterpreter/construct/function.hpp>
#include <libInterpreter/token/token.hpp>
#include <libGraphite/data/data.hpp>
#include <libImage/conversion/conversion.hpp>
#include <libSound/conversion/conversion.hpp>

// MARK: - Helpers

static auto relocate_this(const std::string& field_name, const std::string& id) -> std::string
{
    if (!id.starts_with("this")) {
        return id;
    }
    return field_name + id.substr(4);
}

static auto convert_token(const interpreter::token& token) -> ::resource::value_container
{
    switch (token.type()) {
        case interpreter::token::integer: {
            return ::resource::value_container(token.integer_value());
        }
        case interpreter::token::string: {
            return ::resource::value_container(token.string_value());
        }
        case interpreter::token::reference: {
            return ::resource::value_container(resource::reference(token.integer_value()));
        }
        default: {
            return {};
        }
    }
}

static auto is_builtin_type(const resource::definition::type::descriptor *descriptor) -> bool
{
    if (!descriptor || !descriptor->has_name()) {
        return false;
    }

    return descriptor->name() == kdl::spec::types::integer
        || descriptor->name() == kdl::spec::types::string
        || descriptor->name() == kdl::spec::types::data
        || descriptor->name() == kdl::spec::types::bitmask
        || descriptor->name() == kdl::spec::types::boolean
        || descriptor->name() == kdl::spec::types::file
        || descriptor->name() == kdl::spec::types::image
        || descriptor->name() == kdl::spec::types::image_set
        || descriptor->name() == kdl::spec::types::sound;
}

static auto can_multi_import(const resource::definition::type::descriptor *descriptor) -> bool
{
    if (!descriptor || !descriptor->has_name()) {
        return false;
    }
    return descriptor->name() == kdl::spec::types::image_set;
}

static auto install_constructor(
    kdl::sema::context &ctx,
    const std::string& value_name,
    const resource::definition::type::instance *type,
    std::unordered_map<std::string, ::resource::value_container>& values
)
    -> void
{
    // Make sure we have a type, and it has a constructor available.
    if (!type || type->constructors().empty() || ctx.active_scope()->has_function(type->name())) {
        return;
    }

    interpreter::function constructor_function(type->name(),
        [type, &values, value_name] (interpreter::scope *scope, const std::vector<interpreter::token> &arguments) -> interpreter::token {
            const auto& constructor = type->constructors().back();
            auto result = constructor.execute(*scope, arguments, *type->binary_template());

            for (const auto& it : result.variables) {
                auto value_id = relocate_this(value_name, it.first);
                values.emplace(value_id, convert_token(it.second));
            }

            return interpreter::token(0LL);
        });
    ctx.active_scope()->add_function(constructor_function);

    // Find nested types, and register their constructors as well.
    for (const auto& field : type->binary_template()->all_fields()) {
        if (field.has_nested_type()) {
            const auto *value_type = ctx.type_named(field.type().nested_type_name());
            install_constructor(ctx, value_name + "." + field.label(), value_type, values);
        }
    }
}

static auto write_value(
    std::unordered_map<std::string, ::resource::value_container>& values,
    const std::string& name,
    const ::resource::value_container& value
)
    -> void
{
    auto it = values.find(name);
    if (it == values.end()) {
        values.emplace(name, value);
    }
    else {
        it->second = value;
    }
}

static auto write_value(
    std::unordered_map<std::string, ::resource::value_container>& values,
    const std::string& name,
    const resource::definition::type::descriptor& type,
    const foundation::filesystem::file& file
)
    -> void
{
    if (!type.has_name() || type.name() == kdl::spec::types::data) {
        values.emplace(name, ::resource::value_container(file.bytes()));
    }
    else if (type.name() == kdl::spec::types::string) {
        values.emplace(name, ::resource::value_container(file.string_contents()));
    }
    else if (type.name() == kdl::spec::types::file) {
        values.emplace(name, ::resource::value_container(file.bytes()));
    }
    else if (type.name() == kdl::spec::types::image) {
        if (!type.has_hints() || type.hints().size() != 2) {
            throw std::runtime_error("");
        }
        auto source = type.hints()[0];
        auto result = type.hints()[1];

        graphite::data::block source_data(file.characters());
        graphite::data::block result_data = image::conversion::perform(source, result, source_data);
        values.emplace(name, ::resource::value_container(result_data.get<std::uint8_t *>(), result_data.size()));
    }
    else if (type.name() == kdl::spec::types::sound) {
        if (!type.has_hints() || type.hints().size() != 2) {
            throw std::runtime_error("");
        }
        auto source = type.hints()[0];
        auto result = type.hints()[1];

        graphite::data::block source_data(file.characters());
        graphite::data::block result_data = sound::conversion::perform(source, result, source_data);
        values.emplace(name, ::resource::value_container(result_data.get<std::uint8_t *>(), result_data.size()));
    }
    else {
        throw std::runtime_error("");
    }
}

static auto write_value(
    std::unordered_map<std::string, ::resource::value_container>& values,
    const std::string& name,
    const resource::definition::type::descriptor& type,
    const std::vector<graphite::data::block>& blocks
)
-> void
{
    if (type.name() == kdl::spec::types::image_set) {
        if (!type.has_hints() || type.hints().size() != 2) {
            throw std::runtime_error("");
        }
        auto source = type.hints()[0];
        auto result = type.hints()[1];

        graphite::data::block result_data = image::conversion::perform(source, result, blocks);
        values.emplace(name, ::resource::value_container(result_data.get<std::uint8_t *>(), result_data.size()));
    }
    else {
        throw std::runtime_error("");
    }
}

// MARK: - Parsing

auto kdl::sema::declaration::resource::field::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::equals).be_true() });
}

auto kdl::sema::declaration::resource::field::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> std::unordered_map<std::string, ::resource::value_container>
{
    if (!test(stream)) {
        throw std::runtime_error("");
    }
    auto field_name = stream.read(); stream.advance();

    // Look up the field in the type definition.
    if (!ctx.current_type->has_field_named(field_name.string_value())) {
        throw std::runtime_error("");
    }
    auto field = ctx.current_type->field_named(field_name.string_value());

    // We need to parse out values, for each expected value.
    std::int32_t value_index = 0;
    std::unordered_map<std::string, ::resource::value_container> values;
    auto current_type = ctx.current_type;
    auto current_type_descriptor = ctx.current_type_descriptor;
    while (stream.expect({ expectation(tokenizer::semi).be_false() })) {
        if (value_index >= field.value_count()) {
            throw std::runtime_error("");
        }

        const auto& value = field.value_at(value_index);
        const auto& value_name = value.extended_name(*ctx.active_scope());
        ctx.current_type_descriptor = &value.type();
        ctx.current_type = nullptr;
        if (!is_builtin_type(ctx.current_type_descriptor)) {
            ctx.current_type = ctx.type(*ctx.current_type_descriptor);
        }

        // Determine what type of value parser to load up.
        if (stream.expect({ expectation(tokenizer::new_keyword).be_true() })) {
            // Reference resource - we need to pop on the resource type here and save the current one.
            // Parse the resource instance and get the reference information.
            auto resource = resource::parse_resource(stream, ctx, true);
            ctx.resources.emplace_back(resource);
            write_value(values, value_name, ::resource::value_container(resource.reference()));
        }
        else if (stream.expect({ expectation(tokenizer::import_keyword).be_true() }) && can_multi_import(&value.type())) {
            stream.advance(1);
            std::vector<graphite::data::block> files;

            if (!stream.expect({ expectation(tokenizer::l_brace).be_true() })) {
                throw std::runtime_error("");
            }

            while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
                // Import the contents of a file to use for the value.
                auto import_path_stmt = script::parse_statement(stream, ctx);
                auto import_path_result = import_path_stmt.evaluate(ctx.active_scope());

                if (import_path_result.status == interpreter::script::statement::result::error) {
                    throw std::runtime_error("");
                }

                // TODO: Setup a path resolver.
                foundation::filesystem::path import_path(import_path_result.value.string_value());
                if (!import_path.exists()) {
                    throw std::runtime_error("");
                }

                foundation::filesystem::file file(import_path, foundation::filesystem::file_mode::binary);
                files.emplace_back(file.characters());
            }
            stream.ensure({ expectation(tokenizer::r_brace).be_true() });

            // Load the file and apply the data to the value.
            write_value(values, value_name, value.type(), files);
        }
        else if (stream.expect({ expectation(tokenizer::import_keyword).be_true() })) {
            // Import the contents of a file to use for the value.
            auto import_path_stmt = script::parse_statement(stream, ctx);
            auto import_path_result = import_path_stmt.evaluate(ctx.active_scope());

            if (import_path_result.status == interpreter::script::statement::result::error) {
                throw std::runtime_error("");
            }

            // TODO: Setup a path resolver.
            foundation::filesystem::path import_path(import_path_result.value.string_value());
            if (!import_path.exists()) {
                throw std::runtime_error("");
            }

            // Load the file and apply the data to the value.
            foundation::filesystem::file imported(import_path, foundation::filesystem::file_mode::binary);
            write_value(values, value_name, value.type(), imported);
        }
        else {
            // Expression based argument (single value, constructor, etc)
            auto value_expression = script::parse_statement(stream, ctx);
            auto value_scope = ctx.create_scope();
            if (ctx.current_type && !ctx.current_type->constructors().empty()) {
                install_constructor(ctx, value_name, ctx.current_type, values);
            }
            auto result = value_expression.evaluate(value_scope);
            write_value(values, value_name, convert_token(result.value));
        }

        // Move to the next value?..
        value_index++;

        if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
            stream.advance();
            continue;
        }

        // If we hit this point, then we can not accept another parameter.
        break;
    }
    ctx.current_type = current_type;
    ctx.current_type_descriptor = current_type_descriptor;

    return std::move(values);
}
