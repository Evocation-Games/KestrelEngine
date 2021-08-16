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

#if !defined(KESTREL_STATIC_IMAGE_HPP)
#define KESTREL_STATIC_IMAGE_HPP

#include <memory>
#include "scripting/state.hpp"
#include "core/asset/basic_image.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"

namespace asset
{

    class static_image: public asset::basic_image, public scripting::lua::object
    {
    public:
        constexpr static const char *type { "sïmg" };
        typedef luabridge::RefCountedPtr<asset::static_image> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        static_image(const int64_t& id, const std::string& name, std::shared_ptr<graphics::spritesheet> sheet);

        lua_api explicit static_image(const asset::resource_descriptor::lua_reference& ref);
        lua_api static auto load_best(std::vector<asset::resource_descriptor::lua_reference> refs) -> static_image::lua_reference;
        lua_api static auto using_pict(const asset::macintosh_picture::lua_reference& ref) -> static_image::lua_reference;
        lua_api static auto using_cicn(const asset::color_icon::lua_reference& ref) -> static_image::lua_reference;

        lua_api static auto preferred(const asset::resource_descriptor::lua_reference& ref) -> static_image::lua_reference;

        lua_api [[nodiscard]] auto size() const -> math::size override;
        lua_api [[nodiscard]] auto sprite_count() const -> int override;

        lua_api [[nodiscard]] auto spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference override;
    };

}

#endif //KESTREL_STATIC_IMAGE_HPP
