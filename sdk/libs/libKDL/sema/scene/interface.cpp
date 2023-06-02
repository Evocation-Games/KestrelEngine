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

#include <libKDL/sema/scene/interface.hpp>
#include <libKDL/sema/expectation/expectation.hpp>
#include <libKDL/sema/script/script.hpp>
#include <libUI/format/interface/encoder.hpp>
#include <libUI/format/interface/axis_origin.hpp>

auto kdl::sema::scene_interface::test(const foundation::stream<tokenizer::token> &stream) -> bool
{
    return stream.expect_any({
        expectation(tokenizer::scene_keyword).be_true(),
        expectation(tokenizer::dialog_keyword).be_true()
    });
}

auto kdl::sema::scene_interface::parse(foundation::stream<tokenizer::token> &stream, kdl::sema::context &ctx) -> void
{
    ui::format::interface an_interface;

    if (stream.expect({ expectation(tokenizer::dialog_keyword).be_true() })) {
        stream.advance();
        an_interface.add_flag(ui::format::interface::dialog);
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

    an_interface.set_name(stream.read().string_value());
    const auto name = an_interface.name();
    if (infer_container_from_name) {
        id = id.with_container(an_interface.name());
    }

    stream.ensure({ expectation(tokenizer::l_brace).be_true() });
    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        auto scope = ctx.create_scope();

        if (stream.expect({ expectation(interface_fields::title).be_true(), expectation(tokenizer::equals).be_true() })) {
            stream.advance(2);
            an_interface.set_name(parse_string_value(stream, ctx));
        }
        else if (stream.expect({ expectation(interface_fields::flags).be_true(), expectation(tokenizer::equals).be_true() })) {
            stream.advance(2);
            scope->add_variable("UseImGui", static_cast<std::int64_t>(ui::format::interface::flags::imgui));
            scope->add_variable("ShowsTitleBar", static_cast<std::int64_t>(ui::format::interface::flags::shows_title_bar));
            scope->add_variable("ShowsCloseButton", static_cast<std::int64_t>(ui::format::interface::flags::shows_close_button));
            scope->add_variable("Passthrough", static_cast<std::int64_t>(ui::format::interface::flags::passthrough));
            scope->add_variable("VerticalFlowLayout", static_cast<std::int64_t>(ui::format::interface::flags::vertical_flow_layout));
            scope->add_variable("IsDialog", static_cast<std::int64_t>(ui::format::interface::flags::dialog));
            scope->add_variable("Centered", static_cast<std::int64_t>(ui::format::interface::flags::centered));
            an_interface.set_flags(static_cast<std::uint16_t>(parse_unsigned_integer_value(stream, ctx, scope)));
        }
        else if (stream.expect({ expectation(interface_fields::size).be_true(), expectation(tokenizer::equals).be_true() })) {
            stream.advance(2);
            an_interface.set_frame(parse_rect_value(stream, ctx, scope));
            an_interface.add_flag(ui::format::interface::flags::centered);
            an_interface.add_flag(ui::format::interface::flags::windowed_scene);
        }
        else if (stream.expect({ expectation(interface_fields::frame).be_true(), expectation(tokenizer::equals).be_true() })) {
            stream.advance(2);
            an_interface.set_frame(parse_rect_value(stream, ctx));
            an_interface.add_flag(ui::format::interface::flags::windowed_scene);
        }
        else if (stream.expect({ expectation(interface_fields::background).be_true(), expectation(tokenizer::equals).be_true() })) {
            stream.advance(2);
            an_interface.set_background(parse_background_value(stream, ctx));
        }

        // Elements
        else if (stream.expect({
            expectation({
                interface_elements::button,
                interface_elements::label,
                interface_elements::checkbox,
                interface_elements::popup,
                interface_elements::canvas,
                interface_elements::image,
                interface_elements::sprite,
                interface_elements::text,
                interface_elements::textarea,
                interface_elements::grid,
                interface_elements::list,
                interface_elements::separator,
                interface_elements::slider,
                interface_elements::radio,
                interface_elements::scroll_area,
                interface_elements::timed_trigger,
                interface_elements::key_trigger,
                interface_elements::vertical_spacer,
                interface_elements::horizontal_spacer,
                interface_elements::position,
            }).be_true(),
            expectation(tokenizer::l_paren).be_true(),
            expectation(tokenizer::string).be_true(),
            expectation(tokenizer::r_paren).be_true()
        })) {
            auto element_type = parse_element_type(stream);
            auto element_name = stream.read(1);
            stream.advance(1);
            auto element = parse_element(stream, ctx, element_type, element_name.string_value());
            an_interface.add_element(element);
        }

        // Unknowns
        else {
            throw std::runtime_error("");
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
        ctx.pop_scope();
    }
    stream.ensure({ expectation(tokenizer::r_brace).be_true() });

    // Ready to encode and produce the resource.
    auto scene_ref = id.with_type_name("SceneInterface", "scïn");
    resource::instance scene(scene_ref);
    scene.set_name(name);
    scene.set_value("Data", resource::value_container(
        ui::format::encode(an_interface)
    ));

    if (!ctx.flags.surpress_resource_creation) {
        ctx.resources.emplace_back(std::move(scene));
    }
}

// MARK: - Element Parser

auto kdl::sema::scene_interface::parse_element_type(foundation::stream<tokenizer::token> &stream) -> enum ui::format::element::type
{
    std::unordered_map<std::string, enum ui::format::element::type> types {
        std::pair(interface_elements::button, ui::format::element::type::button),
        std::pair(interface_elements::label, ui::format::element::type::label),
        std::pair(interface_elements::checkbox, ui::format::element::type::checkbox),
        std::pair(interface_elements::popup, ui::format::element::type::popup_button),
        std::pair(interface_elements::canvas, ui::format::element::type::canvas),
        std::pair(interface_elements::image, ui::format::element::type::image),
        std::pair(interface_elements::sprite, ui::format::element::type::sprite),
        std::pair(interface_elements::text, ui::format::element::type::text_field),
        std::pair(interface_elements::textarea, ui::format::element::type::text_area),
        std::pair(interface_elements::grid, ui::format::element::type::grid),
        std::pair(interface_elements::list, ui::format::element::type::list),
        std::pair(interface_elements::separator, ui::format::element::type::separator),
        std::pair(interface_elements::slider, ui::format::element::type::slider),
        std::pair(interface_elements::radio, ui::format::element::type::radio_button),
        std::pair(interface_elements::scroll_area, ui::format::element::type::scroll_area),
        std::pair(interface_elements::timed_trigger, ui::format::element::type::timed_trigger),
        std::pair(interface_elements::key_trigger, ui::format::element::type::key_trigger),
        std::pair(interface_elements::vertical_spacer, ui::format::element::type::vertical_spacer),
        std::pair(interface_elements::horizontal_spacer, ui::format::element::type::horizontal_spacer),
        std::pair(interface_elements::position, ui::format::element::type::position),
    };
    return types.find(stream.read().string_value())->second;
}

auto kdl::sema::scene_interface::parse_element(foundation::stream<tokenizer::token> &stream, sema::context &ctx, enum ui::format::element::type type, const std::string &name) -> ui::format::element
{
    ui::format::element element(type);
    element.add_attribute(ui::format::attribute::code::name, attribute_names::id, ui::format::attribute_value(name));
    stream.ensure({ expectation(tokenizer::l_brace).be_true() });

    while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
        auto scope = ctx.create_scope();

        if (stream.expect({ expectation(attribute_names::id).be_true(), expectation(tokenizer::equals).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::name, attribute_names::id, ui::format::attribute_value(
                parse_string_value(stream, ctx)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::frame).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::frame, attribute_names::frame, ui::format::attribute_value(
                parse_rect_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::x).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::x, attribute_names::x, ui::format::attribute_value(
                parse_integer_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::y).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::y, attribute_names::y, ui::format::attribute_value(
                parse_integer_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::size).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::size, attribute_names::size, ui::format::attribute_value(
                parse_integer_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::axis_origin).be_true() })) {
            stream.advance(2);
            scope->add_variable(axis_origin::top_left, static_cast<std::int64_t>(::ui::format::axis_origin::top_left));
            scope->add_variable(axis_origin::top_center, static_cast<std::int64_t>(::ui::format::axis_origin::top_center));
            scope->add_variable(axis_origin::top_right, static_cast<std::int64_t>(::ui::format::axis_origin::top_right));
            scope->add_variable(axis_origin::middle_left, static_cast<std::int64_t>(::ui::format::axis_origin::middle_left));
            scope->add_variable(axis_origin::center, static_cast<std::int64_t>(::ui::format::axis_origin::center));
            scope->add_variable(axis_origin::middle_right, static_cast<std::int64_t>(::ui::format::axis_origin::middle_right));
            scope->add_variable(axis_origin::bottom_left, static_cast<std::int64_t>(::ui::format::axis_origin::bottom_left));
            scope->add_variable(axis_origin::bottom_center, static_cast<std::int64_t>(::ui::format::axis_origin::bottom_center));
            scope->add_variable(axis_origin::bottom_right, static_cast<std::int64_t>(::ui::format::axis_origin::bottom_right));
            element.add_attribute(ui::format::attribute::code::axis_origin, attribute_names::axis_origin, ui::format::attribute_value(
                parse_integer_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::color).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::color, attribute_names::color, ui::format::attribute_value(
                parse_color_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::background_color).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::background_color, attribute_names::background_color, ui::format::attribute_value(
                parse_color_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::selection_color).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::selection_color, attribute_names::selection_color, ui::format::attribute_value(
                parse_color_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::border_color).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::border_color, attribute_names::border_color, ui::format::attribute_value(
                parse_color_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::secondary_color).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::secondary_color, attribute_names::secondary_color, ui::format::attribute_value(
                parse_color_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::font).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::font, attribute_names::font, ui::format::attribute_value(
                parse_string_value(stream, ctx)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::font_size).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::font_size, attribute_names::font_size, ui::format::attribute_value(
                parse_integer_value(stream, ctx)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::hidden).be_true() })) {
            stream.advance(2);
            scope->add_variable(boolean_names::yes, true);
            scope->add_variable(boolean_names::True, true);
            scope->add_variable(boolean_names::no, false);
            scope->add_variable(boolean_names::False, false);
            element.add_attribute(ui::format::attribute::code::hidden, attribute_names::hidden, ui::format::attribute_value(
                parse_bool_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::disabled).be_true() })) {
            stream.advance(2);
            scope->add_variable(boolean_names::yes, true);
            scope->add_variable(boolean_names::True, true);
            scope->add_variable(boolean_names::no, false);
            scope->add_variable(boolean_names::False, false);
            element.add_attribute(ui::format::attribute::code::disabled, attribute_names::disabled, ui::format::attribute_value(
                parse_bool_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::weight).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::weight, attribute_names::weight, ui::format::attribute_value(
                parse_integer_value(stream, ctx)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::horizontal_alignment).be_true() })) {
            stream.advance(2);
            scope->add_variable(horizontal_alignment_names::left, static_cast<std::int64_t>(-1));
            scope->add_variable(horizontal_alignment_names::center, static_cast<std::int64_t>(0));
            scope->add_variable(horizontal_alignment_names::right, static_cast<std::int64_t>(1));
            element.add_attribute(ui::format::attribute::code::horizontal_alignment, attribute_names::horizontal_alignment, ui::format::attribute_value(
                parse_integer_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::vertical_alignment).be_true() })) {
            stream.advance(2);
            scope->add_variable(vertical_alignment_names::top, static_cast<std::int64_t>(-1));
            scope->add_variable(vertical_alignment_names::middle, static_cast<std::int64_t>(0));
            scope->add_variable(vertical_alignment_names::bottom, static_cast<std::int64_t>(1));
            element.add_attribute(ui::format::attribute::code::vertical_alignment, attribute_names::vertical_alignment, ui::format::attribute_value(
                parse_integer_value(stream, ctx, scope)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::action).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::action, attribute_names::action, ui::format::attribute_value(
                parse_action(stream, ctx)
            ));
        }
        else if (stream.expect({ expectation(attribute_names::value).be_true() })) {
            stream.advance(2);
            element.add_attribute(ui::format::attribute::code::value, attribute_names::value, ui::format::attribute_value(
                parse_value(stream, ctx)
            ));
        }
        else {
            throw std::runtime_error("");
        }

        stream.ensure({ expectation(tokenizer::semi).be_true() });
        ctx.pop_scope();
    }

    stream.ensure({ expectation(tokenizer::r_brace).be_true() });
    return element;
}

