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
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/image/tga.hpp>

// MARK: - Construction

kestrel::image::static_image::static_image(graphite::rsrc::resource::identifier id, const std::string& name, const std::shared_ptr<graphics::sprite_sheet>& sheet)
{
    configure(id, name, sheet);
}

kestrel::image::static_image::static_image(const math::size &size, const graphics::color::lua_reference &color)
    : basic_image(size, color)
{
}

kestrel::image::static_image::static_image(const resource::descriptor::lua_reference &ref)
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
                attributes.emplace_back("namespace", ref->namespaces.front());
            }
        }

        if (graphite::rsrc::manager::shared_manager().find(resource_type::code, descriptor->id, attributes)) {
            descriptor = descriptor->with_type(resource_type::code);
        }
        else if (graphite::rsrc::manager::shared_manager().find(legacy::macintosh::quickdraw::picture::resource_type::code, descriptor->id, attributes)) {
            descriptor = descriptor->with_type(legacy::macintosh::quickdraw::picture::resource_type::code);
        }
        else if (graphite::rsrc::manager::shared_manager().find(legacy::macintosh::quickdraw::color_icon::resource_type::code, descriptor->id, attributes)) {
            descriptor = descriptor->with_type(legacy::macintosh::quickdraw::color_icon::resource_type::code);
        }
    }

    // Attempt to load the resource data in preparation for determining the correct decoding procedure.
    if (auto resource = descriptor->load()) {
        if (descriptor->type == legacy::macintosh::quickdraw::picture::resource_type::code) {
            graphite::quickdraw::pict pict(resource->data(), resource->id(), resource->name());
            const auto& surface = pict.surface();
            configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
            return;
        }
        else if (descriptor->type == legacy::macintosh::quickdraw::color_icon::resource_type::code) {
            graphite::quickdraw::cicn cicn(resource->data(), resource->id(), resource->name());
            const auto& surface = cicn.surface();
            configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
            return;
        }
        else if (descriptor->type == resource_type::code) {
            graphite::data::reader reader(&resource->data());
            reader.change_byte_order(graphite::data::byte_order::msb);
            auto format = reader.read_cstr(4);

            if (format == "TGA ") {
                auto tga_raw_data = reader.read_data(reader.size() - 4);
                kestrel::image::tga tga(tga_raw_data);

                const auto& surface = tga.surface();
                configure(resource->id(), resource->name(), math::size(surface.size().width, surface.size().height), surface.raw());
            }
            else {
                throw std::runtime_error("Unrecognised StaticImage format: '" + format + "' in resource: " + descriptor->type + " #" + std::to_string(descriptor->id));
            }

            return;
        }
    }

    throw std::logic_error("Bad resource reference encountered in StaticImage: Unable to load resource: " + descriptor->type + " #" + std::to_string(descriptor->id));
}

auto kestrel::image::static_image::load_best(const std::vector<resource::descriptor::lua_reference>& refs) -> lua_reference
{
    // Determine the first resource that actually exists. This is the one that we need to use.
    std::optional<resource::descriptor::lua_reference> oref;
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
    auto asset = cache::fetch(ref);
    if (asset.has_value()) {
        if (ref->type == legacy::macintosh::quickdraw::picture::resource_type::code) {
            return static_image::using_pict(std::any_cast<legacy::macintosh::quickdraw::picture::lua_reference>(asset.value()));
        }
        else if (ref->type == legacy::macintosh::quickdraw::color_icon::resource_type::code) {
            return static_image::using_cicn(std::any_cast<legacy::macintosh::quickdraw::color_icon::lua_reference>(asset.value()));
        }
        else if (ref->type == resource_type::code) {
            return std::any_cast<static_image::lua_reference>(asset.value());
        }
        else {
            throw std::logic_error("Unexpected resource type encountered in StaticImage: " + ref->type + " #" + std::to_string(ref->id));
        }
    }

    // We couldn't de-cache, so load the asset from the manager fresh.
    if (ref->type == legacy::macintosh::quickdraw::picture::resource_type::code) {
        auto image = legacy::macintosh::quickdraw::picture::lua_reference(new legacy::macintosh::quickdraw::picture(ref));
        cache::add(ref, image);
        return static_image::using_pict(image);
    }
    else if (ref->type == legacy::macintosh::quickdraw::color_icon::resource_type::code) {
        auto image = legacy::macintosh::quickdraw::color_icon::lua_reference(new legacy::macintosh::quickdraw::color_icon(ref));
        cache::add(ref, image);
        return static_image::using_cicn(image);
    }
    else if (ref->type == resource_type::code) {
        auto image = static_image::lua_reference(new static_image(ref));
        cache::add(ref, image);
        return image;
    }

    throw std::logic_error("Unable to load StaitcImage from provided resource references. Chose: " + ref->type + " #" + std::to_string(ref->id));
}

auto kestrel::image::static_image::using_pict(const legacy::macintosh::quickdraw::picture::lua_reference &ref) -> lua_reference
{
    return { new static_image(ref->id(), ref->name(), ref->sprite_sheet()) };
}

auto kestrel::image::static_image::using_cicn(const legacy::macintosh::quickdraw::color_icon::lua_reference &ref) -> lua_reference
{
    return { new static_image(ref->id(), ref->name(), ref->sprite_sheet()) };
}

auto kestrel::image::static_image::preferred(const resource::descriptor::lua_reference &ref) -> lua_reference
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
        resource_type::code,
        legacy::macintosh::quickdraw::picture::resource_type::code,
        legacy::macintosh::quickdraw::color_icon::resource_type::code
    };

    for (const auto& type : type_order) {
        const auto& typed_ref = ref->with_type(type);
        if (typed_ref->valid()) {
            return { new static_image(typed_ref) };
        }
    }

    return nullptr;
}

auto kestrel::image::static_image::from(const luabridge::LuaRef &ref) -> lua_reference
{
    if (lua::ref_isa<static_image>(ref)) {
        return ref.cast<static_image::lua_reference>();
    }
    else if (lua::ref_isa<legacy::macintosh::quickdraw::picture>(ref)) {
        const auto& pict = ref.cast<legacy::macintosh::quickdraw::picture::lua_reference>();
        return static_image::using_pict(pict);
    }
    else if (lua::ref_isa<legacy::macintosh::quickdraw::color_icon>(ref)) {
        const auto& cicn = ref.cast<legacy::macintosh::quickdraw::color_icon::lua_reference>();
        return static_image::using_cicn(cicn);
    }
    else if (lua::ref_isa<resource::descriptor>(ref)) {
        auto descriptor = ref.cast<resource::descriptor::lua_reference>();

        // TODO: Refactor this to remove the exception here...
        try {
            return { new static_image(descriptor) };
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

auto kestrel::image::static_image::size() const -> math::size
{
    return basic_image::size();
}

auto kestrel::image::static_image::sprite_count() const -> std::size_t
{
    return basic_image::sprite_count();
}

// MARK: - Sprites & Entities

auto kestrel::image::static_image::spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity>
{
    return basic_image::spawn_entity(position);
}
