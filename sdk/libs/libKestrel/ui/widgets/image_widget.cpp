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

#include <libKestrel/ui/widgets/image_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>
#include <libKestrel/graphics/image/static_image.hpp>

// MARK: - Construction

kestrel::ui::widgets::image_widget::image_widget(const luabridge::LuaRef &image)
{
    if (image.state()) {
        m_image_ref = image;
    }

    auto internal_entity = std::make_shared<ecs::entity>(math::size(1));
    m_entity = { new scene_entity(internal_entity) };

    resize(true);
}

// MARK: - Accessors

auto kestrel::ui::widgets::image_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::image_widget::frame() const -> math::rect
{
    return m_frame;
}

auto kestrel::ui::widgets::image_widget::image() const -> luabridge::LuaRef
{
    return m_image_ref;
}

auto kestrel::ui::widgets::image_widget::dynamic_resizing() const -> bool
{
    return m_dynamic_resizing;
}

auto kestrel::ui::widgets::image_widget::anchor_point() const -> layout::axis_origin
{
    return m_entity->anchor_point();
}

auto kestrel::ui::widgets::image_widget::scaling_mode() const -> layout::scaling_mode
{
    return m_entity->scaling_mode();
}

// MARK: - Setters

auto kestrel::ui::widgets::image_widget::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
    resize();
}

auto kestrel::ui::widgets::image_widget::set_image(const luabridge::LuaRef &image) -> void
{
    m_image_ref = image;
    resize(true);
}

auto kestrel::ui::widgets::image_widget::set_dynamic_resizing(bool v) -> void
{
    m_dynamic_resizing = v;
    resize();
}

auto kestrel::ui::widgets::image_widget::set_click_action(const luabridge::LuaRef &action) -> void
{
    m_click_action = action;
}

auto kestrel::ui::widgets::image_widget::set_down_action(const luabridge::LuaRef &action) -> void
{
    m_down_action = action;
}

auto kestrel::ui::widgets::image_widget::set_anchor_point(const layout::axis_origin &point) -> void
{
    m_entity->set_anchor_point(point);
}

auto kestrel::ui::widgets::image_widget::set_scaling_mode(const layout::scaling_mode &mode) -> void
{
    m_entity->set_scaling_mode(mode);
}

// MARK: - Drawing

auto kestrel::ui::widgets::image_widget::resize(bool reload) -> void
{
    if (reload) {
        if (lua::ref_isa<image::static_image>(m_image_ref)) {
            m_entity->change_internal_entity(m_image_ref.cast<image::static_image::lua_reference>()->spawn_entity({ 0, 0 }));
        }
        else if (lua::ref_isa<image::legacy::macintosh::quickdraw::picture>(m_image_ref)) {
            m_entity->change_internal_entity(m_image_ref.cast<image::legacy::macintosh::quickdraw::picture::lua_reference>()->spawn_entity({ 0, 0 }));
        }
        else if (lua::ref_isa<image::legacy::macintosh::quickdraw::color_icon>(m_image_ref)) {
            m_entity->change_internal_entity(m_image_ref.cast<image::legacy::macintosh::quickdraw::color_icon::lua_reference>()->spawn_entity({ 0, 0 }));
        }
        else if (lua::ref_isa<resource::descriptor>(m_image_ref)) {
            auto descriptor = m_image_ref.cast<resource::descriptor::lua_reference>();
            image::static_image img(descriptor);
            m_entity->change_internal_entity(img.spawn_entity({0, 0}));
        }
        else {
            // TODO: Handle unrecognised image format...
        }
    }

    if (m_dynamic_resizing) {
        m_entity->set_size(m_entity->internal_entity()->get_size());
        switch (m_alignment) {
            default:
                math::point position(
                    m_frame.x() + ((m_frame.width() - m_entity->size().width()) / 2.f),
                    m_frame.y() + ((m_frame.height() - m_entity->size().height()) / 2.f)
                );
                m_entity->set_position(position);
                break;
        }
    }
    else if (m_entity->internal_entity() && m_entity->internal_entity()->texture()) {
        auto texture = m_entity->internal_entity()->texture();
        m_entity->set_size(texture->size());
    }
}

auto kestrel::ui::widgets::image_widget::draw() -> void
{
    // Do nothing...
}

// MARK: - Event Handling

auto kestrel::ui::widgets::image_widget::receive_event(const event &e) -> bool
{
    if (e.is_mouse_event() && entity()->hit_test(e.location())) {
        if (e.has(::ui::event::any_mouse_down) && !m_pressed) {
            m_pressed = true;
            if (m_down_action.state() && m_down_action.isFunction()) {
                m_down_action();
            }
        }

        if (e.has(::ui::event::any_mouse_up) && m_pressed) {
            m_pressed = false;
            if (m_click_action.state() && m_click_action.isFunction()) {
                m_click_action();
            }
        }
        return true;
    }
    return false;
}

