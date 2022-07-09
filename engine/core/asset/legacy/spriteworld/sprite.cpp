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
#include <libGraphite/spriteworld/rle.hpp>
#include "sprite.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::legacy::spriteworld::sprite::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("SpriteWorld")
                .beginClass<sprite>("Sprite")
                    .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
                    .addStaticFunction("load", &sprite::load)
                    .addProperty("size", &sprite::size)
                    .addProperty("numberOfSprites", &sprite::sprite_count)
                    .addFunction("spawnEntity", &sprite::spawn_entity)
                    .addFunction("setSpriteSize", &sprite::layout_sprites)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::legacy::spriteworld::sprite::sprite(const asset::resource_descriptor::lua_reference& ref)
{
    if (auto resource = ref->with_type(type_16)->load()) {
        m_source_type = std::string(type_16);
        graphite::spriteworld::rle<16> rle(resource->data(), resource->id(), resource->name());
        m_surface = std::move(rle.surface());
        configure(resource->id(), resource->name(), { m_surface.size().width, m_surface.size().height }, m_surface.raw());

        auto frame_size = rle.frame_rect(0).size;
        layout_sprites({ frame_size.width, frame_size.height });

        return;
    }
    else if (auto resource = ref->with_type(type_32)->load()) {
        m_source_type = std::string(type_32);
        graphite::spriteworld::rle<32> rle(resource->data(), resource->id(), resource->name());
        m_surface = std::move(rle.surface());
        configure(resource->id(), resource->name(), { m_surface.size().width, m_surface.size().height }, m_surface.raw());

        auto frame_size = rle.frame_rect(0).size;
        layout_sprites({ frame_size.width, frame_size.height });

        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::legacy::spriteworld::sprite::load(const asset::resource_descriptor::lua_reference& ref) -> lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(sprite::type_32, ref);
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }

        asset = env->cache()->fetch(sprite::type_16, ref);
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }
    }

    auto image = lua_reference(new sprite(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(image->m_source_type, ref, image);
    }
    return image;
}

// MARK: - Properties

auto asset::legacy::spriteworld::sprite::size() const -> math::size
{
    return basic_image::size();
}

auto asset::legacy::spriteworld::sprite::sprite_count() const -> int
{
    return basic_image::sprite_count();
}

// MARK: - Layout

auto asset::legacy::spriteworld::sprite::layout_sprites(const math::size& sprite_size) -> void
{
    asset::basic_image::layout_sprites(sprite_size);
}

// MARK: - Entities

auto asset::legacy::spriteworld::sprite::spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>
{
    return asset::basic_image::spawn_entity(position);
}