// MARK: - Value Parsers

auto kdl::sema::scene_interface::parse_reference_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope) -> resource::reference
{
    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::reference)) {
        throw std::runtime_error("");
    }

    return result.value.reference_value();
}

auto kdl::sema::scene_interface::parse_string_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope) -> std::string
{
    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::string)) {
        throw std::runtime_error("");
    }

    return result.value.string_value();
}

auto kdl::sema::scene_interface::parse_integer_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope) -> std::int64_t
{
    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::integer)) {
        throw std::runtime_error("");
    }

    return result.value.integer_value();
}

auto kdl::sema::scene_interface::parse_unsigned_integer_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope) -> std::uint64_t
{
    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::integer)) {
        throw std::runtime_error("");
    }

    return static_cast<std::uint64_t>(result.value.integer_value());
}

auto kdl::sema::scene_interface::parse_rect_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope) -> ui::rect
{
    // Result
    ui::rect frame(0, 0, 0, 0);

    // Install Functions
    scope->add_function({ "Frame", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        frame = {
            static_cast<std::int16_t>(args[0].integer_value()),
            static_cast<std::int16_t>(args[1].integer_value()),
            static_cast<std::int16_t>(args[2].integer_value()),
            static_cast<std::int16_t>(args[3].integer_value())
        };
        return interpreter::token(true);
    }});

    scope->add_function({ "FrameSize", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        frame = {
            0, 0,
            static_cast<std::int16_t>(args[0].integer_value()),
            static_cast<std::int16_t>(args[1].integer_value())
        };
        return interpreter::token(true);
    }});

    scope->add_function({ "Rect", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        frame = {
            static_cast<std::int16_t>(args[0].integer_value()),
            static_cast<std::int16_t>(args[1].integer_value()),
            static_cast<std::int16_t>(args[2].integer_value() - args[0].integer_value()),
            static_cast<std::int16_t>(args[3].integer_value() - args[1].integer_value())
        };
        return interpreter::token(true);
    }});

    scope->add_variable("FlexibleHeight", static_cast<std::int64_t>(-1));

    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::boolean)) {
        throw std::runtime_error("");
    }

    if (result.value.is(false)) {
        throw std::runtime_error("");
    }

    return frame;
}

