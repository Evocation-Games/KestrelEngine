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
            .addStaticFunction("load", &asset::static_image::load_best)
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

asset::static_image::static_image(const asset::resource::lua_reference &ref)
{
    int64_t id;
    std::string type { asset::static_image::type };

    // Determine which _type_ of resource we are going to be trying to load. We need to follow slightly different
    // paths for each valid type in order to be able to use the correct decoders.
    if (ref->type().has_value()) {
        type = ref->type().value();
    }

    if (ref->id().has_value()) {
        id = ref->id().value();
    }
    else {
        throw std::logic_error("Bad resource reference encountered: Missing resource id.");
    }

    // Attempt to load the resource data in preparation for determining the correct decoding procedure.
    if (auto res = graphite::rsrc::manager::shared_manager().find(type, id).lock()) {
        if (type == asset::macintosh_picture::type) {
            graphite::qd::pict pict(res->data(), res->id(), res->name());
            if (auto surface = pict.image_surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
        else if (type == asset::color_icon::type) {
            graphite::qd::cicn cicn(res->data(), res->id(), res->name());
            if (auto surface = cicn.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
        else if (type == asset::static_image::type) {
            asset::tga tga(res->data()->get());
            if (auto surface = tga.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                return;
            }
        }
    }

    throw std::logic_error("Bad resource reference encountered in StaticImage: Unable to load resource: " + type + " #" + std::to_string(id));
}

auto asset::static_image::load_best(std::vector<asset::resource::lua_reference> refs) -> static_image::lua_reference
{
    // Determine the first resource that actually exists. This is the one that we need to use.
    std::optional<asset::resource::lua_reference> ref;
    for (const auto& r : refs) {
        if (r->type().has_value() && r->id().has_value() && r->exists()) {
            ref = r;
            break;
        }
    }

    if (!ref.has_value()) {
        throw std::logic_error("No valid resource references passed to StaticImage");
    }

    // Extract the type and id of the reference.
    std::string type { ref.value()->type().value() };
    int64_t id { ref.value()->id().value()};

    // Check the environment for a pre-cached version of the resource.
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(type, ref.value());
        if (asset.has_value()) {
            if (type == asset::macintosh_picture::type) {
                return asset::static_image::using_pict(std::any_cast<asset::macintosh_picture::lua_reference>(asset.value()));
            }
            else if (type == asset::color_icon::type) {
                return asset::static_image::using_cicn(std::any_cast<asset::color_icon::lua_reference>(asset.value()));
            }
            else if (type == asset::static_image::type) {
                return std::any_cast<asset::static_image::lua_reference>(asset.value());
            }
            else {
                throw std::logic_error("Unexpected resource type encountered in StaticImage: " + type + " #" + std::to_string(id));
            }
        }
    }

    // We couldn't de-cache, so load the asset from the manager fresh.
    if (type == asset::macintosh_picture::type) {
        auto image = asset::macintosh_picture::lua_reference(new asset::macintosh_picture(ref.value()));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(type, ref.value(), image);
        }
        return asset::static_image::using_pict(image);
    }
    else if (type == asset::color_icon::type) {
        auto image = asset::color_icon::lua_reference(new asset::color_icon(ref.value()));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(type, ref.value(), image);
        }
        return asset::static_image::using_cicn(image);
    }
    else if (type == asset::static_image::type) {
        auto image = asset::static_image::lua_reference(new asset::static_image(ref.value()));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(type, ref.value(), image);
        }
        return image;
    }

    throw std::logic_error("Unable to load StaitcImage from provided resource references. Chose: " + type + " #" + std::to_string(id));
}

auto asset::static_image::using_pict(const asset::macintosh_picture::lua_reference &ref) -> static_image::lua_reference
{
    return asset::static_image::lua_reference(new asset::static_image(ref->id(), ref->name(), ref->spritesheet()));
}

auto asset::static_image::using_cicn(const asset::color_icon::lua_reference &ref) -> static_image::lua_reference
{
    return asset::static_image::lua_reference(new asset::static_image(ref->id(), ref->name(), ref->spritesheet()));
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