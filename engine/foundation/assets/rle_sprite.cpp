// Copyright (c) 2020 Tom Hancocks
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

#include "foundation/assets/rle_sprite.hpp"
#include "foundation/assets/cache.hpp"
#include "libGraphite/rsrc/manager.hpp"
#include "libGraphite/quickdraw/rle.hpp"
#include "LuaBridge/Vector.h"

// MARK: - Lua Integration

auto kestrel::assets::rle_sprite::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<assets::rle_sprite>("Sprites")
            .addConstructor<auto(*)(resource_reference::lua_reference)->void, rle_sprite::lua_reference>()
            .addStaticFunction("load", &rle_sprite::load)
            .addFunction("draw", &rle_sprite::lua_draw)
            .addFunction("drawFrame", & rle_sprite::lua_draw_frame)
            .addFunction("initializeSpriteSheet", &rle_sprite::lua_reconfigure_spritesheet)
            .addProperty("size", &rle_sprite::size)
        .endClass();
}

// MARK: - Constructor

auto kestrel::assets::rle_sprite::load(resource_reference::lua_reference ref) -> rle_sprite::lua_reference
{
    // Attempt to de-cache
    auto cached_sprite = cache::global().fetch("rlëD", ref);
    if (cached_sprite.has_value()) {
        return std::any_cast<rle_sprite::lua_reference>(cached_sprite.value());
    }

    // Get new and cache it.
    auto new_sprite = rle_sprite::lua_reference(new rle_sprite(ref));
    cache::global().add("rlëD", ref, new_sprite);
    return new_sprite;
}

kestrel::assets::rle_sprite::rle_sprite(resource_reference::lua_reference ref)
{
    if (ref->id().has_value()) {
        if (auto res = graphite::rsrc::manager::shared_manager().find("rlëD", ref->id().value()).lock()) {
            m_id = res->id();
            m_name = res->name();
            graphite::qd::rle rle(res->data(), m_id, m_name);

            if (auto surface = rle.surface().lock()) {
                m_spritesheet = gl::spritesheet::create(surface, rle.frame_rect(0).size(), rle.frame_count());
            }
        }
    }
    // TODO: Handle Resource Name Reference
}

// MARK: - Sprite Sheet

auto kestrel::assets::rle_sprite::lua_reconfigure_spritesheet(kestrel::math::size::lua_reference size,
                                                              int total_sprites) -> void
{

}

// MARK: - Accessors

auto kestrel::assets::rle_sprite::valid() const -> bool
{
    return (m_spritesheet != nullptr);
}

auto kestrel::assets::rle_sprite::spritesheet() -> std::weak_ptr<gl::spritesheet>
{
    return m_spritesheet;
}

auto kestrel::assets::rle_sprite::count() const -> int
{
    return m_spritesheet->sprite_count();
}

auto kestrel::assets::rle_sprite::size() const -> math::size
{
    return m_spritesheet->texture()->size();
}

// MARK: - Drawing

auto kestrel::assets::rle_sprite::lua_draw(math::point::lua_reference position) const -> void
{
    draw(position->vector());
}

auto kestrel::assets::rle_sprite::lua_draw_frame(math::point::lua_reference position, int frame) const -> void
{
    draw(position->vector(), frame);
}

auto kestrel::assets::rle_sprite::draw(const math::vector& v, int frame) const -> void
{
    if (m_spritesheet) {
        if (auto sprite = m_spritesheet->sprite(frame).lock()) {
            sprite->draw(v, *m_spritesheet);
        }
    }
}
