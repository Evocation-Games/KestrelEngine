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

#if !defined(KESTREL_IMAGE_HPP)
#define KESTREL_IMAGE_HPP

#include <string>
#include "core/graphics/common/color.hpp"
#include "core/asset/resource_reference.hpp"
#include "core/graphics/common/spritesheet.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/graphics/common/entity.hpp"

namespace asset
{

    class image: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<asset::image> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    protected:
        int64_t m_id { INT64_MIN };
        std::string m_name { "" };
        std::shared_ptr<graphics::spritesheet> m_sheet;

        auto configure(const int64_t& id, const std::string& name, const math::size& size, std::vector<uint32_t> data) -> void;

    public:
        image() = default;
        image(const math::size& size, const graphics::color& color = graphics::color::white_color());
        image(const int64_t& id, const std::string& name, const math::size& size, std::vector<uint32_t> data);
        lua_api image(const math::size::lua_reference& size, const graphics::color::lua_reference& color);

        lua_api auto size() const -> math::size;
        lua_api virtual auto sprite_count() const -> int;

        lua_api virtual auto layout_sprites(const math::size& sprite_size) -> void;

        auto spritesheet() const -> std::shared_ptr<graphics::spritesheet>;

        lua_api virtual auto spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference;
    };

}

#endif //KESTREL_IMAGE_HPP