auto kdl::sema::scene_interface::parse_background_value(foundation::stream<tokenizer::token> &stream, sema::context &ctx, interpreter::scope *scope) -> ui::format::background
{
    if (stream.expect({ expectation(tokenizer::l_brace).be_true() })) {
        resource::reference top;
        resource::reference fill;
        resource::reference bottom;

        stream.advance();
        while (stream.expect({ expectation(tokenizer::r_brace).be_false() })) {
            if (stream.expect({ expectation("Top").be_true(), expectation(tokenizer::equals).be_true() })) {
                stream.advance(2);
                top = parse_reference_value(stream, ctx, scope);
            }
            else if (stream.expect({ expectation("Fill").be_true(), expectation(tokenizer::equals).be_true() })) {
                stream.advance(2);
                fill = parse_reference_value(stream, ctx, scope);
            }
            else if (stream.expect({ expectation("Bottom").be_true(), expectation(tokenizer::equals).be_true() })) {
                stream.advance(2);
                bottom = parse_reference_value(stream, ctx, scope);
            }
            else {
                throw std::runtime_error("");
            }

            stream.ensure({ expectation(tokenizer::semi).be_true() });
        }
        stream.ensure({ expectation(tokenizer::r_brace).be_true() });

        return ui::format::background(top, fill, bottom);
    }
    else {
        return ui::format::background(parse_reference_value(stream, ctx, scope));
    }
}

