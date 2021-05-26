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

#include <stdexcept>
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/quickdraw/rle.hpp>
#include "sprite.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::sprite::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("SpriteWorld")
                .beginClass<asset::sprite>("Sprite")
                    .addConstructor<auto(*)(const asset::resource_reference::lua_reference&)->void, asset::sprite::lua_reference>()
                    .addStaticFunction("load", &asset::sprite::load)
                    .addProperty("size", &asset::sprite::size)
                    .addProperty("numberOfSprites", &asset::sprite::sprite_count)
                    .addFunction("spawnEntity", &asset::sprite::spawn_entity)
                    .addFunction("setSpriteSize", &asset::sprite::layout_sprites)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::sprite::sprite(const asset::resource_reference::lua_reference& ref)
{
    if (ref->id().has_value()) {
        if (auto res = graphite::rsrc::manager::shared_manager().find(sprite::type, ref->id().value()).lock()) {
            graphite::qd::rle rle(res->data(), res->id(), res->name());
            if (auto surface = rle.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::sprite::load(const asset::resource_reference::lua_reference& ref) -> sprite::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(sprite::type, ref);
        if (asset.has_value()) {
            return std::any_cast<asset::sprite::lua_reference>(asset.value());
        }
    }

    auto image = asset::sprite::lua_reference(new asset::sprite(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(sprite::type, ref, image);
    }
    return image;
}

// MARK: - Properties

auto asset::sprite::size() const -> math::size
{
    return basic_image::size();
}

auto asset::sprite::sprite_count() const -> int
{
    return basic_image::sprite_count();
}

// MARK: - Layout

auto asset::sprite::layout_sprites(const math::size& sprite_size) -> void
{
    asset::basic_image::layout_sprites(sprite_size);
}

// MARK: - Entities

auto asset::sprite::spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference
{
    return asset::basic_image::spawn_entity(position);
}
