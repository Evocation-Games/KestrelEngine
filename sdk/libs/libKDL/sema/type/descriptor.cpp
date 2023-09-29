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

#include <libKDL/sema/type/descriptor.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/spec/types.hpp>
#include <libResource/definition/template/instance.hpp>

auto kdl::sema::type_definition::descriptor::parse(foundation::stream<tokenizer::token> &stream, context &ctx) -> resource::definition::type::descriptor
{
    std::optional<std::string> type_name;
    std::vector<std::string> hints;
    bool is_reference = false;
    bool allows_hints = false;

    if (stream.expect({ expectation(tokenizer::identifier).be_true() })) {
        type_name = stream.read().string_value();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::named_reference_type).be_true() })) {
        type_name = stream.read().string_value();
        is_reference = true;
        allows_hints = false;
    }
    else if (stream.expect({ expectation(tokenizer::string_type).be_true() })){
        type_name = spec::types::string;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::integer_type).be_true() })){
        type_name = spec::types::integer;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::bitmask_type).be_true() })){
        type_name = spec::types::bitmask;
        auto tk = stream.read();
        hints = tk.associated_values();
    }
    else if (stream.expect({ expectation(tokenizer::file_type).be_true() })){
        type_name = spec::types::file;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::image_type).be_true() })){
        type_name = spec::types::image;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::data_type).be_true() })){
        type_name = spec::types::data;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::image_set_type).be_true() })){
        type_name = spec::types::image_set;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::sound_type).be_true() })){
        type_name = spec::types::sound;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::command_encoder_type).be_true() })){
        type_name = spec::types::command_encoder;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = false;
    }
    else if (stream.expect({ expectation(tokenizer::range_type).be_true() })){
        type_name = spec::types::range;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = true;
    }
    else if (stream.expect({ expectation(tokenizer::color_type).be_true() })){
        type_name = spec::types::color;
        auto tk = stream.read();
        hints = tk.associated_values();
        allows_hints = false;
    }

    if (allows_hints && stream.expect({ expectation(tokenizer::l_angle).be_true() })) {
        stream.advance();
        while (stream.expect({ expectation(tokenizer::r_angle).be_false() })) {
            hints.emplace_back(stream.read().string_value());

            if (stream.expect({ expectation(tokenizer::comma).be_true() })) {
                stream.advance();
                continue;
            }
            else if (stream.expect({ expectation(tokenizer::r_angle).be_true() })) {
                break;
            }
        }
        stream.ensure({ expectation(tokenizer::r_angle).be_true() });
    }

    if (stream.expect({ expectation(tokenizer::amp).be_true() })) {
        stream.advance();
        is_reference = true;
    }

    if (type_name.has_value()) {
        return { is_reference, type_name.value(), hints };
    }
    else {
        return resource::definition::type::descriptor(is_reference);
    }
}

auto kdl::sema::type_definition::descriptor::infer_type(context &ctx) -> resource::definition::type::descriptor
{
    switch (ctx.current_binary_field->type().value()) {
        case resource::definition::binary_template::type::DBYT:
        case resource::definition::binary_template::type::DWRD:
        case resource::definition::binary_template::type::DLNG:
        case resource::definition::binary_template::type::DQWD:
        case resource::definition::binary_template::type::HBYT:
        case resource::definition::binary_template::type::HWRD:
        case resource::definition::binary_template::type::HLNG:
        case resource::definition::binary_template::type::HQWD:
            return { false, spec::types::integer };

        case resource::definition::binary_template::type::RECT:
            return { false, "Rect" };

        case resource::definition::binary_template::type::PSTR:
        case resource::definition::binary_template::type::CSTR:
        case resource::definition::binary_template::type::Cnnn:
        case resource::definition::binary_template::type::LSTR:
        case resource::definition::binary_template::type::OSTR:
            return { false, spec::types::string };

        case resource::definition::binary_template::type::CHAR:
        case resource::definition::binary_template::type::OCNT:
            return { false, spec::types::integer };

        case resource::definition::binary_template::type::LSTC:
            return { false, "$ListStart" };

        case resource::definition::binary_template::type::LSTE:
            return { false, "$ListEnd" };

        case resource::definition::binary_template::type::BBIT:
        case resource::definition::binary_template::type::BOOL:
            return { false, spec::types::boolean };

        case resource::definition::binary_template::type::HEXD:
            return { false, spec::types::data };

        case resource::definition::binary_template::type::RSRC:
            return { true, "" };

        case resource::definition::binary_template::type::NESTED:
            return { true, ctx.current_binary_field->type().nested_type_name() };

        case resource::definition::binary_template::type::LUA_BYTE_CODE:
            return { false, spec::types::string };
    }
}