auto kdl::sema::scene_interface::parse_color_value(foundation::stream<tokenizer::token> &stream, sema::context &ctx, interpreter::scope *scope) -> std::uint32_t
{
    // Install Functions
    scope->add_function({ "rgb", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        auto r = static_cast<std::uint8_t>(args[0].integer_value());
        auto g = static_cast<std::uint8_t>(args[1].integer_value());
        auto b = static_cast<std::uint8_t>(args[2].integer_value());
        return interpreter::token(static_cast<std::int64_t>((r << 16) | (g << 8) | b | 0xFF000000LL));
    }});

    scope->add_function({ "rgba", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        auto r = static_cast<std::uint8_t>(args[0].integer_value());
        auto g = static_cast<std::uint8_t>(args[1].integer_value());
        auto b = static_cast<std::uint8_t>(args[2].integer_value());
        auto a = static_cast<std::uint8_t>(args[3].integer_value());
        return interpreter::token(static_cast<std::int64_t>((a << 24) | (r << 16) | (g << 8) | b | 0LL));
    }});

    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::integer)) {
        throw std::runtime_error("");
    }

    return static_cast<std::uint32_t>(result.value.integer_value());
}

auto kdl::sema::scene_interface::parse_bool_value(foundation::stream<tokenizer::token> &stream, sema::context &ctx, interpreter::scope *scope) -> bool
{
    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(scope ?: ctx.create_scope());

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::boolean)) {
        throw std::runtime_error("");
    }

    return result.value.bool_value();
}

