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
#include <libGraphite/quickdraw/format/pict.hpp>
#include <libGraphite/quickdraw/format/cicn.hpp>
#include "core/asset/static_image.hpp"
#include "core/asset/spritesheet.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"
#include "core/asset/tga.hpp"

// MARK: - Environment

auto asset::static_image::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<static_image>("StaticImage")
            .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
            .addStaticFunction("load", &static_image::load_best)
            .addStaticFunction("preferred", &static_image::preferred)
            .addStaticFunction("usingLegacyMacintoshPicture", &static_image::using_pict)
            .addStaticFunction("usingLegacyMacintoshColorIcon", &static_image::using_cicn)
            .addProperty("size", &static_image::size)
            .addFunction("spawnEntity", &static_image::spawn_entity)
        .endClass();
}

// MARK: - Construction

asset::static_image::static_image(const int64_t& id, const std::string& name, const std::shared_ptr<graphics::spritesheet>& sheet)
{
    configure(id, name, sheet);
}

asset::static_image::static_image(const math::size &size, const graphics::color::lua_reference &color)
    : asset::basic_image(size, color)
{
}

asset::static_image::static_image(const resource_descriptor::lua_reference &ref)
{
    if (!ref->has_id()) {
        throw std::logic_error("Bad resource reference encountered: Missing resource id.");
    }

    // Build a descriptor and ensure it has a type. If no type has been supplied in the reference, then
    // use the static image type.
    auto descriptor = ref;
    if (!ref->has_type()) {
        std::vector<graphite::rsrc::attribute> attributes;
        if (ref->is_namespaced()) {
            if (!ref->namespaces.front().empty()) {
                attributes.emplace_back(graphite::rsrc::attribute("namespace", ref->namespaces.front()));
            }
        }

        if (graphite::rsrc::manager::shared_manager().find(static_image::type, descriptor->id, attributes)) {
            descriptor = descriptor->with_type(static_image::type);
        }
        else if (graphite::rsrc::manager::shared_manager().find(legacy::macintosh::quickdraw::picture::type, descriptor->id, attributes)) {
            descriptor = descriptor->with_type(legacy::macintosh::quickdraw::picture::type);
        }
        else if (graphite::rsrc::manager::shared_manager().find(legacy::macintosh::quickdraw::color_icon::type, descriptor->id, attributes)) {
            descriptor = descriptor->with_type(legacy::macintosh::quickdraw::color_icon::type);
        }
    }

    // Attempt to load the resource data in preparation for determining the correct decoding procedure.
    if (auto resource = descriptor->load()) {
        if (descriptor->type == legacy::macintosh::quickdraw::picture::type) {
            graphite::quickdraw::pict pict(resource->data(), resource->id(), resource->name());
            const auto& surface = pict.surface();
            configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
            return;
        }
        else if (descriptor->type == legacy::macintosh::quickdraw::color_icon::type) {
            graphite::quickdraw::cicn cicn(resource->data(), resource->id(), resource->name());
            const auto& surface = cicn.surface();
            configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
            return;
        }
        else if (descriptor->type == static_image::type) {
            graphite::data::reader reader(&resource->data());
            reader.change_byte_order(graphite::data::byte_order::msb);
            auto tga_raw_data = std::make_shared<std::vector<char>>(reader.read_bytes(reader.size()));
            tga tga(tga_raw_data);

            const auto& surface = tga.surface();
            configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
            return;
        }
    }

    throw std::logic_error("Bad resource reference encountered in StaticImage: Unable to load resource: " + descriptor->type + " #" + std::to_string(descriptor->id));
}

