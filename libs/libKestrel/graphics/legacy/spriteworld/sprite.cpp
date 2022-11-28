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
#include <libGraphite/spriteworld/rleD.hpp>
#include <libGraphite/spriteworld/rleX.hpp>
#include <libKestrel/graphics/legacy/spriteworld/sprite.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libKestrel/kestrel.hpp>

// MARK: - Construction

kestrel::image::legacy::spriteworld::sprite::sprite(const resource::descriptor::lua_reference& ref)
{
    if (ref->has_type() && ref->type == resource_type::code) {
        if (auto resource = ref->with_type(resource_type::code)->load()) {
            m_source_type = std::string(resource_type::code);
            graphite::spriteworld::rleD rle(resource->data(), resource->id(), resource->name());
            m_surface = rle.surface();
            configure(resource->id(),
                      resource->name(), {
                          static_cast<double>(m_surface.size().width),
                          static_cast<double>(m_surface.size().height)
                      },
                      m_surface.raw());

            auto frame_size = rle.frame_rect(0).size;
            layout_sprites({ static_cast<double>(frame_size.width), static_cast<double>(frame_size.height) });

            return;
        }
    }
    else if (ref->has_type() && ref->type == alternate_type::code) {
        if (auto resource = ref->with_type(alternate_type::code)->load()) {
            m_source_type = std::string(alternate_type::code);
            graphite::spriteworld::rleX rle(resource->data(), resource->id(), resource->name());
            m_surface = rle.surface();
            configure(resource->id(),
                      resource->name(), {
                          static_cast<double>(m_surface.size().width),
                          static_cast<double>(m_surface.size().height)
                      },
                      m_surface.raw());

            auto frame_size = rle.frame_rect(0).size;
            layout_sprites({ static_cast<double>(frame_size.width), static_cast<double>(frame_size.height) });

            return;
        }
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto kestrel::image::legacy::spriteworld::sprite::load(const resource::descriptor::lua_reference& ref) -> lua_reference
{
    // Attempt to de-cache asset
    if (ref->has_type()) {
        auto asset = cache::fetch(ref);
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }
    }
    else {
        auto asset = cache::fetch(ref->with_type(alternate_type::code));
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }

        asset = cache::fetch(ref->with_type(resource_type::code));
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }
    }

    auto image = lua_reference(new sprite(ref));
    cache::add(ref->with_type(image->type()), image);
    return image;
}

// MARK: - Properties

auto kestrel::image::legacy::spriteworld::sprite::type() const -> std::string
{
    return m_source_type;
}

auto kestrel::image::legacy::spriteworld::sprite::size() const -> math::size
{
    return basic_image::size();
}

auto kestrel::image::legacy::spriteworld::sprite::sprite_count() const -> std::size_t
{
    return basic_image::sprite_count();
}

// MARK: - Layout

auto kestrel::image::legacy::spriteworld::sprite::layout_sprites(const math::size& sprite_size) -> void
{
    basic_image::layout_sprites(sprite_size);
}

// MARK: - Entities

auto kestrel::image::legacy::spriteworld::sprite::spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity>
{
    return basic_image::spawn_entity(position);
}
