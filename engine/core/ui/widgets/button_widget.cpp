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

#include "core/ui/widgets/button_widget.hpp"
#include "core/ui/entity/scene_entity.hpp"

// MARK: - Global Button Stencils

static struct
{
    ui::stencils::button_stencil::lua_reference normal { nullptr };
    ui::stencils::button_stencil::lua_reference disabled { nullptr };
    ui::stencils::button_stencil::lua_reference pressed { nullptr };
} s_global_stencils;

auto ui::widgets::button_widget::set_global_normal_stencil(const stencils::button_stencil::lua_reference& stencil) -> void
{
    s_global_stencils.normal = stencil;
}

auto ui::widgets::button_widget::set_global_pressed_stencil(const stencils::button_stencil::lua_reference& stencil) -> void
{
    s_global_stencils.pressed = stencil;
}

auto ui::widgets::button_widget::set_global_disabled_stencil(const stencils::button_stencil::lua_reference& stencil) -> void
{
    s_global_stencils.disabled = stencil;
}

// MARK: - Lua

auto ui::widgets::button_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<button_widget>("Button")
                .addStaticFunction("setGlobalNormalStencil", &button_widget::set_global_normal_stencil)
                .addStaticFunction("setGlobalPressedStencil", &button_widget::set_global_pressed_stencil)
                .addStaticFunction("setGlobalDisabledStencil", &button_widget::set_global_disabled_stencil)
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addProperty("normalStencil", &button_widget::normal_stencil, &button_widget::set_normal_stencil)
                .addProperty("pressedStencil", &button_widget::pressed_stencil, &button_widget::set_pressed_stencil)
                .addProperty("disabledStencil", &button_widget::disabled_stencil, &button_widget::set_disabled_stencil)
                .addProperty("label", &button_widget::label, &button_widget::set_label)
                .addProperty("title", &button_widget::label, &button_widget::set_label)
                .addProperty("icon", &button_widget::icon, &button_widget::set_icon)
                .addProperty("font", &button_widget::font, &button_widget::set_font)
                .addProperty("labelColor", &button_widget::label_color, &button_widget::set_label_color)
                .addProperty("pressedLabelColor", &button_widget::label_pressed_color, &button_widget::set_label_pressed_color)
                .addProperty("disabledLabeledColor", &button_widget::label_disabled_color, &button_widget::set_label_disabled_color)
                .addProperty("action", &button_widget::action_body, &button_widget::set_action)
                .addProperty("mouseEnter", &button_widget::mouse_enter_body, &button_widget::set_mouse_enter)
                .addProperty("mouseExit", &button_widget::mouse_exit_body, &button_widget::set_mouse_exit)
                .addProperty("userInfo", &button_widget::user_info, &button_widget::set_user_info)
                .addProperty("frame", &button_widget::frame, &button_widget::set_frame)
                .addProperty("continuous", &button_widget::continuous, &button_widget::set_continuous_action)
                .addProperty("disabled", &button_widget::disabled, &button_widget::set_disabled)
                .addFunction("draw", &button_widget::draw)
                .addFunction("setAction", &button_widget::set_action)
                .addFunction("setMouseEnter", &button_widget::set_mouse_enter)
                .addFunction("setMouseExit", &button_widget::set_mouse_exit)
            .endClass()
        .endNamespace();
}

// MARK: - Constructor

ui::widgets::button_widget::button_widget(const std::string &label)
    : m_label(label)
{
    m_font = ui::font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    graphics::typesetter ts { label };
    ts.set_font(*m_font.get());
    ts.layout();

    if (ts.get_bounding_size().width == 0 || ts.get_bounding_size().height == 0) {
        m_canvas = std::make_shared<graphics::canvas>(math::size(10, 10));
        m_entity = { new scene_entity(m_canvas->spawn_entity({ 0, 0 })) };
    }
    else {
        m_canvas = std::make_shared<graphics::canvas>(ts.get_bounding_size());
        m_entity = { new scene_entity(m_canvas->spawn_entity({ 0, 0 })) };
    }

    bind_internal_events();
}

// MARK: - Accessors

