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
#include <libGraphite/quickdraw/format/cicn.hpp>
#include "color_icon.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::legacy::macintosh::quickdraw::color_icon::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<color_icon>("ColorIcon")
                    .addConstructor<auto(*)(const resource_descriptor::lua_reference&)->void, color_icon::lua_reference>()
                    .addStaticFunction("load", &color_icon::load)
                    .addProperty("size", &color_icon::size)
                    .addFunction("spawnEntity", &color_icon::spawn_entity)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::legacy::macintosh::quickdraw::color_icon::color_icon(const asset::resource_descriptor::lua_reference &ref)
{
    if (auto resource = ref->with_type(type)->load()) {
        graphite::quickdraw::cicn icon(resource->data(), resource->id(), resource->name());
        const auto& surface = icon.surface();
        configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::legacy::macintosh::quickdraw::color_icon::load(const asset::resource_descriptor::lua_reference &ref) -> color_icon::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(ref->with_type(type));
        if (asset.has_value()) {
            return std::any_cast<color_icon::lua_reference>(asset.value());
        }
    }

    auto icon = asset::legacy::macintosh::quickdraw::color_icon::lua_reference(new color_icon(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(ref->with_type(type), icon);
    }
    return icon;
}

// MARK: - Properties

auto asset::legacy::macintosh::quickdraw::color_icon::size() const -> math::size
{
    return basic_image::size();
}

// MARK: - Entities

auto asset::legacy::macintosh::quickdraw::color_icon::spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>
{
    return asset::basic_image::spawn_entity(position);
}