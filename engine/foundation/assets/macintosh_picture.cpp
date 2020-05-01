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


#include "foundation/assets/macintosh_picture.hpp"
#include "foundation/assets/cache.hpp"
#include "libGraphite/rsrc/manager.hpp"
#include "libGraphite/quickdraw/pict.hpp"
#include "libGraphite/quickdraw/cicn.hpp"
#include "LuaBridge/Vector.h"

// MARK: - Lua Integration

auto kestrel::assets::macintosh_picture::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<assets::macintosh_picture>("MacintoshPicture")
            .addConstructor<auto(*)(resource_reference::lua_reference)->void, macintosh_picture::lua_reference>()
            .addStaticFunction("load", &macintosh_picture::load)
            .addFunction("draw", &macintosh_picture::lua_draw)
            .addFunction("drawFrame", & macintosh_picture::lua_draw_frame)
            .addFunction("initializeSpriteSheet", &macintosh_picture::lua_reconfigure_spritesheet)
        .endClass();
}

// MARK: - Constructor

auto kestrel::assets::macintosh_picture::load(resource_reference::lua_reference ref) -> macintosh_picture::lua_reference
{
    // Attempt to de-cache
    auto cached_pict = cache::global().fetch("PICT", ref);
    if (cached_pict.has_value()) {
        return std::any_cast<macintosh_picture::lua_reference>(cached_pict.value());
    }

    // Get new and cache it.
    auto new_image = macintosh_picture::lua_reference(new macintosh_picture(ref));
    cache::global().add("PICT", ref, new_image);
    return new_image;
}

kestrel::assets::macintosh_picture::macintosh_picture(resource_reference::lua_reference ref)
{
    if (ref->id().has_value()) {
        if (auto res = graphite::rsrc::manager::shared_manager().find("PICT", ref->id().value()).lock()) {
            m_id = res->id();
            m_name = res->name();
            construct_spritesheet(graphite::qd::pict(res->data()).image_surface());
        }
    }
    // TODO: Handle Resource Name Reference
}

auto kestrel::assets::macintosh_picture::construct_spritesheet(std::weak_ptr<graphite::qd::surface> surface) -> void
{
    if (auto s = surface.lock()) {
        m_spritesheet = gl::spritesheet::create(s, s->size(), 1);
    }
}

auto kestrel::assets::macintosh_picture::reconfigure_spritesheet(const math::size& sprite_size, int total_sprites) -> void
{
    m_spritesheet->configure_sprites(sprite_size, total_sprites);
}

auto kestrel::assets::macintosh_picture::lua_reconfigure_spritesheet(std::vector<double> size, int total_sprites) -> void
{
    reconfigure_spritesheet({ size[0], size[1] }, total_sprites);
}

// MARK: - Accessors

auto kestrel::assets::macintosh_picture::valid() const -> bool
{
    return (m_spritesheet != nullptr);
}

auto kestrel::assets::macintosh_picture::spritesheet() -> std::weak_ptr<gl::spritesheet>
{
    return m_spritesheet;
}

auto kestrel::assets::macintosh_picture::count() const -> int
{
    return m_spritesheet->sprite_count();
}

auto kestrel::assets::macintosh_picture::size() const -> math::size
{
    return m_spritesheet->texture()->size();
}

// MARK: - Drawing

auto kestrel::assets::macintosh_picture::lua_draw(std::vector<double> position) const -> void
{
    draw(math::vector(position[0], position[1]));
}

auto kestrel::assets::macintosh_picture::lua_draw_frame(std::vector<double> position, int frame) const -> void
{
    draw(math::vector(position[0], position[1]), frame);
}

auto kestrel::assets::macintosh_picture::draw(const math::vector& v, int frame) const -> void
{
    if (m_spritesheet) {
        if (auto sprite = m_spritesheet->sprite(frame).lock()) {
            sprite->draw(v, *m_spritesheet);
        }
    }
}
