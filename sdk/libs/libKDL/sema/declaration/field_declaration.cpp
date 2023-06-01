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
#include <libKDL/sema/declaration/field_declaration.hpp>
#include <libKDL/sema/declaration/resource_declaration.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/command/encoder.hpp>
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
            return ::resource::value_container(token.reference_value());
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
        || descriptor->name() == kdl::spec::types::sound
        || descriptor->name() == kdl::spec::types::command_encoder
        || descriptor->name() == kdl::spec::types::color
        || descriptor->name() == kdl::spec::types::range
        || descriptor->name() == kdl::spec::types::rect;
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

            return interpreter::token(static_cast<std::int64_t>(0));
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

static auto install_color_functions(kdl::sema::context &ctx) -> void
{
    interpreter::function rgb_function("rgb", [] (interpreter::scope *scope, const std::vector<interpreter::token> &arguments) -> interpreter::token {
        std::uint32_t value = 0;

        auto r = static_cast<std::uint8_t>(arguments.at(0).integer_value() & 0xFF);
        auto g = static_cast<std::uint8_t>(arguments.at(1).integer_value() & 0xFF);
        auto b = static_cast<std::uint8_t>(arguments.at(2).integer_value() & 0xFF);
        value = (0xFF << 24) | (r << 16) | (g << 8) | b;

        return interpreter::token(static_cast<std::int64_t>(value));
    });
    rgb_function.add_parameter("r", interpreter::token::integer);
    rgb_function.add_parameter("g", interpreter::token::integer);
    rgb_function.add_parameter("b", interpreter::token::integer);


    interpreter::function rgba_function("rgba", [] (interpreter::scope *scope, const std::vector<interpreter::token> &arguments) -> interpreter::token {
        std::uint32_t value = 0;

        auto r = static_cast<std::uint8_t>(arguments.at(0).integer_value() & 0xFF);
        auto g = static_cast<std::uint8_t>(arguments.at(1).integer_value() & 0xFF);
        auto b = static_cast<std::uint8_t>(arguments.at(2).integer_value() & 0xFF);
        auto a = static_cast<std::uint8_t>(arguments.at(3).integer_value() & 0xFF);
        value = (a << 24) | (r << 16) | (g << 8) | b;

        return interpreter::token(static_cast<std::int64_t>(value));
    });
    rgba_function.add_parameter("r", interpreter::token::integer);
    rgba_function.add_parameter("g", interpreter::token::integer);
    rgba_function.add_parameter("b", interpreter::token::integer);
    rgba_function.add_parameter("a", interpreter::token::integer);

    ctx.active_scope()->add_function(rgb_function);
    ctx.active_scope()->add_function(rgba_function);
}

