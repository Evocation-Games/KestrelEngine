// Copyright (c) 2022 Tom Hancocks
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

#include <memory>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/event/responder/responder_chain.hpp>

namespace kestrel::ui::widgets
{
    enum class lua_api(Widget.ImageContentAlignment, Available_0_8) image_content_alignment : std::int8_t
    {
        center lua_case(Center, Available_0_8),
        fit lua_case(Fit, Available_0_8),
        fill lua_case(Fill, Available_0_8)
    };

    struct lua_api(Widget.Image, Available_0_8) image_widget: public responder_chain::mouse_responder
    {
    public:
        has_constructable_lua_api(image_widget);

        luatool_type_fix(const luabridge::LuaRef&, image)
        lua_constructor(Available_0_8) explicit image_widget(const luabridge::LuaRef& image);

        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;
        lua_function(draw, Available_0_8) auto draw() -> void;

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(image, Available_0_8) [[nodiscard]] auto image() const -> luabridge::LuaRef;
        lua_getter(dynamicResizing, Available_0_8) [[nodiscard]] auto dynamic_resizing() const -> bool;
        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_getter(scalingMode, Available_0_9) [[nodiscard]] auto scaling_mode() const -> layout::scaling_mode;

        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;
        lua_setter(image, Available_0_8) auto set_image(const luabridge::LuaRef& image) -> void;
        lua_setter(dynamicResizing, Available_0_8) auto set_dynamic_resizing(bool v) -> void;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& point) -> void;
        lua_setter(scalingMode, Available_0_9) auto set_scaling_mode(const layout::scaling_mode& mode) -> void;

        lua_function(onMouseDown, Available_0_8) auto set_down_action(const luabridge::LuaRef& action) -> void;
        lua_function(onClick, Available_0_8) auto set_click_action(const luabridge::LuaRef& action) -> void;

        auto receive_event(const event& e) -> bool override;

    private:
        bool m_pressed { false };
        bool m_dynamic_resizing { true };
        math::rect m_frame { 0, 0, 100, 100 };
        luabridge::LuaRef m_image_ref { nullptr };
        scene_entity::lua_reference m_entity { nullptr };
        image_content_alignment m_alignment { image_content_alignment::center };
        luabridge::LuaRef m_down_action { nullptr };
        luabridge::LuaRef m_click_action { nullptr };

        auto resize(bool reload = false) -> void;
    };
}

