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

#include "foundation/math/point.hpp"

// MARK: - Lua Integration

auto kestrel::math::point::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<math::point>("Point")
            .addConstructor<auto(*)(double, double)->void, math::point::lua_reference>()
            .addProperty("x", &point::x, &point::set_x)
            .addProperty("y", &point::y, &point::set_y)
        .endClass();
}

// MARK: - Constructors

kestrel::math::point::point()
{

}

kestrel::math::point::point(double x, double y)
        : m_x(x), m_y(y)
{

}

kestrel::math::point::point(const math::point& p)
        : m_x(p.m_x), m_y(p.m_y)
{

}

kestrel::math::point::point(const math::vector& v)
        : m_x(v.x()), m_y(v.y())
{

}

kestrel::math::point::point(const diamane::point& p)
        : m_x(static_cast<double>(p.x())), m_y(static_cast<double>(p.y()))
{

}

kestrel::math::point::point(const graphite::qd::point& p)
        : m_x(static_cast<double>(p.x())), m_y(static_cast<double>(p.y()))
{

}

kestrel::math::point::point(const graphite::qd::fixed_point& p)
        : m_x(p.x()), m_y(p.y())
{

}

auto kestrel::math::point::zero() -> math::point
{
    return math::point(0, 0);
}

// MARK: - Conversions

auto kestrel::math::point::vector() const -> math::vector
{
    return math::vector(m_x, m_y);
}

auto kestrel::math::point::diamane() const -> diamane::point
{
    return diamane::point(m_x, m_y);
}

auto kestrel::math::point::quickdraw() const -> graphite::qd::point
{
    return graphite::qd::point(static_cast<int16_t>(m_x), static_cast<int16_t>(m_y));
}

auto kestrel::math::point::fixed() const -> graphite::qd::fixed_point
{
    return graphite::qd::fixed_point(m_x, m_y);
}

// MARK: - Accessors

auto kestrel::math::point::x() const -> double
{
    return m_x;
}

auto kestrel::math::point::y() const -> double
{
    return m_y;
}

auto kestrel::math::point::set_x(const double x) -> void
{
    m_x = x;
}

auto kestrel::math::point::set_y(const double y) -> void
{
    m_y = y;
}
