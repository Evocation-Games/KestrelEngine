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

#if !defined(KESTREL_SPRITE_HPP)
#define KESTREL_SPRITE_HPP

#include "core/asset/image.hpp"
#include "core/asset/resource_reference.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace asset
{

    struct sprite: public asset::image, public scripting::lua::object
    {
    public:
        constexpr static const char *type { "rlëD" };
        typedef luabridge::RefCountedPtr<asset::sprite> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit sprite(const asset::resource_reference::lua_reference& ref);
        lua_api static auto load(const asset::resource_reference::lua_reference& ref) -> sprite::lua_reference;

        lua_api auto size() const -> math::size;
        lua_api auto sprite_count() const -> int override;

        lua_api auto layout_sprites(const math::size& sprite_size) -> void override;

        lua_api auto spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference override;
    };

};

#endif //KESTREL_SPRITE_HPP