auto ui::widgets::button_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::button_widget::normal_stencil() const -> stencils::button_stencil::lua_reference
{
    return m_normal_stencil;
}

auto ui::widgets::button_widget::pressed_stencil() const -> stencils::button_stencil::lua_reference
{
    return m_pressed_stencil;
}

auto ui::widgets::button_widget::disabled_stencil() const -> stencils::button_stencil::lua_reference
{
    return m_disabled_stencil;
}

auto ui::widgets::button_widget::label() const -> std::string
{
    return m_label;
}

auto ui::widgets::button_widget::font() const -> ui::font::reference::lua_reference
{
    return m_font;
}

auto ui::widgets::button_widget::is_pressed() const -> bool
{
    return m_pressed;
}

auto ui::widgets::button_widget::is_disabled() const -> bool
{
    return m_disabled;
}

auto ui::widgets::button_widget::label_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_label_normal) };
}

auto ui::widgets::button_widget::label_pressed_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_label_pressed) };
}

auto ui::widgets::button_widget::label_disabled_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_label_disabled) };
}

auto ui::widgets::button_widget::icon() const -> asset::static_image::lua_reference
{
    return m_icon;
}

auto ui::widgets::button_widget::action_body() const -> luabridge::LuaRef
{
    return m_action;
}

auto ui::widgets::button_widget::mouse_enter_body() const -> luabridge::LuaRef
{
    return m_mouse_enter;
}

auto ui::widgets::button_widget::mouse_exit_body() const -> luabridge::LuaRef
{
    return m_mouse_exit;
}

auto ui::widgets::button_widget::user_info() const -> luabridge::LuaRef
{
    return m_user_info;
}

auto ui::widgets::button_widget::frame() const -> math::rect
{
    return { m_entity->position(), m_entity->size() };
}

auto ui::widgets::button_widget::disabled() const -> bool
{
    return m_disabled;
}

auto ui::widgets::button_widget::continuous() const -> bool
{
    return m_continuous;
}

// MARK: - Setters

