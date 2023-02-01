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

#pragma once

#include <libKestrel/graphics/image/basic_image.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libGraphite/quickdraw/support/surface.hpp>
#include <libKestrel/resource/macro.hpp>

namespace kestrel::image::legacy::spriteworld
{
    struct lua_api(Legacy.SpriteWorld.Sprite, Available_0_8) sprite: public basic_image
    {
    public:
        has_constructable_lua_api(sprite);

        is_resource_type("rlëD");
        struct alternate_type { static constexpr const char *code = "rlëX"; };

        lua_constructor(Available_0_8) explicit sprite(const resource::descriptor::lua_reference& ref);
        lua_function(load, Available_0_8) static auto load(const resource::descriptor::lua_reference& ref) -> lua_reference;

        lua_getter(type, Available_0_8) [[nodiscard]] auto type() const -> std::string;
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override;
        lua_getter(spriteCount, Available_0_8) [[nodiscard]] auto sprite_count() const -> std::size_t override;

        lua_function(layoutSprites, Available_0_8) auto layout_sprites(const math::size& sprite_size) -> void override;

        lua_function(spawnEntity, Available_0_8) [[nodiscard]] auto spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity> override;

    private:
        std::string m_source_type;
        graphite::quickdraw::surface m_surface;
    };
};
