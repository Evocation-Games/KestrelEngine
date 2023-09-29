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

#pragma once

#include <memory>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/graphics/image/basic_image.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>

namespace kestrel::image
{

    class lua_api(StaticImage, Available_0_8) static_image: public basic_image
    {
    public:
        is_resource_type("sïmg");
        has_constructable_lua_api(static_image);

        static_image(resource_core::identifier id, const std::string& name, const std::shared_ptr<graphics::sprite_sheet>& sheet);
        static_image(const math::size& size, const graphics::color::lua_reference& color);

        static auto from(const luabridge::LuaRef& ref) -> lua_reference;

        luatool_type_fix(const resource::descriptor::lua_reference&, ref)
        lua_constructor(Available_0_8) explicit static_image(const resource::descriptor::lua_reference& ref);
        lua_function(load, Available_0_8) static auto load_best(const std::vector<resource::descriptor::lua_reference>& refs) -> lua_reference;
        lua_function(usingLegacyMacintoshPicture, Available_0_8) static auto using_pict(const legacy::macintosh::quickdraw::picture::lua_reference& ref) -> lua_reference;
        lua_function(usingLegacyMacintoshColorIcon, Available_0_8) static auto using_cicn(const legacy::macintosh::quickdraw::color_icon::lua_reference& ref) -> lua_reference;

        lua_function(preferred, Available_0_8) static auto preferred(const resource::descriptor::lua_reference& ref) -> lua_reference;

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override;
        lua_getter(spriteCount, Available_0_8) [[nodiscard]] auto sprite_count() const -> std::size_t override;

        lua_function(spawnEntity, Available_0_8) [[nodiscard]] auto spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity> override;
    };

}
