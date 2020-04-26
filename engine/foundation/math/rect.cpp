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

#include "foundation/math/rect.hpp"

// MARK: - Constructors

kestrel::math::rect::rect()
{

}

kestrel::math::rect::rect(double x, double y, double w, double h)
        : m_origin(x, y), m_size(w, h)
{

}

kestrel::math::rect::rect(const math::point& origin, const math::size& sz)
        : m_origin(origin), m_size(sz)
{

}

kestrel::math::rect::rect(const math::rect& r)
        : m_origin(r.m_origin), m_size(r.m_size)
{

}

kestrel::math::rect::rect(const math::vector& v, const math::size& sz)
        : m_origin(v), m_size(sz)
{

}

kestrel::math::rect::rect(const diamane::rect& r)
        : m_origin(r.origin()), m_size(r.size())
{

}

kestrel::math::rect::rect(const graphite::qd::rect& r)
        : m_origin(r.origin()), m_size(r.size())
{

}

kestrel::math::rect::rect(const graphite::qd::fixed_rect& r)
        : m_origin(r.origin()), m_size(r.size().width(), r.size().height())
{

}

auto kestrel::math::rect::zero() -> math::rect
{
    return math::rect(point::zero(), size::zero());
}

// MARK: - Conversions

auto kestrel::math::rect::diamane() const -> diamane::rect
{
    return diamane::rect(m_origin.diamane(), m_size.diamane());
}

auto kestrel::math::rect::quickdraw() const -> graphite::qd::rect
{
    return graphite::qd::rect(m_origin.quickdraw(), m_size.quickdraw());
}

auto kestrel::math::rect::fixed() const -> graphite::qd::fixed_rect
{
    return graphite::qd::fixed_rect(m_origin.fixed(), graphite::qd::fixed_size(width(), height()));
}

// MARK: - Accessors

auto kestrel::math::rect::x() const -> double
{
    return m_origin.x();
}

auto kestrel::math::rect::y() const -> double
{
    return m_origin.y();
}

auto kestrel::math::rect::width() const -> double
{
    return m_size.width();
}

auto kestrel::math::rect::height() const -> double
{
    return m_size.height();
}

auto kestrel::math::rect::max_x() const -> double
{
    return m_origin.x() + width();
}

auto kestrel::math::rect::max_y() const -> double
{
    return m_origin.y() + height();
}

auto kestrel::math::rect::origin() const -> kestrel::math::point
{
    return m_origin;
}

auto kestrel::math::rect::size() const -> math::size
{
    return m_size;
}

auto kestrel::math::rect::set_x(double x) -> void
{
    m_origin.set_x(x);
}

auto kestrel::math::rect::set_y(double y) -> void
{
    m_origin.set_y(y);
}

auto kestrel::math::rect::set_width(double w) -> void
{
    m_size.set_width(w);
}

auto kestrel::math::rect::set_height(double h) -> void
{
    m_size.set_height(h);
}

auto kestrel::math::rect::set_max_x(double x) -> void
{
    m_size.set_width(x - m_origin.x());
}

auto kestrel::math::rect::set_max_y(double y) -> void
{
    m_size.set_height(y - m_origin.y());
}

auto kestrel::math::rect::set_origin(const math::point& origin) -> void
{
    m_origin = origin;
}

auto kestrel::math::rect::set_size(const math::size& sz) -> void
{
    m_size = sz;
}

auto kestrel::math::rect::center() const -> point
{
    return math::point(m_origin.x() + (m_size.width() / 2.0), m_origin.y() + (m_size.height() / 2.0));
}

// MARK: - Operators

auto kestrel::math::rect::operator+ (const math::vector& v) const -> rect
{
    return math::rect(m_origin.vector() + v, m_size);
}

auto kestrel::math::rect::operator- (const math::vector& v) const -> rect
{
    return math::rect(m_origin.vector() - v, m_size);
}