auto kdl::sema::scene_interface::parse_action(foundation::stream<tokenizer::token> &stream, sema::context &ctx, interpreter::scope *scope) -> ui::format::action
{
    if (stream.expect({
        expectation(element_actions::push).be_true(),
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::identifier).be_true(),
        expectation(tokenizer::r_paren).be_true()
    })) {
        // Named Scene
        // Push(BarDialog)
        auto scene_name = stream.read(2); stream.advance();
        return ui::format::action(scene_name.string_value(), ui::format::action::type::push_named_scene);
    }
    else if (stream.expect({
        expectation(element_actions::push).be_true(),
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::reference).be_true(),
        expectation(tokenizer::r_paren).be_true()
    })) {
        // Identified Scene
        // Push(#BarDialog.Scene.1000)
        auto scene_ref = stream.read(2); stream.advance();
        return ui::format::action(scene_ref.reference_value(), ui::format::action::type::push_identified_scene);
    }
    else if (stream.expect({
        expectation(element_actions::function).be_true(),
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::identifier).be_true(),
        expectation(tokenizer::r_paren).be_true()
    })) {
        // Named Function
        // Function(Done)
        auto function_name = stream.read(2); stream.advance();
        return ui::format::action(function_name.string_value(), ui::format::action::type::named_function);
    }
    else if (stream.expect({
        expectation(element_actions::script).be_true(),
        expectation(tokenizer::l_paren).be_true(),
        expectation(tokenizer::reference).be_true(),
        expectation(tokenizer::r_paren).be_true()
    })) {
        // Lua Script Reference
        // Script(#LuaScript.1000)
        auto script_ref = stream.read(2); stream.advance();
        return ui::format::action(script_ref.reference_value(), ui::format::action::type::lua_script_reference);
    }
    else {
        throw std::runtime_error("");
    }
}

