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

#include <libKestrel/ui/widgets/sprite_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>
#include <libKestrel/graphics/legacy/spriteworld/sprite.hpp>

// MARK: - Construction

kestrel::ui::widgets::sprite_widget::sprite_widget(const luabridge::LuaRef &sprite)
{
    if (sprite.state()) {
        m_sprite_ref = sprite;
    }

    auto internal_entity = std::make_shared<ecs::entity>(math::size(1));
    m_entity = { new scene_entity(internal_entity) };

    resize(true);
}

// MARK: - Accessors

auto kestrel::ui::widgets::sprite_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::sprite_widget::frame() const -> math::rect
{
    return m_frame;
}

auto kestrel::ui::widgets::sprite_widget::frame_number() const -> std::int32_t
{
    return m_entity->current_frame();
}

auto kestrel::ui::widgets::sprite_widget::image() const -> luabridge::LuaRef
{
    return m_sprite_ref;
}

auto kestrel::ui::widgets::sprite_widget::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
    resize(false);
}

auto kestrel::ui::widgets::sprite_widget::set_image(const luabridge::LuaRef &image) -> void
{
    m_sprite_ref = image;
    resize(true);
}

auto kestrel::ui::widgets::sprite_widget::set_frame_number(std::int32_t n) -> void
{
    m_entity->set_current_frame(n);
}

// MARK: - Drawing

auto kestrel::ui::widgets::sprite_widget::resize(bool reload) -> void
{
    if (reload) {
        if (lua::ref_isa<image::legacy::spriteworld::sprite>(m_sprite_ref)) {
            m_entity->change_internal_entity(m_sprite_ref.cast<image::legacy::spriteworld::sprite>().spawn_entity({0, 0}));
        }
        else if (lua::ref_isa<resource::descriptor>(m_sprite_ref)) {
            auto descriptor = m_sprite_ref.cast<resource::descriptor::lua_reference>();

            if (descriptor->with_type(image::legacy::spriteworld::sprite::alternate_type::code)->valid()) {
                descriptor = descriptor->with_type(image::legacy::spriteworld::sprite::alternate_type::code);
            }
            else if (descriptor->with_type(image::legacy::spriteworld::sprite::resource_type::code)->valid()) {
                descriptor = descriptor->with_type(image::legacy::spriteworld::sprite::resource_type::code);
            }

            if (descriptor->type == image::legacy::spriteworld::sprite::resource_type::code) {
                image::legacy::spriteworld::sprite sprite(descriptor);
                m_entity->change_internal_entity(sprite.spawn_entity({0, 0}));
            }
            else if (descriptor->type == image::legacy::spriteworld::sprite::alternate_type::code) {
                image::legacy::spriteworld::sprite sprite(descriptor);
                m_entity->change_internal_entity(sprite.spawn_entity({0, 0}));
            }
            else {
                // TODO: Handle this scenario...
            }
        }
        else {
            // TODO: Handle unrecognised image format...
        }
    }

    math::size size = entity()->size();
    auto ar = size.aspect_ratio();
    if (size.width() >= m_frame.size().width() - 30) {
        size.set_width(m_frame.size().width() - 30);
        size.set_height(size.width() * ar);
    }

    if (size.height() >= m_frame.size().height() - 30) {
        size.set_height(m_frame.size().height() - 30);
        size.set_width(size.height() / ar);
    }

    switch (m_alignment) {
        default:
            math::point position(
                m_frame.x() + ((m_frame.width() - size.width()) / 2.f),
                m_frame.y() + ((m_frame.height() - size.height()) / 2.f)
            );
            m_entity->set_position(position);
            m_entity->set_size(size);
            break;
    }
}

auto kestrel::ui::widgets::sprite_widget::draw() -> void
{
    // Do nothing...
}