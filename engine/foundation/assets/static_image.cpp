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

#include "foundation/assets/static_image.hpp"
#include "foundation/assets/cache.hpp"
#include "libGraphite/rsrc/manager.hpp"
#include "libGraphite/quickdraw/pict.hpp"
#include "libGraphite/quickdraw/cicn.hpp"
#include "LuaBridge/Vector.h"

// MARK: - Lua Integration

auto kestrel::assets::static_image::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<assets::static_image>("StaticImage")
            .addStaticFunction("load", &static_image::load)
            .addFunction("draw", &static_image::lua_draw)
            .addFunction("initializeSpriteSheet", &static_image::lua_reconfigure_spritesheet)
        .endClass();
}

// MARK: - Constructor

auto kestrel::assets::static_image::load(int64_t id) -> static_image::lua_static_image
{
    // Attempt to de-cache
    auto cached_pict = cache::global().fetch("PICT", id);
    if (cached_pict.has_value()) {
        return std::any_cast<static_image::lua_static_image>(cached_pict.value());
    }

    // Get new and cache it.
    auto new_image = lua_static_image(new static_image(id));
    cache::global().add("PICT", id, new_image);
    return new_image;
}

kestrel::assets::static_image::static_image(int64_t id, std::vector<static_image::type> preference)
        : m_id(id)
{
    // Work through the type preference and attempt to load the appropriate image.
    for (auto type : preference) {
        switch (type) {
            case pict: {
                if (auto res = graphite::rsrc::manager::shared_manager().find("PICT", id).lock()) {
                    m_name = res->name();
                    m_type = pict;
                    construct_spritesheet(graphite::qd::pict(res->data()).image_surface());
                    return;
                }
                break;
            }
            case cicn: {
                if (auto res = graphite::rsrc::manager::shared_manager().find("cicn", id).lock()) {
                    m_name = res->name();
                    m_type = cicn;
                    construct_spritesheet(graphite::qd::cicn(res->data()).surface());
                    return;
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

auto kestrel::assets::static_image::construct_spritesheet(std::weak_ptr<graphite::qd::surface> surface) -> void
{
    if (auto s = surface.lock()) {
        m_spritesheet = gl::spritesheet::create(s, s->size(), 1);
    }
}

auto kestrel::assets::static_image::reconfigure_spritesheet(const math::size& sprite_size, int total_sprites) -> void
{
    m_spritesheet->configure_sprites(sprite_size, total_sprites);
}

auto kestrel::assets::static_image::lua_reconfigure_spritesheet(std::vector<double> size, int total_sprites) -> void
{
    reconfigure_spritesheet({ size[0], size[1] }, total_sprites);
}

// MARK: - Accessors

auto kestrel::assets::static_image::valid() const -> bool
{
    return (m_spritesheet != nullptr);
}

auto kestrel::assets::static_image::spritesheet() -> std::weak_ptr<gl::spritesheet>
{
    return m_spritesheet;
}

auto kestrel::assets::static_image::count() const -> int
{
    return m_spritesheet->sprite_count();
}

auto kestrel::assets::static_image::size() const -> math::size
{
    return m_spritesheet->texture()->size();
}

// MARK: - Drawing

auto kestrel::assets::static_image::lua_draw(std::vector<double> position) const -> void
{
    draw(math::vector(position[0], position[1]));
}

auto kestrel::assets::static_image::draw(const math::vector& v, int frame) const -> void
{
    if (m_spritesheet) {
        if (auto sprite = m_spritesheet->sprite(frame).lock()) {
            sprite->draw(v, *m_spritesheet);
        }
    }
}
