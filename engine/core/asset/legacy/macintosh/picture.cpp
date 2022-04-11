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
#include <libGraphite/quickdraw/pict.hpp>
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::legacy::macintosh::quickdraw::picture::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<picture>("Picture")
                    .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
                    .addStaticFunction("load", &picture::load)
                    .addProperty("size", &picture::size)
                    .addProperty("numberOfSprites", &picture::sprite_count)
                    .addFunction("spawnEntity", &picture::spawn_entity)
                    .addFunction("setSpriteSize", &picture::layout_sprites)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::legacy::macintosh::quickdraw::picture::picture(const asset::resource_descriptor::lua_reference& ref)
{
    if (auto res = ref->with_type(type)->load().lock()) {
        graphite::qd::pict pict(res->data(), res->id(), res->name());
        if (auto surface = pict.image_surface().lock()) {
            configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
            return;
        }
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::legacy::macintosh::quickdraw::picture::load(const asset::resource_descriptor::lua_reference& ref) -> lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(picture::type, ref);
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }
    }

    auto image = lua_reference(new picture(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(picture::type, ref, image);
    }
    return image;
}

// MARK: - Accessors

auto asset::legacy::macintosh::quickdraw::picture::size() const -> math::size
{
    return basic_image::size();
}

auto asset::legacy::macintosh::quickdraw::picture::sprite_count() const -> int
{
    return basic_image::sprite_count();
}

// MARK: - Sprites & Entities

auto asset::legacy::macintosh::quickdraw::picture::layout_sprites(const math::size& sprite_size) -> void
{
    asset::basic_image::layout_sprites(sprite_size);
}

auto asset::legacy::macintosh::quickdraw::picture::spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>
{
    return asset::basic_image::spawn_entity(position);
}