auto kdl::sema::scene_interface::parse_value(foundation::stream<tokenizer::token> &stream, sema::context &ctx, interpreter::scope *scope) -> ui::format::value
{
    ui::format::value value;

    // Handle special case first.
    if (stream.expect({ expectation(basic_names::null).be_true() })) {
        stream.advance();
        return value;
    }

    // Setup a scope and register various helper functions.
    auto value_scope = ctx.create_scope();

    value_scope->add_function({ "Get", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        if (!args[0].is(interpreter::token::reference)) {
            throw std::runtime_error("");
        }

        // Resolve the reference type information
        auto ref = args[0].reference_value();
        if (auto type = ctx.type_named(ref.type_name())) {
            ref = ref.with_type_name(type->name(), type->code());
        }
        else {
            throw std::runtime_error("");
        }

        if (args[1].is(interpreter::token::string)) {
            if (args.size() == 2 && args[1].string_value().length() == 4) {
                // Basic Lookup { RSRC,TNAM }
                value = ui::format::value(ui::format::lookup(
                    ref, args[1].string_value()
                ));
            }
        }
        else if (args[1].is(interpreter::token::string)) {
            if (args.size() == 3 && args[2].is(interpreter::token::string)) {
                // Named Lookup { RSRC,PSTR,TNAM }
                value = ui::format::value(ui::format::named_lookup(
                    ref, args[1].string_value(), args[2].string_value()
                ));
            }
            else {
                throw std::runtime_error("");
            }
        }
        else if (args[1].is(interpreter::token::integer)) {
            if (args.size() == 3 && args[2].is(interpreter::token::string)) {
                // Indexed Lookup { RSRC,HWRD,TNAM }
                value = ui::format::value(ui::format::indexed_lookup(
                    ref, args[1].integer_value(), args[2].string_value()
                ));
            }
            else if (args.size() > 3 && args.back().is(interpreter::token::string)) {
                // Indexed List Lookup { RSRC,HWRD*,TNAM }
                std::vector<std::uint16_t> list;
                for (auto i = 1; i < args.size() - 1; ++i) {
                    list.emplace_back(args[i].integer_value());
                }
                value = ui::format::value(ui::format::indexed_list_lookup(
                    ref, list, args.back().string_value()
                ));
            }
            else {
                throw std::runtime_error("");
            }
        }
        return interpreter::token::result(true);
    }});

    value_scope->add_function({ "rgb", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        auto r = static_cast<std::uint8_t>(args[0].integer_value());
        auto g = static_cast<std::uint8_t>(args[1].integer_value());
        auto b = static_cast<std::uint8_t>(args[2].integer_value());
        value = ui::format::value(static_cast<std::uint32_t>((r << 16) | (g << 8) | b | 0xFF000000));
        return interpreter::token::result(true);
    }});

    value_scope->add_function({ "rgba", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        auto r = static_cast<std::uint8_t>(args[0].integer_value());
        auto g = static_cast<std::uint8_t>(args[1].integer_value());
        auto b = static_cast<std::uint8_t>(args[2].integer_value());
        auto a = static_cast<std::uint8_t>(args[3].integer_value());
        value = ui::format::value(static_cast<std::uint32_t>((a << 24) | (r << 16) | (g << 8) | b));
        return interpreter::token::result(true);
    }});

    value_scope->add_function({ "StringList", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        std::vector<std::string> list;
        for (const auto& str : args) {
            list.emplace_back(str.string_value());
        }
        value = ui::format::value(list);
        return interpreter::token::result(true);
    }});

    value_scope->add_function({ "IntegerList", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        std::vector<std::int64_t> list;
        for (const auto& i : args) {
            list.emplace_back(i.integer_value());
        }
        value = ui::format::value(list);
        return interpreter::token::result(true);
    }});

    value_scope->add_function({ "Image", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        value = ui::format::value(args[0].reference_value());
        return interpreter::token::result(true);
    }});

    value_scope->add_function({ "ImageList", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& args) -> interpreter::token {
        std::vector<resource::reference> list;
        for (const auto& image : args) {
            list.emplace_back(image.reference_value());
        }
        value = ui::format::value(list);
        return interpreter::token::result(true);
    }});

    auto value_stmt = sema::script::parse_statement(stream, ctx);
    auto result = value_stmt.evaluate(value_scope);

    if (result.status != interpreter::script::statement::result::ok) {
        throw std::runtime_error("");
    }

    if (!result.value.is(interpreter::token::function_result)) {
        if (result.value.is(interpreter::token::string)) {
            value = ui::format::value(result.value.string_value());
        }
        else if (result.value.is(interpreter::token::integer)) {
            value = ui::format::value(result.value.integer_value());
        }
//        else if (result.value.is(interpreter::token::boolean)) {
//            value = ui::format::value(result.value.bool_value());
//        }
    }

    ctx.pop_scope();
    return value;
}