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

#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>
#include "core/asset/spritesheet.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"
#include "core/asset/tga.hpp"

// MARK: - Lua

auto asset::spritesheet::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<asset::spritesheet>("SpriteSheet")
        .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, asset::spritesheet::lua_reference>()
            .addStaticFunction("load", &asset::spritesheet::load)
            .addProperty("size", &asset::spritesheet::size)
            .addProperty("numberOfSprites", &asset::spritesheet::sprite_count)
            .addFunction("spawnEntity", &asset::spritesheet::spawn_entity)
        .endClass();
}

// MARK: - Construction

asset::spritesheet::spritesheet(const asset::resource_descriptor::lua_reference& ref)
{
    if (auto res = ref->with_type(type)->load().lock()) {
        // The format of the SpriteSheet resource is as follows:
        //
        //  uint16_t    format_version      = 1
        //  uint16_t    frames - 1
        //      uint16_t    x
        //      uint16_t    y
        //      uint16_t    width
        //      uint16_t    height
        //  uint32_t    tga_size
        //  uint8_t...  tga
        graphite::data::reader reader(res->data(), graphite::data::msb);

        if (reader.read_short() == 1) {
            // Read the initial meta data
            auto frame_count = reader.read_short();
            std::vector<math::rect> sprite_frames {};
            for (auto i = 0; i < frame_count; ++i) {
                auto x = reader.read_short();
                auto y = reader.read_short();
                auto width = reader.read_short();
                auto height = reader.read_short();
                sprite_frames.emplace_back(math::rect(x, y, width, height));
            }

            // Read the TGA data
            auto tga_size = reader.read_long();
            auto tga_raw_data = std::make_shared<std::vector<char>>(reader.read_bytes(tga_size));
            asset::tga tga(tga_raw_data);
            if (auto surface = tga.surface().lock()) {
                configure(res->id(), res->name(), math::size(surface->size().width(), surface->size().height()), surface->raw());
                m_sheet->layout_sprites(sprite_frames, true);
                return;
            }
        }
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::spritesheet::load(const asset::resource_descriptor::lua_reference& ref) -> spritesheet::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(spritesheet::type, ref);
        if (asset.has_value()) {
            return std::any_cast<asset::spritesheet::lua_reference>(asset.value());
        }
    }

    auto image = asset::spritesheet::lua_reference(new asset::spritesheet(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(spritesheet::type, ref, image);
    }
    return image;
}

auto asset::spritesheet::size() const -> math::size
{
    return basic_image::size();
}

auto asset::spritesheet::sprite_count() const -> int
{
    return basic_image::sprite_count();
}

auto asset::spritesheet::spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>
{
    return asset::basic_image::spawn_entity(position);
}

