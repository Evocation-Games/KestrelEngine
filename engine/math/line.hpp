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

#if !defined(KESTREL_LINE_HPP)
#define KESTREL_LINE_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "math/point.hpp"
#include <cmath>

namespace math
{

    struct line: public scripting::lua::object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        math::point p;
        math::point q;

        lua_api line(const math::point& p, const math::point& q);

        lua_api auto operator*(const double& f) const -> math::line;
        lua_api auto operator/(const double& f) const -> math::line;

        lua_api auto get_p() const -> math::point;
        lua_api auto set_p(const math::point& p) -> void;
        lua_api auto get_q() const -> math::point;
        lua_api auto set_q(const math::point& q) -> void;

        lua_api auto intersects(const math::line& l) const -> bool;
    };

}

#endif //KESTREL_LINE_HPP
