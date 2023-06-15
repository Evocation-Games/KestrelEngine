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

#include <libFoundation/string/trim.hpp>
#include <libLexer/lexer.hpp>
#include "analyzer/cxx/types/parser.hpp"
#include "analyzer/cxx/types/scalar_types.hpp"
#include "analyzer/cxx/types/complex_types.hpp"
#include "analyzer/cxx/types/expectation.hpp"

// MARK: - Construction

kdtool::cxx::type_parser::type_parser(const std::string &type_string)
    : m_raw(type_string)
{}

// MARK: - Type Provider Conformance

auto kdtool::cxx::type_parser::type_for(enum project::structure::type::scalar scalar) -> std::shared_ptr<struct project::structure::type::base>
{
    switch (scalar) {
        case project::structure::type::scalar::error_type:          return std::make_shared<scalar::error_type>();
        case project::structure::type::scalar::void_type:           return std::make_shared<scalar::void_type>();
        case project::structure::type::scalar::boolean_type:        return std::make_shared<scalar::bool_type>();
        case project::structure::type::scalar::char_type:           return std::make_shared<scalar::int8_type>();
        case project::structure::type::scalar::short_type:          return std::make_shared<scalar::int16_type>();
        case project::structure::type::scalar::long_type:           return std::make_shared<scalar::int32_type>();
        case project::structure::type::scalar::long_long_type:      return std::make_shared<scalar::int64_type>();
        case project::structure::type::scalar::uchar_type:          return std::make_shared<scalar::uint8_type>();
        case project::structure::type::scalar::ushort_type:         return std::make_shared<scalar::uint16_type>();
        case project::structure::type::scalar::ulong_type:          return std::make_shared<scalar::uint32_type>();
        case project::structure::type::scalar::ulong_long_type:     return std::make_shared<scalar::uint64_type>();
        case project::structure::type::scalar::float_type:          return std::make_shared<scalar::float_type>();
        case project::structure::type::scalar::double_type:         return std::make_shared<scalar::double_type>();
        case project::structure::type::scalar::long_double_type:    return std::make_shared<scalar::long_double_type>();
        case project::structure::type::scalar::int_type:            return std::make_shared<scalar::int_type>();
        case project::structure::type::scalar::uint_type:           return std::make_shared<scalar::unsigned_int_type>();
    }
}

auto kdtool::cxx::type_parser::type_for(const std::string& str) -> std::shared_ptr<struct project::structure::type::base>
{
    auto type_str = foundation::string::trim(str);

    // Before proceeding, check to see if the string is the name of the type.
    auto type = construct_type_for(type_str, nullptr);
    if (type) {
        return type;
    }

    // We need a more complex parsing path, so construct a parser.
    type_parser parser(type_str);
    auto result = parser.parse();

    return project::structure::type::error_type();
}

auto kdtool::cxx::type_parser::construct_type_for(const std::string& type_str, struct context *ctx) -> std::shared_ptr<struct project::structure::type::base>
{
    if (type_str == "void") {
        return type_for(project::structure::type::scalar::void_type);
    }
    else if (type_str == "bool") {
        return type_for(project::structure::type::scalar::boolean_type);
    }
    else if (type_str == "char" || type_str == "std::int8_t" || type_str == "int8_t") {
        return type_for(project::structure::type::scalar::char_type);
    }
    else if (type_str == "short" || type_str == "std::int16_t" || type_str == "int16_t") {
        return type_for(project::structure::type::scalar::short_type);
    }
    else if (type_str == "long" || type_str == "std::int32_t" || type_str == "int32_t") {
        return type_for(project::structure::type::scalar::long_type);
    }
    else if (type_str == "long long" || type_str == "std::int64_t" || type_str == "int64_t") {
        return type_for(project::structure::type::scalar::long_long_type);
    }
    else if (type_str == "unsigned char" || type_str == "std::uint8_t" || type_str == "uint8_t") {
        return type_for(project::structure::type::scalar::uchar_type);
    }
    else if (type_str == "unsigned short" || type_str == "std::uint16_t" || type_str == "uint16_t") {
        return type_for(project::structure::type::scalar::ushort_type);
    }
    else if (type_str == "unsigned long" || type_str == "std::uint32_t" || type_str == "uint32_t") {
        return type_for(project::structure::type::scalar::ulong_type);
    }
    else if (type_str == "unsigned long long" || type_str == "std::uint64_t" || type_str == "uint64_t") {
        return type_for(project::structure::type::scalar::ulong_long_type);
    }
    else if (type_str == "float") {
        return type_for(project::structure::type::scalar::float_type);
    }
    else if (type_str == "double") {
        return type_for(project::structure::type::scalar::double_type);
    }
    else if (type_str == "long double") {
        return type_for(project::structure::type::scalar::long_double_type);
    }
    else if (type_str == "int") {
        return type_for(project::structure::type::scalar::int_type);
    }
    else if (type_str == "unsigned int") {
        return type_for(project::structure::type::scalar::uint_type);
    }
    else if (ctx != nullptr) {
        if (ctx->template_field_types.empty()) {
            return std::make_shared<type::basic>(type_str);
        }
        else {
            return std::make_shared<type::template_specialised>(type_str, ctx->template_field_types);
        }
    }
    return nullptr;
}

// MARK: - Parsing

auto kdtool::cxx::type_parser::parse() const -> std::shared_ptr<struct project::structure::type::base>
{
    lexer::lexer lexer(m_raw);
    auto lx_result = lexer.analyze();

    struct context ctx;
    return parse(ctx, lx_result);
}

auto kdtool::cxx::type_parser::parse(struct kdtool::cxx::type_parser::context &ctx, lexer::lexer::lexical_result& stream) const -> std::shared_ptr<struct project::structure::type::base>
{
    if (stream.expect({ type::expectation("const").be_true() })) {
        stream.advance();
        ctx.is_const = true;
    }

    // Get the type name.
    while (stream.expect({ type::expectation(lexer::identifier).be_true() })) {
        ctx.name.emplace_back(stream.read().text());

        if (stream.expect({
            type::expectation(lexer::colon).be_true(),
            type::expectation(lexer::colon).be_true(),
            type::expectation(lexer::identifier).be_true()
        })) {
            stream.advance(2);
            continue;
        }
        else {
            break;
        }
    }

    // Check for template information...
    if (stream.expect({ type::expectation(lexer::l_angle).be_true() })) {
        stream.advance();
        while (stream.expect({ type::expectation(lexer::r_angle).be_false() })) {
            struct context template_info_ctx;
            auto template_info_type = parse(template_info_ctx, stream);
            ctx.template_field_types.emplace_back(template_info_type);

            if (stream.expect({ type::expectation(lexer::comma).be_true() })) {
                stream.advance();
                continue;
            }
            break;
        }
        stream.ensure({ type::expectation(lexer::r_angle).be_true() });
    }

    if (stream.expect({ type::expectation(lexer::amp).be_true() })) {
        ctx.is_reference = true;
        stream.advance();
    }

    if (stream.expect({ type::expectation(lexer::star).be_true() })) {
        ctx.is_pointer = true;
        stream.advance();
    }

    // Construct the type itself.
    const auto name = foundation::string::joined(ctx.name, "::");
    const auto core_type = construct_type_for(name, &ctx);
    auto result = core_type;

    if (ctx.is_const) {
        result = std::make_shared<type::const_qualified>(result);
    }
    if (ctx.is_pointer) {
        result = std::make_shared<type::pointer>(result);
    }
    if (ctx.is_reference) {
        result = std::make_shared<type::reference>(result);
    }

    return result;
}