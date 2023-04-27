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
    return stream.expect_any({
        expectation(tokenizer::scene_keyword).be_true(),
        expectation(tokenizer::dialog_keyword).be_true()
    });
}

auto kdl::sema::scene::parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void
{
    bool is_dialog = false;
    if (stream.expect({ expectation(tokenizer::dialog_keyword).be_true() })) {
        stream.read();
        is_dialog = true;
    }
    else {
        stream.ensure({ expectation(tokenizer::scene_keyword).be_true() });
    }

    resource::reference id(0);
    bool infer_container_from_name = true;
    if (stream.expect({ expectation(tokenizer::l_angle).be_true() })) {
        auto id_token = stream.read(1);
        infer_container_from_name = false;
        if (id_token.is(tokenizer::identifier)) {
            id = id.with_container(id_token.string_value());
        }
        else {
            id = id_token.reference_value();
            infer_container_from_name = !id.has_container();
        }
        stream.ensure({ expectation(tokenizer::r_angle).be_true() });
    }

    auto scene_name = stream.read();
    if (infer_container_from_name) {
        id = id.with_container(scene_name.string_value());
    }
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    // Setup the scene instance.
    auto ref = id.with_type_name("SceneInterface", "scïn");
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
            scope->add_variable("IsDialog", 0x0040LL);

            if (is_dialog) {
                stream.push({ { "IsDialog", tokenizer::identifier}, { "|", tokenizer::pipe } });
            }

            parse_value("Flags", interpreter::token::integer, stream, ctx, scope, scene);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Size").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("SceneWidth", interpreter::token::integer, stream, ctx, nullptr, scene);
            stream.ensure({ expectation(tokenizer::comma).be_true() });
            parse_value("SceneHeight", interpreter::token::integer, stream, ctx, nullptr, scene);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Background").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            if (stream.expect({ expectation(tokenizer::l_brace).be_true() })) {
                stream.advance();
                scene.set_value("BackgroundType", resource::value_container(static_cast<std::uint8_t>(1)));
                while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
                    if (stream.expect({ expectation(tokenizer::identifier, "Top").be_true() })) {
                        stream.advance(); stream.ensure({ expectation(tokenizer::equals).be_true() });
                        parse_value("BackgroundTop", interpreter::token::reference, stream, ctx, nullptr, scene);
                    }
                    else if (stream.expect({ expectation(tokenizer::identifier, "Bottom").be_true() })) {
                        stream.advance(); stream.ensure({ expectation(tokenizer::equals).be_true() });
                        parse_value("BackgroundBottom", interpreter::token::reference, stream, ctx, nullptr, scene);
                    }
                    else if (stream.expect({ expectation(tokenizer::identifier, "Fill").be_true() })) {
                        stream.advance(); stream.ensure({ expectation(tokenizer::equals).be_true() });
                        parse_value("Background", interpreter::token::reference, stream, ctx, nullptr, scene);
                    }
                    else {
                        throw std::runtime_error("");
                    }
                    stream.ensure({ expectation(tokenizer::semi).be_true() });
                }
                stream.ensure({ expectation(tokenizer::r_brace).be_true() });
            }
            else {
                scene.set_value("BackgroundType", resource::value_container(static_cast<std::uint8_t>(0)));
                parse_value("Background", interpreter::token::reference, stream, ctx, nullptr, scene);
            }
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
        scene_instance.set_value(name, resource::value_container(result.value.reference_value()));
    }
    else {
        throw std::runtime_error("");
    }
}

