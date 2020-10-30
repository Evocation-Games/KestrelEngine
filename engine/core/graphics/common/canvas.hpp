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

#if !defined(KESTREL_CANVAS_HPP)
#define KESTREL_CANVAS_HPP

#include <type_traits>
#include <memory>
#include <vector>
#include <optional>
#include "util/hint.hpp"
#include "scripting/state.hpp"
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"
#include "core/graphics/common/entity.hpp"

namespace graphics
{

    class canvas: public std::enable_shared_from_this<graphics::canvas>, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<graphics::canvas> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        math::size m_size;
        std::vector<graphics::color> m_buffer;
        graphics::color m_pen_color;
        graphics::entity::lua_reference m_entity { nullptr };

        auto draw_pixel(const double& x, const double& y) -> void;
        auto index_at(const double& x, const double& y) const -> int;
        auto raw() const -> std::vector<uint32_t>;

    public:
        lua_api explicit canvas(const math::size& size);

        lua_api auto get_pen_color() const -> graphics::color;
        lua_api auto set_pen_color(const graphics::color& color) -> void;

        lua_api auto clear() -> void;

        lua_api auto fill_rect(const math::rect& r) -> void;

        lua_api auto draw_circle(const math::point& p, const double& r) -> void;

        lua_api auto spawn_entity(const math::vector& position) -> graphics::entity::lua_reference;
        lua_api auto entity() -> graphics::entity::lua_reference;
        lua_api auto rebuild_texture() -> void;
    };

}

#endif //KESTREL_CANVAS_HPP