static auto install_rect_functions(kdl::sema::context &ctx) -> void
{
    interpreter::function frame_function("frame", [] (interpreter::scope *scope, const std::vector<interpreter::token> &arguments) -> interpreter::token {
        union {
            std::uint64_t value { 0 };
            struct {
                std::int16_t right;
                std::int16_t bottom;
                std::int16_t left;
                std::int16_t top;
            } fields;
        } rect;

        auto x = static_cast<std::int16_t>(arguments.at(0).integer_value());
        auto y = static_cast<std::int16_t>(arguments.at(1).integer_value());
        auto width = static_cast<std::int16_t>(arguments.at(2).integer_value());
        auto height = static_cast<std::int16_t>(arguments.at(3).integer_value());

        rect.fields.left = x;
        rect.fields.top = y;
        rect.fields.bottom = y + height;
        rect.fields.right = x + width;

        return interpreter::token(static_cast<std::int64_t>(rect.value));
    });
    frame_function.add_parameter("x", interpreter::token::integer);
    frame_function.add_parameter("y", interpreter::token::integer);
    frame_function.add_parameter("width", interpreter::token::integer);
    frame_function.add_parameter("height", interpreter::token::integer);

    interpreter::function rect_function("rect", [] (interpreter::scope *scope, const std::vector<interpreter::token> &arguments) -> interpreter::token {
        union {
            std::uint64_t value { 0 };
            struct {
                std::int16_t d;
                std::int16_t c;
                std::int16_t b;
                std::int16_t a;
            } fields;
        } rect;

        rect.fields.a = static_cast<std::int16_t>(arguments.at(0).integer_value());
        rect.fields.b = static_cast<std::int16_t>(arguments.at(1).integer_value());
        rect.fields.c = static_cast<std::int16_t>(arguments.at(2).integer_value());
        rect.fields.d = static_cast<std::int16_t>(arguments.at(3).integer_value());

        return interpreter::token(static_cast<std::int64_t>(rect.value));
    });
    rect_function.add_parameter("a", interpreter::token::integer);
    rect_function.add_parameter("b", interpreter::token::integer);
    rect_function.add_parameter("c", interpreter::token::integer);
    rect_function.add_parameter("d", interpreter::token::integer);

    ctx.active_scope()->add_function(rect_function);
    ctx.active_scope()->add_function(frame_function);
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
    else if (type.name() == kdl::spec::types::image_set) {
        if (!type.has_hints() || type.hints().size() != 2) {
            throw std::runtime_error("");
        }
        auto source = type.hints()[0];
        auto result = type.hints()[1];

        graphite::data::block source_data(file.characters());
        graphite::data::block result_data = image::conversion::perform(source, result,{ source_data });
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

    // Determine the field repeat information
    auto field_scope = ctx.create_scope();
    if (field.repeatable().enabled()) {
        auto count_field = field.name();
        if (field.repeatable().has_count_field()) {
            count_field = field.repeatable().count_field()->label();
        }

        std::int64_t field_number = 0;
        auto it = ctx.field_repeat_counts.find(count_field);
        if (it != ctx.field_repeat_counts.end()) {
            field_number = ++it->second;
            if (field_number > field.repeatable().upper_bound()) {
                throw std::runtime_error("");
            }
        }
        else {
            field_number = field.repeatable().lower_bound();
            ctx.field_repeat_counts.emplace(count_field, field_number);
        }
        field_scope->add_variable("FieldNumber", field_number);
        values.emplace(count_field + std::to_string(field_number), ::resource::value_container(field_number));
    }

    if (stream.expect({ expectation(tokenizer::l_brace).be_true() }) && field.repeatable().enabled()) {
        stream.advance();
        while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
            if (stream.expect({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::equals).be_true() })) {
                const ::resource::definition::type::field_value *field_ref = nullptr;
                auto value_name = stream.read(); stream.advance();

                for (const auto& value : field.values()) {
                    if (value.extended_name(*field_scope) == value_name.string_value()) {
                        field_ref = &value;
                        break;
                    }
                    else if (value.base_name() == value_name.string_value()) {
                        field_ref = &value;
                        break;
                    }
                    else if (value.has_decorator("api", { value_name.string_value() })) {
                        field_ref = &value;
                        break;
                    }
                }

                if (!field_ref) {
                    throw std::runtime_error("");
                }

                parse_value(stream, ctx, field_scope, values, *field_ref, field_ref->extended_name(*field_scope));
            }
            stream.ensure({ expectation(tokenizer::semi).be_true() });
        }
        stream.ensure({ expectation(tokenizer::r_brace).be_true() });
    }
    else {
        // Parse the field values as a comma separated list.
        while (stream.expect({ expectation(tokenizer::semi).be_false() })) {
            if (value_index >= field.value_count()) {
                throw std::runtime_error("");
            }

            const auto& value = field.value_at(value_index);
            auto value_name = value.extended_name(*field_scope);

            ctx.current_type_descriptor = &value.type();
            ctx.current_type = nullptr;
            if (!is_builtin_type(ctx.current_type_descriptor)) {
                ctx.current_type = ctx.type(*ctx.current_type_descriptor);
            }

            parse_value(stream, ctx, field_scope, values, value, value_name);

            // Move to the next value?..
            if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                stream.advance();
                value_index++;
                continue;
            }

            // If we hit this point, then we can not accept another parameter.
            break;
        }

        // Apply defaults...
        for (auto n = value_index + 1; n < field.value_count(); ++n) {
            const auto& value = field.value_at(n);
            auto value_name = value.extended_name(*field_scope);
            if (!value.has_default_value()) {
                continue;
            }
            stream.push(tokenizer::token(tokenizer::default_value));
            parse_value(stream, ctx, field_scope, values, value, value_name);
            stream.clear_pushed_items();
        }

    }

    ctx.current_type = current_type;
    ctx.current_type_descriptor = current_type_descriptor;

    return std::move(values);
}

