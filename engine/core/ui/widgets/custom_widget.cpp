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

#include "core/ui/widgets/custom_widget.hpp"
#include "core/ui/entity/scene_entity.hpp"

// MARK: - Lua

auto ui::widgets::custom_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<ui::widgets::custom_widget>("Custom")
                .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
                .addProperty("frame", &custom_widget::frame, &custom_widget::set_frame)
                .addProperty("userInfo", &custom_widget::user_info, &custom_widget::set_user_info)
                .addFunction("draw", &custom_widget::redraw)
                .addFunction("drawingFunction", &custom_widget::set_drawing_function)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::custom_widget::custom_widget(const luabridge::LuaRef &drawing_function)
: m_drawing_function(drawing_function), m_user_info(nullptr)
{
    resize();
}

// MARK: - Accessors

auto ui::widgets::custom_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::custom_widget::frame() const -> math::rect
{
    return m_frame;
}

auto ui::widgets::custom_widget::user_info() const -> luabridge::LuaRef
{
    return m_user_info;
}

// MARK: - Setters

auto ui::widgets::custom_widget::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
    resize();

    m_entity->internal_entity()->set_position(frame.origin);
    m_entity->set_position(frame.origin);
}

auto ui::widgets::custom_widget::set_user_info(const luabridge::LuaRef &info) -> void
{
    m_user_info = info;
}

auto ui::widgets::custom_widget::set_drawing_function(const luabridge::LuaRef &block) -> void
{
    m_drawing_function = block;
}

// MARK: - Drawing

auto ui::widgets::custom_widget::resize() -> void
{
    m_dirty = true;
    m_canvas = { new graphics::canvas(m_frame.size) };
    m_entity = { new scene_entity(m_canvas->spawn_entity({ 0, 0 })) };
}

auto ui::widgets::custom_widget::redraw() -> void
{
    if (m_drawing_function.state() && m_drawing_function.isFunction()) {
        m_drawing_function(m_canvas, m_user_info);
        m_canvas->rebuild_texture();
    }
    m_dirty = false;
}

auto ui::widgets::custom_widget::draw() -> void
{
    if (m_dirty) {
        redraw();
    }
}
