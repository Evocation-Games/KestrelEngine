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

#include "core/ui/widgets/sprite_widget.hpp"
#include "core/ui/entity/scene_entity.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"
#include "core/asset/legacy/spriteworld/sprite.hpp"

// MARK: - Lua

auto ui::widgets::sprite_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<ui::widgets::sprite_widget>("Sprite")
                .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
                .addProperty("frame", &sprite_widget::frame, &sprite_widget::set_frame)
                .addProperty("frameNumber", &sprite_widget::frame_number, &sprite_widget::set_frame_number)
                .addProperty("image", &sprite_widget::image, &sprite_widget::set_image)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::sprite_widget::sprite_widget(const luabridge::LuaRef &sprite)
{
    if (sprite.state()) {
        m_sprite_ref = sprite;
    }

    auto internal_entity = std::make_shared<graphics::entity>(math::size(1));
    m_entity = { new scene_entity(internal_entity) };

    resize(true);
}

// MARK: - Accessors

auto ui::widgets::sprite_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::sprite_widget::frame() const -> math::rect
{
    return m_frame;
}

auto ui::widgets::sprite_widget::frame_number() const -> std::int32_t
{
    return m_entity->current_frame();
}

auto ui::widgets::sprite_widget::image() const -> luabridge::LuaRef
{
    return m_sprite_ref;
}

auto ui::widgets::sprite_widget::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
    resize(true);
}

auto ui::widgets::sprite_widget::set_image(const luabridge::LuaRef &image) -> void
{
    m_sprite_ref = image;
    resize(true);
}

auto ui::widgets::sprite_widget::set_frame_number(std::int32_t n) -> void
{
    m_entity->set_current_frame(n);
}

// MARK: - Drawing

auto ui::widgets::sprite_widget::resize(bool reload) -> void
{
    if (reload) {
        if (scripting::lua::ref_isa<asset::legacy::spriteworld::sprite>(m_sprite_ref)) {
            m_entity->change_internal_entity(m_sprite_ref.cast<asset::legacy::spriteworld::sprite>().spawn_entity({0, 0}));
        }
        else if (scripting::lua::ref_isa<asset::resource_descriptor>(m_sprite_ref)) {
            auto descriptor = m_sprite_ref.cast<asset::resource_descriptor::lua_reference>();

            if (descriptor->type == asset::legacy::spriteworld::sprite::type_16) {
                asset::legacy::spriteworld::sprite sprite(descriptor);
                m_entity->change_internal_entity(sprite.spawn_entity({0, 0}));
            }
            else if (descriptor->type == asset::legacy::spriteworld::sprite::type_32) {
                asset::legacy::spriteworld::sprite sprite(descriptor);
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

    switch (m_alignment) {
        default:
            math::point position(
                m_frame.get_x() + ((m_frame.get_width() - m_entity->size().get_width()) / 2),
                m_frame.get_y() + ((m_frame.get_height() - m_entity->size().get_height()) / 2)
            );
            m_entity->set_position(position);
            m_entity->internal_entity()->set_position(position);
            break;
    }
}

auto ui::widgets::sprite_widget::draw() -> void
{
    // Do nothing...
}