auto asset::static_image::load_best(const std::vector<resource_descriptor::lua_reference>& refs) -> lua_reference
{
    // Determine the first resource that actually exists. This is the one that we need to use.
    std::optional<resource_descriptor::lua_reference> oref;
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
        auto asset = env->cache()->fetch(ref);
        if (asset.has_value()) {
            if (ref->type == legacy::macintosh::quickdraw::picture::type) {
                return static_image::using_pict(std::any_cast<legacy::macintosh::quickdraw::picture::lua_reference>(asset.value()));
            }
            else if (ref->type == legacy::macintosh::quickdraw::color_icon::type) {
                return static_image::using_cicn(std::any_cast<legacy::macintosh::quickdraw::color_icon::lua_reference>(asset.value()));
            }
            else if (ref->type == static_image::type) {
                return std::any_cast<static_image::lua_reference>(asset.value());
            }
            else {
                throw std::logic_error("Unexpected resource type encountered in StaticImage: " + ref->type + " #" + std::to_string(ref->id));
            }
        }
    }

    // We couldn't de-cache, so load the asset from the manager fresh.
    if (ref->type == legacy::macintosh::quickdraw::picture::type) {
        auto image = legacy::macintosh::quickdraw::picture::lua_reference(new legacy::macintosh::quickdraw::picture(ref));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(ref, image);
        }
        return asset::static_image::using_pict(image);
    }
    else if (ref->type == legacy::macintosh::quickdraw::color_icon::type) {
        auto image = legacy::macintosh::quickdraw::color_icon::lua_reference(new legacy::macintosh::quickdraw::color_icon(ref));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(ref, image);
        }
        return static_image::using_cicn(image);
    }
    else if (ref->type == static_image::type) {
        auto image = static_image::lua_reference(new static_image(ref));
        if (auto env = environment::active_environment().lock()) {
            env->cache()->add(ref, image);
        }
        return image;
    }

    throw std::logic_error("Unable to load StaitcImage from provided resource references. Chose: " + ref->type + " #" + std::to_string(ref->id));
}

auto asset::static_image::using_pict(const legacy::macintosh::quickdraw::picture::lua_reference &ref) -> static_image::lua_reference
{
    return { new static_image(ref->id(), ref->name(), ref->sprite_sheet()) };
}

auto asset::static_image::using_cicn(const legacy::macintosh::quickdraw::color_icon::lua_reference &ref) -> static_image::lua_reference
{
    return { new asset::static_image(ref->id(), ref->name(), ref->sprite_sheet()) };
}

auto asset::static_image::preferred(const resource_descriptor::lua_reference &ref) -> static_image::lua_reference
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
        legacy::macintosh::quickdraw::picture::type,
        legacy::macintosh::quickdraw::color_icon::type
    };

    for (const auto& type : type_order) {
        const auto& typed_ref = ref->with_type(type);
        if (typed_ref->valid()) {
            return { new static_image(typed_ref) };
        }
    }

    return nullptr;
}

auto asset::static_image::from(const luabridge::LuaRef &ref) -> lua_reference
{
    if (scripting::lua::ref_isa<asset::static_image>(ref)) {
        return ref.cast<asset::static_image::lua_reference>();
    }
    else if (scripting::lua::ref_isa<asset::legacy::macintosh::quickdraw::picture>(ref)) {
        const auto& pict = ref.cast<asset::legacy::macintosh::quickdraw::picture::lua_reference>();
        return asset::static_image::using_pict(pict);
    }
    else if (scripting::lua::ref_isa<asset::legacy::macintosh::quickdraw::color_icon>(ref)) {
        const auto& cicn = ref.cast<asset::legacy::macintosh::quickdraw::color_icon::lua_reference>();
        return asset::static_image::using_cicn(cicn);
    }
    else if (scripting::lua::ref_isa<asset::resource_descriptor>(ref)) {
        auto descriptor = ref.cast<asset::resource_descriptor::lua_reference>();

        // TODO: Refactor this to remove the exception here...
        try {
            return { new asset::static_image(descriptor) };
        }
        catch (const std::exception& exception) {
            return nullptr;
        }
    }
    else {
        return nullptr;
    }
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

auto asset::static_image::spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>
{
    return basic_image::spawn_entity(position);
}
