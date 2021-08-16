// Copyright (c) 2021 Tom Hancocks
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

#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>
#include <libGraphite/quickdraw/pict.hpp>
#include <libGraphite/quickdraw/cicn.hpp>
#include "core/asset/static_image.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"
#include "core/asset/tga.hpp"

// MARK: - Environment

auto asset::static_image::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<asset::static_image>("StaticImage")
            .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
            .addStaticFunction("load", &asset::static_image::load_best)
            .addStaticFunction("preferred", &asset::static_image::preferred)
            .addStaticFunction("usingLegacyMacintoshPicture", &asset::static_image::using_pict)
            .addStaticFunction("usingLegacyMacintoshColorIcon", &asset::static_image::using_cicn)
            .addProperty("size", &asset::static_image::size)
            .addFunction("spawnEntity", &asset::static_image::spawn_entity)
        .endClass();
}

// MARK: - Construction

asset::static_image::static_image(const int64_t& id, const std::string& name, std::shared_ptr<graphics::spritesheet> sheet)
{
    configure(id, name, sheet);
}

asset::static_image::static_image(const asset::resource_descriptor::lua_reference &ref)
{
    if (!ref->has_id()) {
        throw std::logic_error("Bad resource reference encountered: Missing resource id.");
    }

    // Build a descriptor and ensure it has a type. If no type has been supplied in the reference, then
    // use the static image type.
    auto descriptor = ref->with_type(ref->has_type() ? ref->type : asset::static_image::type);

    // Attempt to load the resource data in preparation for determining the correct decoding procedure.
    if (auto res = descriptor->load().lock()) {
        if (ref->type == asset::macintosh_picture::type) {
            graphite::qd::pict pict(res->data(), res->id(), res->name());
            if (auto surface = pict.image_surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
        else if (ref->type == asset::color_icon::type) {
            graphite::qd::cicn cicn(res->data(), res->id(), res->name());
            if (auto surface = cicn.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
        else if (ref->type == asset::static_image::type) {
            asset::tga tga(res->data()->get());
            if (auto surface = tga.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
    }

    throw std::logic_error("Bad resource reference encountered in StaticImage: Unable to load resource: " + ref->type + " #" + std::to_string(ref->id));
}

auto asset::static_image::load_best(std::vector<asset::resource_descriptor::lua_reference> refs) -> static_image::lua_reference
{
    // Determine the first resource that actually exists. This is the one that we need to use.
    std::optional<asset::resource_descriptor::lua_reference> oref;
    for (const auto& r : refs) {
        if (r->has_type() && r->has_id() && r->valid()) {
            oref = r;
            break;
        }
    }

    if (!oref.has_value()) {
        throw std::logic_error("No valid resource references passed to StaticImage");
    }
    auto ref = oref.value();

    // Check the environment for a pre-cached version of the resource.
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(ref->type, ref);
        if (asset.has_value()) {
            if (ref->type == asset::macintosh_picture::type) {
                return asset::static_image::using_pict(std::any_cast<asset::macintosh_picture::lua_reference>(asset.value()));
            }
            else if (ref->type == asset::color_icon::type) {
                return asset::static_image::using_cicn(std::any_cast<asset::color_icon::lua_reference>(asset.value()));
            }
            else if (ref->type == asset::static_image::type) {
                return std::any_cast<asset::static_image::lua_reference>(asset.value());
            }
            else {
                throw std::logic_error("Unexpected resource type encountered in StaticImage: " + ref->type + " #" + std::to_string(ref->id));
            }
        }
    }

    // We couldn't de-cache, so load the asset from the manager fresh.
    if (ref->type == asset::macintosh_picture::type) {
        auto image = asset::macintosh_picture::lua_reference(new asset::macintosh_picture(ref));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(ref->type, ref, image);
        }
        return asset::static_image::using_pict(image);
    }
    else if (ref->type == asset::color_icon::type) {
        auto image = asset::color_icon::lua_reference(new asset::color_icon(ref));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(ref->type, ref, image);
        }
        return asset::static_image::using_cicn(image);
    }
    else if (ref->type == asset::static_image::type) {
        auto image = asset::static_image::lua_reference(new asset::static_image(ref));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(ref->type, ref, image);
        }
        return image;
    }

    throw std::logic_error("Unable to load StaitcImage from provided resource references. Chose: " + ref->type + " #" + std::to_string(ref->id));
}

auto asset::static_image::using_pict(const asset::macintosh_picture::lua_reference &ref) -> static_image::lua_reference
{
    return asset::static_image::lua_reference(new asset::static_image(ref->id(), ref->name(), ref->spritesheet()));
}

auto asset::static_image::using_cicn(const asset::color_icon::lua_reference &ref) -> static_image::lua_reference
{
    return asset::static_image::lua_reference(new asset::static_image(ref->id(), ref->name(), ref->spritesheet()));
}

auto asset::static_image::preferred(const asset::resource_descriptor::lua_reference &ref) -> asset::static_image::lua_reference
{
    if (ref->has_type()) {
        if (ref->valid()) {
            return { new static_image(ref) };
        }
        else {
            return nullptr;
        }
    }

    std::vector<std::string> type_order {
        static_image::type,
        macintosh_picture::type,
        color_icon::type
    };

    for (const auto& type : type_order) {
        const auto& typed_ref = ref->with_type(type);
        if (typed_ref->valid()) {
            return { new static_image(typed_ref) };
        }
    }

    return nullptr;
}


// MARK: - Accessors

auto asset::static_image::size() const -> math::size
{
    return basic_image::size();
}

auto asset::static_image::sprite_count() const -> int
{
    return basic_image::sprite_count();
}

// MARK: - Sprites & Entities

auto asset::static_image::spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference
{
    return asset::basic_image::spawn_entity(position);
}