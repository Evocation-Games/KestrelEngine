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
#include <libResourceCore/manager.hpp>
#include <libQuickdraw/format/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libKestrel/kestrel.hpp>

// MARK: - Construction

kestrel::image::legacy::macintosh::quickdraw::picture::picture(const resource::descriptor::lua_reference& ref)
{
    if (auto resource = ref->with_type(resource_type::code)->load()) {
        ::quickdraw::picture pict(resource->data(), resource->id(), resource->name());
        const auto& surface = pict.surface();
        configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto kestrel::image::legacy::macintosh::quickdraw::picture::load(const resource::descriptor::lua_reference& ref) -> lua_reference
{
    // Attempt to de-cache asset
    auto asset = cache::fetch(ref->with_type(resource_type::code));
    if (asset.has_value()) {
        return std::any_cast<lua_reference>(asset.value());
    }

    auto image = lua_reference(new picture(ref));
    cache::add(ref->with_type(resource_type::code), image);
    return image;
}

// MARK: - Accessors

auto kestrel::image::legacy::macintosh::quickdraw::picture::size() const -> math::size
{
    return basic_image::size();
}

auto kestrel::image::legacy::macintosh::quickdraw::picture::sprite_count() const -> std::size_t
{
    return basic_image::sprite_count();
}

// MARK: - Sprites & Entities

auto kestrel::image::legacy::macintosh::quickdraw::picture::layout_sprites(const math::size& sprite_size) -> void
{
    basic_image::layout_sprites(sprite_size);
}

auto kestrel::image::legacy::macintosh::quickdraw::picture::spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity>
{
    return basic_image::spawn_entity(position);
}

