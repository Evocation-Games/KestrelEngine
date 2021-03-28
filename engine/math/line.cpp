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

#include "math/line.hpp"

// MARK: - Lua

auto math::line::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<math::line>("Line")
            .addConstructor<auto(*)(const math::point&, const math::point&)->void, luabridge::RefCountedPtr<math::line>>()
            .addProperty("p", &math::line::get_p, &math::line::set_p)
            .addProperty("q", &math::line::get_q, &math::line::set_q)
            .addFunction("intersects", &math::line::intersects)
        .endClass();
}

// MARK: - Construction

math::line::line(const math::point &p, const math::point &q)
    : p(p), q(q)
{

}

// MARK: - Accessors

auto math::line::get_p() const -> math::point
{
    return p;
}

auto math::line::set_p(const math::point& p) -> void
{
    this->p = p;
}

auto math::line::get_q() const -> math::point
{
    return q;
}

auto math::line::set_q(const math::point& q) -> void
{
    this->q = q;
}

// MARK: - Functions

auto math::line::intersects(const math::line& l) const -> bool
{
    const auto determinant = [](const double& a, const double& b, const double& c, const double& d) -> double {
        return (a * d) - (b * c);
    };

    auto det_l1 = determinant(p.x, p.y, q.x, q.y);
    auto det_l2 = determinant(l.p.x, l.p.y, l.q.x, l.q.y);
    auto x1mx2 = p.x - q.x;
    auto x3mx4 = l.p.x - l.q.x;
    auto y1my2 = p.y - q.y;
    auto y3my4 = l.p.y - l.q.y;

    auto xnom = determinant(det_l1, x1mx2, det_l2, x3mx4);
    auto ynom = determinant(det_l1, y1my2, det_l2, y3my4);
    auto denom = determinant(x1mx2, y1my2, x3mx4, y3my4);

    if (denom == 0.0) {
        return false;
    }

    auto x = xnom / denom;
    auto y = ynom / denom;
    if (!std::isfinite(x) || !std::isfinite(y)) {
        return false;
    }

    // TODO: Pass out the coordinate to the caller.
    return true;
}
