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
#include <libGraphite/quickdraw/cicn.hpp>
#include "color_icon.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::color_icon::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<asset::color_icon>("ColorIcon")
                    .addConstructor<auto(*)(const asset::resource::lua_reference&)->void, asset::color_icon::lua_reference>()
                    .addStaticFunction("load", &asset::color_icon::load)
                    .addProperty("size", &asset::color_icon::size)
                    .addFunction("spawnEntity", &asset::color_icon::spawn_entity)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::color_icon::color_icon(const asset::resource::lua_reference &ref)
{
    if (ref->id().has_value()) {
        if (auto res = graphite::rsrc::manager::shared_manager().find(color_icon::type, ref->id().value()).lock()) {
            graphite::qd::cicn icon(res->data(), res->id(), res->name());
            if (auto surface = icon.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::color_icon::load(const asset::resource::lua_reference &ref) -> color_icon::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(color_icon::type, ref);
        if (asset.has_value()) {
            return std::any_cast<asset::color_icon::lua_reference>(asset.value());
        }
    }

    auto icon = asset::color_icon::lua_reference(new asset::color_icon(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(color_icon::type, ref, icon);
    }
    return icon;
}

// MARK: - Properties

auto asset::color_icon::size() const -> math::size
{
    return basic_image::size();
}

// MARK: - Entities

auto asset::color_icon::spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference
{
    return asset::basic_image::spawn_entity(position);
}