auto kdl::sema::scene::parse_element(foundation::stream<tokenizer::token> &stream, context &ctx, resource::instance& scene_instance, std::uint16_t& element) -> void
{
    element++;
    scene_instance.set_value("Elements" + std::to_string(element), resource::value_container(element));

    // Setup some helper functions in the scope.
    auto scope = ctx.create_scope();
    scope->add_function({"rgba", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        std::int64_t value = static_cast<std::uint8_t>(arguments.at(0).integer_value()) << 16;
        value |= static_cast<std::uint8_t>(arguments.at(1).integer_value()) << 8;
        value |= static_cast<std::uint8_t>(arguments.at(2).integer_value());
        value |= static_cast<std::uint8_t>(arguments.at(3).integer_value()) << 24;
        return interpreter::token(value);
    }});

    scope->add_function({"rgb", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        std::int64_t value = static_cast<std::uint8_t>(arguments.at(0).integer_value()) << 16;
        value |= static_cast<std::uint8_t>(arguments.at(1).integer_value()) << 8;
        value |= static_cast<std::uint8_t>(arguments.at(2).integer_value());
        value |= 0xFF000000;
        return interpreter::token(value);
    }});

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

            scope->add_function({ "Value", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
                std::string result = "%REF:" + std::to_string(args.size() - 1) + ":";

                // Encode the reference into the string.
                auto reference = args[0].reference_value();
                if (reference.has_container()) {
                    result += "C=" + reference.container_name() + ",";
                }
                if (reference.has_type_name()) {
                    if (auto type = ctx.type_named(reference.type_name())) {
                        result += "T='" + type->code() + "',";
                    }
                }
                result += std::to_string(reference.id());

                // If there are more fields, then encode those.
                for (auto i = 1; i < args.size(); ++i) {
                    auto& v = args[i];
                    if (v.is(interpreter::token::string)) {
                        result += ":f=" + v.string_value();
                    }
                    else if (v.is(interpreter::token::integer)) {
                        result += ":#=" + std::to_string(v.integer_value());
                    }
                }

                return interpreter::token(result);
            }});

            parse_value("ElementValue" + std::to_string(element), interpreter::token::string, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Action").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });

            scope->add_function({ "Function", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
                std::string result = "FUNCTION:" + args.at(0).string_value();
                return interpreter::token(result);
            }});
            scope->add_function({ "Lua", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
                std::string result = "LUA:" + args.at(0).string_value();
                return interpreter::token(result);
            }});
            scope->add_function({ "Push", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
                std::string result = "PUSH:" + args.at(0).string_value();
                return interpreter::token(result);
            }});

            parse_value("ElementAction" + std::to_string(element), interpreter::token::string, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Y").be_true() }) && element_type.is("Position")) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementY" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Height").be_true() }) && element_type.is("Spacer")) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementHeight" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "AxisOrigin").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });

            scope->add_variable("TopLeft", 0x00LL);
            scope->add_variable("TopCenter", 0x01LL);
            scope->add_variable("TopRight", 0x02LL);
            scope->add_variable("MiddleLeft", 0x10LL);
            scope->add_variable("Center", 0x11LL);
            scope->add_variable("MiddleRight", 0x12LL);
            scope->add_variable("BottomLeft", 0x20LL);
            scope->add_variable("BottomCenter", 0x21LL);
            scope->add_variable("BottomRight", 0x22LL);

            parse_value("ElementAxisOrigin" + std::to_string(element), interpreter::token::integer, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "BackgroundColor").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementBackgroundColor" + std::to_string(element), interpreter::token::integer, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Color").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementColor" + std::to_string(element), interpreter::token::integer, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "BorderColor").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementBorderColor" + std::to_string(element), interpreter::token::integer, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "SelectionColor").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementSelectionColor" + std::to_string(element), interpreter::token::integer, stream, ctx, scope, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Font").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementFont" + std::to_string(element), interpreter::token::string, stream, ctx, nullptr, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "FontSize").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });
            parse_value("ElementFontSize" + std::to_string(element), interpreter::token::integer, stream, ctx, nullptr, scene_instance);
        }
        else if (stream.expect({ expectation(tokenizer::identifier, "Alignment").be_true() })) {
            stream.advance();
            stream.ensure({ expectation(tokenizer::equals).be_true() });

            scope->add_variable("Left", 0x00LL);
            scope->add_variable("Center", 0x01LL);
            scope->add_variable("Right", 0x02LL);

            parse_value("ElementAlignment" + std::to_string(element), interpreter::token::integer, stream, ctx, scope, scene_instance);
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
}