auto kdl::sema::declaration::resource::field::parse_value(
    foundation::stream<tokenizer::token>& stream,
    context& ctx,
    interpreter::scope *scope,
    std::unordered_map<std::string, ::resource::value_container>& values,
    const ::resource::definition::type::field_value& value,
    const std::string& value_name
)
    -> void
{
    // Determine what type of value parser to load up.
    if (value.type().name() == spec::types::command_encoder) {
        if (!stream.peek().is(tokenizer::default_value)) {
            auto data = sema::command_encoder::parse(stream, ctx);
            values.emplace(value_name, ::resource::value_container(data));
        }
    }
    else if (value.has_joined_values() && (value.type().name() == spec::types::bitmask)) {
        std::vector<std::uint64_t> masks(value.joined_values().size() + 1, 0);
        while (true) {
            if (stream.expect_any({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::default_value).be_true() })) {
                auto symbol = stream.read();
                for (auto n = 0; n < masks.size(); ++n) {
                    if (n == 0 && value.has_symbol_named(symbol.string_value())) {
                        masks[0] |= value.symbol_named(symbol.string_value())
                            .value().integer_value<std::uint64_t>();
                    }
                    else if (n == 0 && symbol.is(tokenizer::default_value) && value.has_default_value()) {
                        masks[0] |= static_cast<std::uint64_t>(value.default_value()
                            .evaluate(scope).value.integer_value());
                    }
                    else if (n > 0 && value.joined_values()[n - 1].has_symbol_named(symbol.string_value())) {
                        masks[n] |= value.joined_values()[n - 1]
                            .symbol_named(symbol.string_value())
                            .value().integer_value<std::uint64_t>();
                    }
                    else if (n > 0 && symbol.is(tokenizer::default_value) && value.has_default_value()) {
                        masks[n] |= static_cast<std::uint64_t>(value.joined_values()[n - 1].default_value()
                            .evaluate(scope).value.integer_value());
                    }
                }
            }
            else if (stream.expect({ expectation(tokenizer::integer).be_true() })) {
                auto i = stream.read();
                masks[0] |= i.value<std::uint64_t>();
            }
            else {
                throw std::runtime_error("");
            }

            if (stream.expect({ expectation(tokenizer::pipe).be_true() })) {
                stream.advance();
                continue;
            }
            break;
        }

        for (auto n = 0; n < masks.size(); ++n) {
            if (n == 0) {
                write_value(values, value_name, ::resource::value_container(masks[n]));
            }
            else {
                write_value(values, value.joined_values()[n - 1].extended_name(*scope), ::resource::value_container(masks[n]));
            }
        }
    }
    else if (stream.expect({ expectation(tokenizer::new_keyword).be_true() })) {
        // Reference resource - we need to pop on the resource type here and save the current one.
        // Parse the resource instance and get the reference information.
        auto repeat_field_counts = ctx.field_repeat_counts;
        ctx.field_repeat_counts.clear();
        auto resource = resource::parse_resource(stream, ctx, true);
        ctx.field_repeat_counts = repeat_field_counts;
        if (!ctx.flags.surpress_resource_creation) {
            ctx.resources.emplace_back(resource);
        }
        write_value(values, value_name, ::resource::value_container(resource.reference()));
    }
    else if (stream.expect({ expectation(tokenizer::import_keyword).be_true(), expectation(tokenizer::l_brace).be_true() }) && can_multi_import(&value.type())) {
        auto source_path = stream.peek().source().source_directory();
        stream.advance(1);
        std::vector<graphite::data::block> files;

        if (!stream.expect({ expectation(tokenizer::l_brace).be_true() })) {
            throw std::runtime_error("");
        }

        while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
            // Import the contents of a file to use for the value.
            auto import_path_stmt = script::parse_statement(stream, ctx);
            auto import_path_result = import_path_stmt.evaluate(scope);

            if (import_path_result.status == interpreter::script::statement::result::error) {
                throw std::runtime_error("");
            }

            // TODO: Setup a path resolver.
            auto import_path = ctx.resolve_path(import_path_result.value.string_value(), source_path);
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
        auto source_path = stream.peek().source().source_directory();

        // Import the contents of a file to use for the value.
        auto import_path_stmt = script::parse_statement(stream, ctx);
        auto import_path_result = import_path_stmt.evaluate(scope);

        if (import_path_result.status == interpreter::script::statement::result::error) {
            throw std::runtime_error("");
        }

        auto import_path = ctx.resolve_path(import_path_result.value.string_value(), source_path);
        if (!import_path.exists()) {
            throw std::runtime_error("");
        }

        // Load the file and apply the data to the value.
        foundation::filesystem::file imported(import_path, foundation::filesystem::file_mode::binary);
        write_value(values, value_name, value.type(), imported);
    }
    else {
        // Expression based argument (single value, constructor, etc)
        interpreter::script::statement value_expression;
        if (stream.expect({ expectation(tokenizer::default_value).be_true() })) {
            if (!value.has_default_value()) {
                return;
            }
            value_expression = value.default_value();
        }
        else {
            value_expression = script::parse_statement(stream, ctx);
        }
        auto value_scope = ctx.create_scope();

        // Install Constructors / Functions
        if (ctx.current_type && !ctx.current_type->constructors().empty()) {
            install_constructor(ctx, value_name, ctx.current_type, values);
        }
        else if (ctx.current_type_descriptor && ctx.current_type_descriptor->name() == spec::types::color) {
            install_color_functions(ctx);
        }
        else if (ctx.current_type_descriptor && ctx.current_type_descriptor->name() == spec::types::rect) {
            install_rect_functions(ctx);
        }

        // Install symbols
        if (value.has_symbols()) {
            // Primary
            for (const auto& symbol : value.symbols()) {
                auto symbol_name = symbol.first;
                auto symbol_value = symbol.second.value();
                value_scope->add_variable(symbol_name, symbol_value);
            }

            // Joined
            if (value.has_joined_values()) {

            }
        }

        auto result = value_expression.evaluate(value_scope);

        // If the result is a reference, we need to try and resolve the type code.
        switch (result.type) {
            case interpreter::token::reference: {
                auto ref = result.value.reference_value();
                if (ref.has_type_name() && !ref.has_type_code()) {
                    const auto& type_name = ref.type_name();
                    if (auto type = ctx.type_named(type_name)) {
                        ref = ref.with_type_name(type_name, type->code());
                    }
                }
                result.value = interpreter::token(ref);
                break;
            }
            default: break;
        }

        write_value(values, value_name, convert_token(result.value));
    }
}
