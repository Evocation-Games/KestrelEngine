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

#include <libKDL/modules/spriteworld/spriteworld.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/definition/type/field.hpp>
#include <libResource/definition/template/instance.hpp>
#include <libKDL/spec/types.hpp>
#include <libImage/conversion/conversion.hpp>

// MARK: -

namespace kdl::modules::spriteworld
{
    static inline auto construct_sprite_set(sema::context& ctx) -> void
    {
        resource::definition::type::instance sprite_set("SpriteWorldSprite", "rlëD");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        sprite_set.set_binary_template(tmpl);

        resource::definition::type::field png_field("PNG");
        resource::definition::type::field_value png_value(&sprite_set.binary_template()->field_named("Data"));
        png_value.set_type(resource::definition::type::descriptor(false, spec::types::image_set, std::vector<std::string>({
            image::conversion::format::png, image::conversion::format::rled
        })), true);
        png_field.add_value(png_value);
        sprite_set.add_field(png_field);

        resource::definition::type::field tga_field("TGA");
        resource::definition::type::field_value tga_value(&sprite_set.binary_template()->field_named("Data"));
        tga_value.set_type(resource::definition::type::descriptor(false, spec::types::image_set, std::vector<std::string>({
            image::conversion::format::tga, image::conversion::format::rled
        })), true);
        tga_field.add_value(tga_value);
        sprite_set.add_field(tga_field);

        ctx.register_type(sprite_set);
    }
}

// MARK: -

auto kdl::modules::spriteworld::construct(sema::context& ctx) -> void
{
    construct_sprite_set(ctx);
}
