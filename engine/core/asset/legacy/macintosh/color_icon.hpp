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

#if !defined(KESTREL_COLOR_ICON_HPP)
#define KESTREL_COLOR_ICON_HPP

#include "core/asset/basic_image.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace asset::legacy::macintosh::quickdraw
{

    struct color_icon: public asset::basic_image, public scripting::lua::object
    {
    public:
        constexpr static const char *type { "cicn" };
        typedef luabridge::RefCountedPtr<color_icon> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        lua_api explicit color_icon(const asset::resource_descriptor::lua_reference& ref);
        lua_api static auto load(const asset::resource_descriptor::lua_reference& ref) -> color_icon::lua_reference;

        lua_api [[nodiscard]] auto size() const -> math::size override;

        lua_api [[nodiscard]] auto spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity> override;
    };

};

#endif //KESTREL_COLOR_ICON_HPP
