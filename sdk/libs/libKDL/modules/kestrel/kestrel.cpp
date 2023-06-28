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

#include <libKDL/modules/kestrel/kestrel.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/definition/type/field.hpp>
#include <libResource/definition/template/instance.hpp>
#include <libKDL/spec/types.hpp>
#include <libImage/conversion/conversion.hpp>

// MARK: -

namespace kdl::modules::kestrel
{
    static inline auto construct_lua_script(sema::context& ctx) -> void
    {
        resource::definition::type::instance lua_script("LuaScript", "LuaS");
        lua_script.add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::LUA_BYTE_CODE, "Data");
        lua_script.set_binary_template(tmpl);

        resource::definition::type::field source_field("Source");
        resource::definition::type::field_value source_value(&lua_script.binary_template()->field_named("Data"));
        source_field.add_value(source_value);
        lua_script.add_field(source_field);

        ctx.register_type(lua_script);
    }

    static inline auto construct_glsl(sema::context& ctx) -> void
    {
        resource::definition::type::instance glsl_shader("GLSLShader", "glsl");
        glsl_shader.add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "VertexFunction");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "FragmentFunction");
        glsl_shader.set_binary_template(tmpl);

        resource::definition::type::field vertex_field("VertexFunction");
        resource::definition::type::field_value vertex_value(&glsl_shader.binary_template()->field_named("VertexFunction"));
        vertex_value.set_type(resource::definition::type::descriptor(spec::types::file), true);
        vertex_field.add_value(vertex_value);
        glsl_shader.add_field(vertex_field);

        resource::definition::type::field fragment_field("FragmentFunction");
        resource::definition::type::field_value fragment_value(&glsl_shader.binary_template()->field_named("FragmentFunction"));
        fragment_value.set_type(resource::definition::type::descriptor(spec::types::file), true);
        fragment_field.add_value(fragment_value);
        glsl_shader.add_field(fragment_field);

        ctx.register_type(glsl_shader);
    }

    static inline auto construct_mlsl(sema::context& ctx) -> void
    {
        resource::definition::type::instance mlsl_shader("MetalShader", "mlsl");
        mlsl_shader.add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "VertexFunction");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "FragmentFunction");
        mlsl_shader.set_binary_template(tmpl);

        resource::definition::type::field vertex_field("VertexFunction");
        resource::definition::type::field_value vertex_value(&mlsl_shader.binary_template()->field_named("VertexFunction"));
        vertex_value.set_type(resource::definition::type::descriptor(spec::types::file), true);
        vertex_field.add_value(vertex_value);
        mlsl_shader.add_field(vertex_field);

        resource::definition::type::field fragment_field("FragmentFunction");
        resource::definition::type::field_value fragment_value(&mlsl_shader.binary_template()->field_named("FragmentFunction"));
        fragment_value.set_type(resource::definition::type::descriptor(spec::types::file), true);
        fragment_field.add_value(fragment_value);
        mlsl_shader.add_field(fragment_field);

        ctx.register_type(mlsl_shader);
    }

    static inline auto construct_shader_set(sema::context& ctx) -> void
    {
        resource::definition::type::instance shader_set("ShaderSet", "shdr");
        shader_set.add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::RSRC, "glsl");
        tmpl.add_field(resource::definition::binary_template::type::RSRC, "mlsl");
        shader_set.set_binary_template(tmpl);

        resource::definition::type::field glsl_field("GLSL");
        resource::definition::type::field_value glsl_value(&shader_set.binary_template()->field_named("glsl"));
        glsl_value.set_type(resource::definition::type::descriptor(true, "GLSLShader"), true);
        glsl_field.add_value(glsl_value);
        shader_set.add_field(glsl_field);

        resource::definition::type::field metal_field("Metal");
        resource::definition::type::field_value metal_value(&shader_set.binary_template()->field_named("mlsl"));
        metal_value.set_type(resource::definition::type::descriptor(true, "MetalShader"), true);
        metal_field.add_value(metal_value);
        shader_set.add_field(metal_field);

        ctx.register_type(shader_set);
    }

    static inline auto construct_sprite_set(sema::context& ctx) -> void
    {
        auto sprite_set = ctx.register_type(resource::definition::type::instance("SpriteSet", "rlëX"));
        sprite_set->add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        sprite_set->set_binary_template(tmpl);

        resource::definition::type::field png_field("PNG");
        resource::definition::type::field_value png_value(&sprite_set->binary_template()->field_named("Data"));
        png_value.set_type(resource::definition::type::descriptor(false, spec::types::image_set, std::vector<std::string>({
            image::conversion::format::png, image::conversion::format::rlex
        })), true);
        png_field.add_value(png_value);
        sprite_set->add_field(png_field);

        resource::definition::type::field tga_field("TGA");
        resource::definition::type::field_value tga_value(&sprite_set->binary_template()->field_named("Data"));
        tga_value.set_type(resource::definition::type::descriptor(false, spec::types::image_set, std::vector<std::string>({
            image::conversion::format::tga, image::conversion::format::rlex
        })), true);
        tga_field.add_value(tga_value);
        sprite_set->add_field(tga_field);

    }

    static inline auto construct_scene_interface(sema::context& ctx) -> void
    {
        auto scene_interface = ctx.register_type(resource::definition::type::instance("SceneInterface", "scïn"));
        scene_interface->add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        scene_interface->set_binary_template(tmpl);

        resource::definition::type::field data_field("Data");
        resource::definition::type::field_value data_value(&scene_interface->binary_template()->field_named("Data"));
        data_value.set_type(resource::definition::type::descriptor(false, spec::types::data), true);
        data_field.add_value(data_value);
        scene_interface->add_field(data_field);
    }

    static inline auto construct_scene_definition(sema::context& ctx) -> void
    {
        auto scene = ctx.register_type(resource::definition::type::instance("SceneDefinition", "scën"));
        ctx.root_scope()->add_variable("SceneDefinitionID", static_cast<std::int64_t>(0));
        scene->add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::RSRC, "Script");
        tmpl.add_field(resource::definition::binary_template::type::RSRC, "Interface");
        tmpl.add_field(resource::definition::binary_template::type::RSRC, "DLOG");

        resource::definition::binary_template::field ditl_elements(resource::definition::binary_template::type::OCNT, "DITLElementNames");
        ditl_elements.add_list_field(resource::definition::binary_template::type::DWRD, "Index");
        ditl_elements.add_list_field(resource::definition::binary_template::type::PSTR, "Name");
        ditl_elements.add_list_field(resource::definition::binary_template::type::DWRD, "Type");
        tmpl.add_field(ditl_elements);
        scene->set_binary_template(tmpl);

        resource::definition::type::field script_field("Script");
        resource::definition::type::field_value script_value(&scene->binary_template()->field_named("Script"));
        script_value.set_type(resource::definition::type::descriptor(true, "LuaScript"), true);
        script_field.add_value(script_value);
        scene->add_field(script_field);

        resource::definition::type::field interface_field("Interface");
        resource::definition::type::field_value interface_value(&scene->binary_template()->field_named("Interface"));
        interface_value.set_type(resource::definition::type::descriptor(true, "SceneInterface"), true);
        interface_field.add_value(interface_value);
        scene->add_field(interface_field);

        resource::definition::type::field dlog_field("DLOG");
        resource::definition::type::field_value dlog_value(&scene->binary_template()->field_named("DLOG"));
        dlog_value.set_type(resource::definition::type::descriptor(true), true);
        dlog_field.add_value(dlog_value);
        scene->add_field(dlog_field);

        resource::definition::type::field elements_field("DITLElement");
        elements_field.make_repeatable(0, 32767);
        elements_field.repeatable().set_count_field(&scene->binary_template()->field_named("DITLElementNames"));
        resource::definition::type::field_value index_value(&scene->binary_template()->field_named(std::vector<std::string>({ "DITLElementNames", "Index" })));
        index_value.set_type(resource::definition::type::descriptor(false, spec::types::integer), false);
        elements_field.add_value(index_value);

        resource::definition::type::field_value name_value(&scene->binary_template()->field_named(std::vector<std::string>({ "DITLElementNames", "Name" })));
        name_value.set_type(resource::definition::type::descriptor(false, spec::types::string), false);
        elements_field.add_value(name_value);

        resource::definition::type::field_value type_value(&scene->binary_template()->field_named(std::vector<std::string>({ "DITLElementNames", "Type" })));
        type_value.set_type(resource::definition::type::descriptor(false, spec::types::integer), false);
        type_value.add_symbol("None", resource::value_container(0));
        type_value.add_symbol("Button", resource::value_container(1));
        type_value.add_symbol("Label", resource::value_container(2));
        type_value.add_symbol("TextArea", resource::value_container(3));
        type_value.add_symbol("Image", resource::value_container(4));
        type_value.add_symbol("TextField", resource::value_container(5));
        type_value.add_symbol("Checkbox", resource::value_container(6));
        type_value.add_symbol("List", resource::value_container(7));
        type_value.add_symbol("ScrollArea", resource::value_container(8));
        type_value.add_symbol("Grid", resource::value_container(9));
        type_value.add_symbol("LabeledList", resource::value_container(10));
        type_value.add_symbol("Canvas", resource::value_container(11));
        type_value.add_symbol("Sprite", resource::value_container(12));
        type_value.add_symbol("PopupButton", resource::value_container(13));
        type_value.add_symbol("Slider", resource::value_container(14));
        type_value.add_symbol("Table", resource::value_container(15));
        type_value.add_symbol("Box", resource::value_container(16));
        type_value.add_symbol("Radio", resource::value_container(17));
        type_value.add_symbol("TabBar", resource::value_container(18));
        type_value.add_symbol("Separator", resource::value_container(19));
        type_value.add_symbol("Spacer", resource::value_container(20));
        type_value.add_symbol("Position", resource::value_container(21));
        elements_field.add_value(type_value);

        scene->add_field(elements_field);
    }

    static inline auto construct_package(sema::context& ctx) -> void
    {
        resource::definition::type::instance package("KestrelPackage", "kmöd");
        package.add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "Name");
        tmpl.add_field(resource::definition::binary_template::type::Cnnn, 0x040, "Version");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "Author");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "PrimaryContainer");
        tmpl.add_field(resource::definition::binary_template::type::RSRC, "LuaEntryScript");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "Description");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "Category");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "PackageID");
        tmpl.add_field(resource::definition::binary_template::type::CSTR, "ScenarioID");
        package.set_binary_template(tmpl);

        resource::definition::type::field name_field("Name");
        resource::definition::type::field_value name_value(&package.binary_template()->field_named("Name"));
        name_field.add_value(name_value);

        resource::definition::type::field version_field("Version");
        resource::definition::type::field_value version_value(&package.binary_template()->field_named("Version"));
        version_field.add_value(version_value);

        resource::definition::type::field author_field("Author");
        resource::definition::type::field_value author_value(&package.binary_template()->field_named("Author"));
        author_field.add_value(author_value);

        resource::definition::type::field primary_container_field("PrimaryContainer");
        resource::definition::type::field_value primary_container_value(&package.binary_template()->field_named("PrimaryContainer"));
        primary_container_field.add_value(primary_container_value);

        resource::definition::type::field lua_entry_script_field("LuaEntryScript");
        resource::definition::type::field_value lua_entry_script_value(&package.binary_template()->field_named("LuaEntryScript"));
        lua_entry_script_field.add_value(lua_entry_script_value);

        resource::definition::type::field description_field("Description");
        resource::definition::type::field_value description_value(&package.binary_template()->field_named("Description"));
        description_field.add_value(description_value);

        resource::definition::type::field category_field("Category");
        resource::definition::type::field_value category_value(&package.binary_template()->field_named("Category"));
        category_field.add_value(category_value);

        resource::definition::type::field package_id_field("PackageID");
        resource::definition::type::field_value package_id_value(&package.binary_template()->field_named("PackageID"));
        package_id_field.add_value(package_id_value);

        resource::definition::type::field scenario_id_field("ScenarioID");
        resource::definition::type::field_value scenario_id_value(&package.binary_template()->field_named("ScenarioID"));
        scenario_id_field.add_value(scenario_id_value);

        package.add_field(name_field);
        package.add_field(version_field);
        package.add_field(author_field);
        package.add_field(primary_container_field);
        package.add_field(lua_entry_script_field);
        package.add_field(description_field);
        package.add_field(category_field);
        package.add_field(package_id_field);
        package.add_field(scenario_id_field);

        ctx.register_type(package);
    }

    static inline auto construct_static_image(sema::context& ctx) -> void
    {
        auto static_image = ctx.register_type(resource::definition::type::instance("StaticImage", "sïmg"));
        static_image->add_decorator("__builtin", "");

        resource::definition::binary_template::instance tmpl;
        tmpl.add_field(resource::definition::binary_template::type::Cnnn, 0x004, "ImageFormat");
        tmpl.add_field(resource::definition::binary_template::type::HEXD, "Data");
        static_image->set_binary_template(tmpl);

        resource::definition::type::field png_field("PNG");
        resource::definition::type::field_value png_value(&static_image->binary_template()->field_named("Data"));
        png_value.set_type(resource::definition::type::descriptor(false, spec::types::image, std::vector<std::string>({
            image::conversion::format::png, image::conversion::format::tga
        })), true);
        png_field.add_value(png_value);

        resource::definition::type::field_value png_format_value(&static_image->binary_template()->field_named("ImageFormat"));
        png_format_value.set_type(resource::definition::type::descriptor(false, spec::types::string, {}), true);
        png_format_value.set_default_value(interpreter::script::statement({
            interpreter::token(std::string("TGA "))
        }));
        png_field.add_value(png_format_value);
        static_image->add_field(png_field);

        resource::definition::type::field tga_field("TGA");
        resource::definition::type::field_value tga_value(&static_image->binary_template()->field_named("Data"));
        tga_value.set_type(resource::definition::type::descriptor(false, spec::types::image, std::vector<std::string>({
            image::conversion::format::tga, image::conversion::format::tga
        })), true);
        tga_field.add_value(tga_value);

        resource::definition::type::field_value tga_format_value(&static_image->binary_template()->field_named("ImageFormat"));
        tga_format_value.set_type(resource::definition::type::descriptor(false, spec::types::string, {}), true);
        tga_format_value.set_default_value(interpreter::script::statement({
            interpreter::token(std::string("TGA "))
        }));
        tga_field.add_value(tga_format_value);

        static_image->add_field(tga_field);
    }
}

// MARK: -

auto kdl::modules::kestrel::construct(sema::context& ctx) -> void
{
    construct_lua_script(ctx);
    construct_glsl(ctx);
    construct_mlsl(ctx);
    construct_shader_set(ctx);
    construct_sprite_set(ctx);
    construct_scene_interface(ctx);
    construct_scene_definition(ctx);
    construct_package(ctx);
    construct_static_image(ctx);
}
