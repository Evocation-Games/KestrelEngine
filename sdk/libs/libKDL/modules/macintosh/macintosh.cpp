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

#include <libKDL/modules/macintosh/macintosh.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/definition/type/field.hpp>
#include <libResource/definition/template/instance.hpp>
#include <libKDL/spec/types.hpp>
#include <libImage/conversion/conversion.hpp>
#include <libSound/conversion/conversion.hpp>

// MARK: -

namespace kdl::modules::macintosh
{
    static inline auto construct_string_list(sema::context &ctx) -> void
    {
        auto string_list = ctx.register_type(resource::definition::type::instance("StringList", "STR#"));

        resource::definition::binary_template::instance tmpl;
        resource::definition::binary_template::field strings_binary_field(resource::definition::binary_template::type::OCNT, "Strings");
        strings_binary_field.add_list_field(resource::definition::binary_template::type::PSTR, "String");
        tmpl.add_field(strings_binary_field);
        string_list->set_binary_template(tmpl);

        resource::definition::type::field strings_field("Str");
        strings_field.make_repeatable(0, 32767);
        strings_field.repeatable().set_count_field(&string_list->binary_template()->field_named("Strings"));
        resource::definition::type::field_value strings_value(&string_list->binary_template()->field_named(std::vector<std::string>({ "Strings", "String" })));
        strings_field.add_value(strings_value);
        string_list->add_field(strings_field);
    }

    static inline auto construct_string_resource(sema::context& ctx) -> void
    {
        resource::definition::type::instance string("StringResource", "STR ");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "String");
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        string.set_binary_template(tmpl);

        resource::definition::type::field string_field("Str");
        resource::definition::type::field_value string_value(&string.binary_template()->field_named("String"));
        string_value.set_type(resource::definition::type::descriptor(false, spec::types::file), true);
        string_field.add_value(string_value);
        string.add_field(string_field);

        ctx.register_type(string);
    }

    static inline auto construct_picture(sema::context& ctx) -> void
    {
        resource::definition::type::instance pict("Picture", "PICT");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        pict.set_binary_template(tmpl);

        resource::definition::type::field png_field("PNG");
        resource::definition::type::field_value png_value(&pict.binary_template()->field_named("Data"));
        png_value.set_type(resource::definition::type::descriptor(false, spec::types::image, std::vector<std::string>({
            image::conversion::format::png, image::conversion::format::pict
        })), true);
        png_field.add_value(png_value);
        pict.add_field(png_field);

        resource::definition::type::field tga_field("TGA");
        resource::definition::type::field_value tga_value(&pict.binary_template()->field_named("Data"));
        tga_value.set_type(resource::definition::type::descriptor(false, spec::types::image, std::vector<std::string>({
            image::conversion::format::tga, image::conversion::format::pict
        })), true);
        tga_field.add_value(tga_value);
        pict.add_field(tga_field);

        ctx.register_type(pict);
    }

    static inline auto construct_color_icon(sema::context& ctx) -> void
    {
        resource::definition::type::instance cicn("ColorIcon", "cicn");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        cicn.set_binary_template(tmpl);

        resource::definition::type::field png_field("PNG");
        resource::definition::type::field_value png_value(&cicn.binary_template()->field_named("Data"));
        png_value.set_type(resource::definition::type::descriptor(false, spec::types::image, std::vector<std::string>({
            image::conversion::format::png, image::conversion::format::cicn
        })), true);
        png_field.add_value(png_value);
        cicn.add_field(png_field);

        resource::definition::type::field tga_field("TGA");
        resource::definition::type::field_value tga_value(&cicn.binary_template()->field_named("Data"));
        tga_value.set_type(resource::definition::type::descriptor(false, spec::types::image, std::vector<std::string>({
            image::conversion::format::tga, image::conversion::format::cicn
        })), true);
        tga_field.add_value(tga_value);
        cicn.add_field(tga_field);

        ctx.register_type(cicn);
    }

    static inline auto construct_sound(sema::context& ctx) -> void
    {
        resource::definition::type::instance snd("Sound", "snd ");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        snd.set_binary_template(tmpl);

        resource::definition::type::field wav_field("WAV");
        resource::definition::type::field_value wav_value(&snd.binary_template()->field_named("Data"));
        wav_value.set_type(resource::definition::type::descriptor(false, spec::types::sound, std::vector<std::string>({
            sound::conversion::format::wav, sound::conversion::format::snd
        })), true);
        wav_field.add_value(wav_value);
        snd.add_field(wav_field);

        resource::definition::type::field snd_field("Sound");
        resource::definition::type::field_value snd_value(&snd.binary_template()->field_named("Data"));
        snd_value.set_type(resource::definition::type::descriptor(false, spec::types::sound, std::vector<std::string>({
            sound::conversion::format::snd, sound::conversion::format::snd
        })), true);
        snd_field.add_value(snd_value);
        snd.add_field(snd_field);

        ctx.register_type(snd);
    }
}

// MARK: -

auto kdl::modules::macintosh::construct(sema::context& ctx) -> void
{
    construct_string_list(ctx);
    construct_string_resource(ctx);
    construct_picture(ctx);
    construct_color_icon(ctx);
    construct_sound(ctx);
}
