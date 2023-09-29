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

#pragma once

#include <libFoundation/stream/stream.hpp>
#include <libKDL/tokenizer/token.hpp>
#include <libKDL/sema/context.hpp>
#include <libUI/format/interface/interface.hpp>

namespace kdl::sema::scene_interface
{
    auto test(const foundation::stream<tokenizer::token>& stream) -> bool;
    auto parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> void;

    // Elements
    auto parse_element_type(foundation::stream<tokenizer::token>& stream) -> enum ui::format::element::type;
    auto parse_element(foundation::stream<tokenizer::token>& stream, sema::context& ctx, enum ui::format::element::type type, const std::string& name) -> ui::format::element;

    // Value Parsers
    auto parse_reference_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> resource::reference;
    auto parse_string_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> std::string;
    auto parse_integer_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> std::int64_t;
    auto parse_unsigned_integer_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> std::uint64_t;
    auto parse_rect_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> ui::rect;
    auto parse_background_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> ui::format::background;
    auto parse_color_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> std::uint32_t;
    auto parse_bool_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> bool;
    auto parse_action(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> ui::format::action;
    auto parse_value(foundation::stream<tokenizer::token>& stream, sema::context& ctx, interpreter::scope *scope = nullptr) -> ui::format::value;

    struct interface_fields {
        static constexpr const char *title = "Title";
        static constexpr const char *frame = "Frame";
        static constexpr const char *size = "Size";
        static constexpr const char *background = "Background";
        static constexpr const char *flags = "Flags";
    };

    struct interface_flags {
        static constexpr const char *dialog = "IsDialog";
        static constexpr const char *imgui = "UsesImGui";
        static constexpr const char *shows_titlebar = "ShowsTitleBar";
        static constexpr const char *shows_closebutton = "ShowsCloseButton";
        static constexpr const char *passthrough = "ScenePassthrough";
        static constexpr const char *centered = "Centered";
    };

    struct interface_elements {
        static constexpr const char *button = "Button";
        static constexpr const char *label = "Label";
        static constexpr const char *checkbox = "Checkbox";
        static constexpr const char *popup = "PopupButton";
        static constexpr const char *canvas = "Canvas";
        static constexpr const char *image = "Image";
        static constexpr const char *sprite = "Sprite";
        static constexpr const char *text = "TextField";
        static constexpr const char *textarea = "TextArea";
        static constexpr const char *grid = "Grid";
        static constexpr const char *list = "List";
        static constexpr const char *separator = "Separator";
        static constexpr const char *slider = "Slider";
        static constexpr const char *radio = "RadioButton";
        static constexpr const char *scroll_area = "ScrollArea";
        static constexpr const char *timed_trigger = "TimedTrigger";
        static constexpr const char *key_trigger = "KeyTrigger";
        static constexpr const char *vertical_spacer = "VerticalSpacer";
        static constexpr const char *horizontal_spacer = "HorizontalSpacer";
        static constexpr const char *position = "Position";
    };

    struct attribute_names {
        static constexpr const char *id = "ID";
        static constexpr const char *frame = "Frame";
        static constexpr const char *y = "Y";
        static constexpr const char *x = "X";
        static constexpr const char *size = "Size";
        static constexpr const char *color = "Color";
        static constexpr const char *background_color = "BackgroundColor";
        static constexpr const char *selection_color = "SelectionColor";
        static constexpr const char *border_color = "BorderColor";
        static constexpr const char *secondary_color = "SecondaryColor";
        static constexpr const char *font = "Font";
        static constexpr const char *font_size = "FontSize";
        static constexpr const char *hidden = "Hidden";
        static constexpr const char *disabled = "Disabled";
        static constexpr const char *weight = "Weight";
        static constexpr const char *horizontal_alignment = "HorizontalAlignment";
        static constexpr const char *vertical_alignment = "VerticalAlignment";
        static constexpr const char *axis_origin = "AxisOrigin";
        static constexpr const char *action = "Action";
        static constexpr const char *value = "Value";
    };

    struct horizontal_alignment_names {
        static constexpr const char *left = "Left";
        static constexpr const char *center = "Center";
        static constexpr const char *right = "Right";
    };

    struct vertical_alignment_names {
        static constexpr const char *top = "Top";
        static constexpr const char *middle = "Middle";
        static constexpr const char *bottom = "Bottom";
    };

    struct boolean_names {
        static constexpr const char *yes = "yes";
        static constexpr const char *no = "no";
        static constexpr const char *True = "true";
        static constexpr const char *False = "false";
    };

    struct element_actions {
        static constexpr const char *push = "Scene";
        static constexpr const char *function = "Function";
        static constexpr const char *script = "Script";
    };

    struct axis_origin {
        static constexpr const char *top_left = "TopLeft";
        static constexpr const char *top_center = "TopCenter";
        static constexpr const char *top_right = "TopRight";
        static constexpr const char *middle_left = "MiddleLeft";
        static constexpr const char *center = "Center";
        static constexpr const char *middle_right = "MiddleRight";
        static constexpr const char *bottom_left = "BottomLeft";
        static constexpr const char *bottom_center = "BottomCenter";
        static constexpr const char *bottom_right = "BottomRight";
    };

    struct basic_names {
        static constexpr const char *null = "null";
    };
}