auto ui::widgets::button_widget::set_label(const std::string& label) -> void
{
    m_label = label;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_font(const ui::font::reference::lua_reference & font) -> void
{
    m_font = font;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_label_color(const graphics::color::lua_reference& color) -> void
{
    m_label_normal = { *color.get() };
    m_dirty = true;
}

auto ui::widgets::button_widget::set_label_pressed_color(const graphics::color::lua_reference& color) -> void
{
    m_label_pressed = { *color.get() };
    m_dirty = true;
}

auto ui::widgets::button_widget::set_label_disabled_color(const graphics::color::lua_reference& color) -> void
{
    m_label_disabled = { *color.get() };
    m_dirty = true;
}

auto ui::widgets::button_widget::set_icon(const luabridge::LuaRef& icon) -> void
{
    m_icon = asset::static_image::from(icon);
    m_dirty = true;
}

auto ui::widgets::button_widget::set_action(const luabridge::LuaRef& body) -> void
{
    m_action = body;
}

auto ui::widgets::button_widget::set_mouse_enter(const luabridge::LuaRef& body) -> void
{
    m_mouse_enter = body;
}

auto ui::widgets::button_widget::set_mouse_exit(const luabridge::LuaRef& body) -> void
{
    m_mouse_exit = body;
}

auto ui::widgets::button_widget::set_user_info(const luabridge::LuaRef& info) -> void
{
    m_user_info = info;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_frame(const math::rect &r) -> void
{
    m_canvas = nullptr;
    m_canvas = std::make_shared<graphics::canvas>(r.size);
    m_entity->change_internal_entity(m_canvas->spawn_entity(r.origin));

    m_entity->internal_entity()->set_position(r.origin);
    m_entity->set_position(r.origin);

    redraw_entity();
    bind_internal_events();
}

auto ui::widgets::button_widget::set_normal_stencil(const stencils::button_stencil::lua_reference &stencil) -> void
{
    m_normal_stencil = stencil;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_pressed_stencil(const stencils::button_stencil::lua_reference &stencil) -> void
{
    m_pressed_stencil = stencil;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_disabled_stencil(const stencils::button_stencil::lua_reference &stencil) -> void
{
    m_disabled_stencil = stencil;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_disabled(bool disabled) -> void
{
    m_disabled = disabled;
    m_dirty = true;
}

auto ui::widgets::button_widget::set_continuous_action(bool continuous) -> void
{
    m_continuous = continuous;
    m_entity->set_continuous_mouse_down_action(continuous);
}

// MARK: - Internal Events

auto ui::widgets::button_widget::bind_internal_events() -> void
{
    m_entity->on_mouse_down_internal([&] (const event& e) {
        if (m_disabled) {
            return;
        }

        m_pressed = true;
        redraw_entity();

        if (m_action.state() && m_action.isFunction() && m_continuous) {
            m_action();
        }
    });

    m_entity->on_mouse_drag_internal([&] (const event& e) {

    });

    m_entity->on_mouse_release_internal([&] (const event& e) {
        if (m_disabled) {
            return;
        }

        m_pressed = false;
        redraw_entity();

        if (m_action.state() && m_action.isFunction() && !m_continuous) {
            m_action();
        }
    });
}

// MARK: - Drawing

auto ui::widgets::button_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
    if (m_continuous && m_pressed) {
        m_action();
    }
}

auto ui::widgets::button_widget::redraw_entity() -> void
{
    stencils::button_stencil::info::lua_reference info { new stencils::button_stencil::info() };
    info->bounds = frame();
    info->font = font();
    info->text = label();
    info->icon = m_icon;

    if (m_disabled && m_disabled_stencil.get()) {
        info->text_color = { new graphics::color(m_label_disabled) };
        m_disabled_stencil->draw(m_canvas, info);
    }
    else if (m_disabled && s_global_stencils.disabled.get()) {
        info->text_color = { new graphics::color(m_label_disabled) };
        s_global_stencils.disabled->draw(m_canvas, info);
    }
    else if ((m_pressed && !m_disabled) && m_pressed_stencil.get()) {
        info->text_color = { new graphics::color(m_label_pressed) };
        m_pressed_stencil->draw(m_canvas, info);
    }
    else if ((m_pressed && !m_disabled) && s_global_stencils.pressed.get()) {
        info->text_color = { new graphics::color(m_label_pressed) };
        s_global_stencils.pressed->draw(m_canvas, info);
    }
    else if (m_normal_stencil.get()) {
        info->text_color = { new graphics::color(m_label_normal) };
        m_normal_stencil->draw(m_canvas, info);
    }
    else if (s_global_stencils.normal.get()) {
        info->text_color = { new graphics::color(m_label_normal) };
        s_global_stencils.normal->draw(m_canvas, info);
    }

    m_dirty = false;
}

// MARK: - Events

auto ui::widgets::button_widget::receive_event(const event &e) -> bool
{
    if (e.is_mouse_event() && entity()->hit_test(e.location() - entity()->position())) {
        if (e.has_moved() && !m_inside) {
            m_inside = true;
            mouse_enter();
        }

        if (e.is_pressed() && !m_pressed) {
            mouse_down();
        }

        if (e.is_released() && m_pressed) {
            mouse_up();
        }
        return true;
    }
    else if (e.is_mouse_event()) {
        if (e.has_moved() && m_inside) {
            m_inside = false;
            mouse_exit();
            return true;
        }
    }
    return false;
}

auto ui::widgets::button_widget::mouse_down() -> void
{
    if (m_disabled) {
        return;
    }
    m_pressed = true;
    m_dirty = true;
}

auto ui::widgets::button_widget::mouse_up() -> void
{
    if (m_disabled) {
        return;
    }

    if (m_pressed && m_action.state() && m_action.isFunction()) {
        m_action();
    }

    m_pressed = false;
    m_dirty = true;

}

auto ui::widgets::button_widget::mouse_enter() -> void
{
    if (m_mouse_enter.state() && m_mouse_enter.isFunction()) {
        m_mouse_enter(m_user_info);
    }
}

auto ui::widgets::button_widget::mouse_exit() -> void
{
    if (m_mouse_exit.state() && m_mouse_exit.isFunction()) {
        m_mouse_exit(m_user_info);
    }
}

