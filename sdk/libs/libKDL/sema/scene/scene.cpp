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

#include <libKDL/sema/scene/scene.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libResource/declaration/instance.hpp>
#include <libKDL/sema/script/script.hpp>

auto kdl::sema::scene::test(const foundation::stream<tokenizer::token>& stream) -> bool
{
    return stream.expect({
        expectation(tokenizer::scene_keyword).be_true(),
        expectation(tokenizer::l_angle).be_true(),
        expectation(tokenizer::reference).be_true(),
        expectation(tokenizer::r_angle).be_true(),
        expectation(tokenizer::identifier).be_true(),
        expectation(tokenizer::l_brace).be_true()
    });
}

auto kdl::sema::scene::parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void
{
    stream.ensure({ expectation(tokenizer::scene_keyword).be_true(), expectation(tokenizer::l_angle).be_true() });
    auto id = stream.read();
    stream.ensure({ expectation(tokenizer::r_angle).be_true() });
    auto scene_name = stream.read();
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    // Setup the scene instance.
    auto ref = id.reference_value()
        .with_container(scene_name.string_value())
        .with_type_name("SceneInterface");
    resource::instance scene(ref);
    scene.set_name(scene_name.string_value());
    std::uint16_t element_count = 0;

    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (stream.expect({ expectation(tokenizer::identifier, "Title").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("Title", interpreter::token::string, stream, ctx, nullptr, scene);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Flags").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });

            auto scope = ctx.create_scope();
            scope->add_variable("UseImGui", 0x0001LL);
            scope->add_variable("ImGuiShowTitle", 0x0002LL);
            scope->add_variable("ImGuiCloseButton", 0x0004LL);
            scope->add_variable("ScenePassthrough", 0x0010LL);
            scope->add_variable("VerticalFlowLayout", 0x0020LL);

            parse_value("Flags", interpreter::token::integer, stream, ctx, scope, scene);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Size").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("SceneWidth", interpreter::token::integer, stream, ctx, nullptr, scene);
            stream.ensure({ expectation(tokenizer::comma).be_true() });
            parse_value("SceneHeight", interpreter::token::integer, stream, ctx, nullptr, scene);
        }
        else if (stream.expect({
            expectation(tokenizer::identifier, {
                "Button", "Label", "TextArea", "Image", "TextField", "Checkbox",
                "List", "ScrollArea", "Grid", "LabeledList", "Canvas", "Sprite",
                "PopupButton", "Slider", "Table", "Box", "Radio", "TabBar", "Separator",
                "Spacer", "Position"
            }).be_true(),
            expectation(tokenizer::l_paren).be_true(),
            expectation(tokenizer::string).be_true(),
            expectation(tokenizer::r_paren).be_true(),
            expectation(tokenizer::l_brace).be_true()
        })) {
            parse_element(stream, ctx, scene, element_count);
        }
        else {
            throw std::runtime_error("");
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }

    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
    if (!ctx.flags.surpress_resource_creation) {
        ctx.resources.emplace_back(std::move(scene));
    }
}

// MARK: - Element Parsing

auto kdl::sema::scene::parse_value(
    const std::string &name,
    enum interpreter::token::type expected_type,
    foundation::stream<tokenizer::token> &stream,
    sema::context& ctx,
    interpreter::scope *scope,
    resource::instance &scene_instance
)
    -> void
{
    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.value.is(interpreter::token::string) && result.value.is(expected_type)) {
        scene_instance.set_value(name, resource::value_container(result.value.string_value()));
    }
    else if (result.value.is(interpreter::token::integer) && result.value.is(expected_type)) {
        scene_instance.set_value(name, resource::value_container(result.value.integer_value()));
    }
    else if (result.value.is(interpreter::token::reference) && result.value.is(expected_type)) {
        scene_instance.set_value(name, resource::value_container(resource::reference(result.value.integer_value())));
    }
    else {
        throw std::runtime_error("");
    }
}

auto kdl::sema::scene::parse_element(foundation::stream<tokenizer::token> &stream, context &ctx, resource::instance& scene_instance, std::uint16_t& element) -> void
{
    element++;
    scene_instance.set_value("Elements" + std::to_string(element), resource::value_container(element));

    std::vector<std::string> types({
        "None",
        "Button", "Label", "TextArea", "Image", "TextField", "Checkbox",
        "List", "ScrollArea", "Grid", "LabeledList", "Canvas", "Sprite",
        "PopupButton", "Slider", "Table", "Box", "Radio", "TabBar", "Separator",
        "Spacer", "Position"
    });
    auto element_type = stream.read();
    auto index = std::find(types.begin(), types.end(), element_type.string_value()) - types.begin();
    scene_instance.set_value("ElementType" + std::to_string(element), resource::value_container(index));

    stream.ensure({ expectation(tokenizer::l_paren).be_true() });
    parse_value("ElementId" + std::to_string(element), interpreter::token::string, stream, ctx, nullptr, scene_instance);

    stream.ensure({ expectation(tokenizer::r_paren).be_true(), expectation(tokenizer::l_brace).be_true() });
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        if (stream.expect({ expectation(tokenizer::identifier, "Frame").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementX" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
            stream.ensure({ expectation(tokenizer::comma).be_true() });
            parse_value("ElementY" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
            stream.ensure({ expectation(tokenizer::comma).be_true() });
            parse_value("ElementWidth" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
            stream.ensure({ expectation(tokenizer::comma).be_true() });
            parse_value("ElementHeight" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, { "Value", "Label", "Title" }).be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementValue" + std::to_string(element), interpreter::token::string, stream, ctx, nullptr, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Action").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementAction" + std::to_string(element), interpreter::token::string, stream, ctx, nullptr, scene_instance);
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}