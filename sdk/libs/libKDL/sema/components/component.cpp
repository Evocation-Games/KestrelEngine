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

#include <libKDL/sema/components/component.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/codegen/lua/exporter.hpp>
#include <libKDL/assembler/compiler/lua/lua.hpp>
#include <libGraphite/data/writer.hpp>
#include <libGraphite/data/reader.hpp>

auto kdl::sema::component::test(const foundation::stream<tokenizer::token>& stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::component_keyword).be_true(),
        expectation(tokenizer::l_angle).be_true()
    });
}

auto kdl::sema::component::parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void
{
    stream.ensure({ expectation(tokenizer::component_keyword).be_true(), expectation(tokenizer::l_angle).be_true() });

    if (!stream.expect_any({ expectation(tokenizer::identifier).be_true(), expectation(tokenizer::identifier_path).be_true() })) {
        throw std::runtime_error("");
    }
    auto component_type = stream.read();

    if (!stream.expect_any({ expectation(tokenizer::comma).be_true(), expectation(tokenizer::reference).be_true() })) {
        throw std::runtime_error("");
    }
    stream.advance();
    auto reference = stream.read().reference_value();
    if (component_type.associated_values().size() == 1) {
        reference = reference.with_type_name(component_type.associated_values()[0], "????");
    }
    else if (component_type.is(tokenizer::identifier)) {
        reference = reference.with_type_name(component_type.string_value(), "????");
    }
    else if (component_type.associated_values().size() == 2) {
        reference = reference
            .with_type_name(component_type.associated_values().back(), "????")
            .with_container(component_type.associated_values().front());
    }
    else {
        throw std::runtime_error("");
    }

    stream.ensure({ expectation(tokenizer::r_angle).be_true() });

    if (!stream.expect_any({ expectation(tokenizer::identifier).be_true() })) {
        throw std::runtime_error("");
    }
    auto component_name = stream.read();

    stream.ensure({ expectation(tokenizer::l_brace).be_true() });
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (stream.expect({ expectation(tokenizer::files_keyword).be_true() })) {
            stream.advance();
            parse_files(stream, ctx, reference);
        }
        else if (stream.expect({ expectation(tokenizer::types_keyword).be_true() })) {
            stream.advance();
            parse_types(stream, ctx, reference);
        }
        else {
            throw std::runtime_error("");
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}

// MARK: - Resources

auto kdl::sema::component::synthesize_resource(context &ctx, resource::reference &ref, const foundation::filesystem::path &path, const std::string &name) -> void
{
    auto output_type = ctx.type_named(ref.type_name());
    auto tmpl = output_type->binary_template();

    if (tmpl && (tmpl->field_count() == 1) && (tmpl->all_fields().front().has_lua_byte_code_type())) {
        foundation::filesystem::file source_file(path);
        auto byte_code = assembler::compiler::lua::compile(source_file.string_contents(), path.string());
        resource::instance resource(ref, byte_code);
        resource.set_name(name);
        if (!ctx.flags.surpress_resource_creation) {
            ctx.resources.emplace_back(std::move(resource));
        }
    }
    else {
        graphite::data::block block(path.string());
        resource::instance resource(ref, block);
        resource.set_name(name);
        if (!ctx.flags.surpress_resource_creation) {
            ctx.resources.emplace_back(std::move(resource));
        }
    }

    ref = ref.with_id(ref.id() + 1);
}

auto kdl::sema::component::synthesize_resource(context &ctx, resource::reference &ref, const std::string &content, const std::string &name) -> void
{
    auto output_type = ctx.type_named(ref.type_name());
    auto tmpl = output_type->binary_template();

    graphite::data::block block;

    if (tmpl && (tmpl->field_count() == 1) && (tmpl->all_fields().front().has_lua_byte_code_type())) {
        auto byte_code = assembler::compiler::lua::compile(content, name);
        block = byte_code;
    }
    else {
        block = graphite::data::block(content.size() + 1);
        graphite::data::writer writer(&block);
        writer.write_cstr(content);
    }

    resource::instance resource(ref, block);
    resource.set_name(name);
    if (!ctx.flags.surpress_resource_creation) {
        ctx.resources.emplace_back(std::move(resource));
    }
    ref = ref.with_id(ref.id() + 1);
}

// MARK: - Files

auto kdl::sema::component::parse_files(foundation::stream<tokenizer::token> &stream, context &ctx, resource::reference &ref) -> void
{
    if (!stream.expect({
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::string).be_true(),
        expectation(tokenizer::r_paren).be_true(),
        expectation(tokenizer::l_brace).be_true()
    })) {
        throw std::runtime_error("");
    }
    stream.advance();
    auto path_prefix = stream.read();
    stream.advance(2);

    foundation::filesystem::path prefix(path_prefix.string_value());

    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (stream.expect({
            expectation(tokenizer::string).be_true(),
            expectation(tokenizer::minus).be_true(),
            expectation(tokenizer::r_angle).be_true(),
            expectation(tokenizer::string).be_true()
        })) {
            auto file = stream.read();
            stream.advance(2);
            auto name = stream.read();
            synthesize_resource(ctx, ref, prefix.appending_path_component(file.string_value()), name.string_value());
        }
        else if (stream.expect({
            expectation(tokenizer::string).be_true()
        })) {
            auto file = stream.read();
            synthesize_resource(ctx, ref, prefix.appending_path_component(file.string_value()));
        }
        else {
            throw std::runtime_error("");
        }

        // Check for any hints applied to the entry...
        if (stream.expect({ expectation(tokenizer::l_paren).be_true() })) {
            stream.consume(expectation(tokenizer::r_paren).be_false());
            stream.ensure({ expectation(tokenizer::r_paren).be_true() });
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    };
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}

// MARK: - Types

auto kdl::sema::component::parse_types(foundation::stream<tokenizer::token> &stream, context &ctx, resource::reference &ref) -> void
{
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (stream.expect({ expectation(tokenizer::identifier).be_true() })) {
            auto type_name = stream.read();
            if (auto type = ctx.type_named(type_name.source())) {
                codegen::lua::exporter exporter(type, ctx.create_scope());
                synthesize_resource(ctx, ref, exporter.generate(), type_name.string_value());
            }
            else {
                throw std::runtime_error("");
            }
        }
        else {
            throw std::runtime_error("");
        }
        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }

    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}