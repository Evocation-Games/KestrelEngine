// Copyright (c) 2023 Tom Hancocks
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

#include <libKestrel/math/triangle.hpp>
#include <libSIMD/float32.hpp>

// MARK: - Construction

kestrel::math::triangle::triangle(const vec2 &a, const vec2 &b, const vec2 &c)
    : a(a), b(b), c(c)
{}

// MARK: - Operators

auto kestrel::math::triangle::operator+(const math::point &p) const -> triangle
{
    auto v = (a.m_value.lower() + b.m_value.lower().swapped()) + p.m_value;
    auto u = (c.m_value.lower()) + p.m_value;
    return {
        vec2(v.lower() + v.lower().swapped()),
        vec2(v.upper() + v.upper().swapped()),
        vec2(u.lower() + u.lower().swapped())
    };
}

auto kestrel::math::triangle::operator+(const math::vec2 &v) const -> triangle
{
    return *this + v.to_point();
}

auto kestrel::math::triangle::operator-(const math::point &p) const -> triangle
{
    auto v = (a.m_value.lower() + b.m_value.lower().swapped()) - p.m_value;
    auto u = (c.m_value.lower()) - p.m_value;
    return {
        vec2(v.lower() + v.lower().swapped()),
        vec2(v.upper() + v.upper().swapped()),
        vec2(u.lower() + u.lower().swapped())
    };
}

auto kestrel::math::triangle::operator-(const math::vec2 &v) const -> triangle
{
    return *this - v.to_point();
}

// MARK: - Operations

//auto kestrel::math::triangle::intersects(const triangle &other) const -> bool
//{
//    auto cross = [] (const triangle& t,  const vec2& b, const vec2& c, float n) -> bool {
//        auto cb = c - b;
//        auto pba = t.a - b;
//        auto pbb = t.b - b;
//        auto pbc = t.c - b;
//        auto r1 = simd::float32(pba.x(), pbb.x(), pbc.x()) * simd::float32::constant(cb.y());
//        auto r2 = simd::float32(pba.y(), pbb.y(), pbc.y()) * simd::float32::constant(cb.x());
//        r1 -= r2;
//        r1 *= simd::float32::constant(n);
//        return !((r1[0] < 0) || (r1[1] < 0) || (r1[2] < 0));
//    };
//
//    // SIMD Layout
//    //  (Abx - Aax) * (Acy - Aay) - (Aby - Aay) * (Acx - Aax)
//    //   A             B             C             D
//    //  (Bbx - Bax) * (Bcy - Bay) - (Bby - Bay) * (Bcx - Bax)
//    //   E             F             G             H
//    //
//    //  AA = [ Abx, Bbx, Acy, Bcy ] | [ A, E, B, F ]
//    //  BB = [ Aby, Bby, Acx, Bcx ] | [ C, G, D, H ]
//    //  CC = [ Aax, Bax, Aay, Bay ]
//    //  DD = [ Aay, Bay, Aax, Bax ]
//    //
//    //  AA = AA - CC
//    //  BB = BB - DD
//    //
//    //   (A * B) - (C * D)
//    //    I         J
//    //   (E * F) - (G * H)
//    //    K         L
//    //
//    // Normals
//    auto AA = simd::float32(b.x(), other.b.x(), c.y(), other.c.y());
//    auto BB = simd::float32(b.y(), other.b.y(), c.x(), other.c.x());
//    auto CC = simd::float32(a.x(), other.a.x(), a.y(), other.a.y());
//    auto DD = simd::float32(a.y(), other.a.y(), a.x(), other.a.x());
//    AA -= CC;
//    BB -= DD;
//    AA = simd::float32(AA[0], BB[0], AA[1], BB[1]) * simd::float32(AA[2], AA[3], BB[2], BB[3]);
//    AA = simd::float32(AA[0], AA[1]) -simd::float32(AA[2], AA[3]);
//    auto n1 = AA[0];
//    auto n2 = AA[1];
//
//    return !(
//        cross(other, a, b, n1) || cross(other, b, c, n1) || cross(other, c, a, n1) ||
//        cross(*this, other.a, other.b, n2) || cross(*this, other.b, other.c, n2) || cross(*this, other.c, other.a, n2)
//    );
//}

auto kestrel::math::triangle::intersects(const triangle &other) const -> bool
{
    constexpr float eps = 0.f;
    if (check_winding(a, b, c, false) || check_winding(other.a, other.b, other.c, false)) {
        return false;
    }

    if (boundary_collision(a, b, other.a, eps) && boundary_collision(a, b, other.b, eps) && boundary_collision(a, b, other.c, eps)) {
        return false;
    }
    if (boundary_collision(b, c, other.a, eps) && boundary_collision(b, c, other.b, eps) && boundary_collision(b, c, other.c, eps)) {
        return false;
    }
    if (boundary_collision(c, a, other.a, eps) && boundary_collision(c, a, other.b, eps) && boundary_collision(c, a, other.c, eps)) {
        return false;
    }

    if (boundary_collision(other.a, other.b, a, eps) && boundary_collision(other.a, other.b, b, eps) && boundary_collision(other.a, other.b, c, eps)) {
        return false;
    }
    if (boundary_collision(other.b, other.c, a, eps) && boundary_collision(other.b, other.c, b, eps) && boundary_collision(other.b, other.c, c, eps)) {
        return false;
    }
    if (boundary_collision(other.c, other.a, a, eps) && boundary_collision(other.c, other.a, b, eps) && boundary_collision(other.c, other.a, c, eps)) {
        return false;
    }

    return true;
}


// MARK: - Private

auto kestrel::math::triangle::det(const vec2 &a, const vec2 &b, const vec2 &c) -> float
{
    auto r = (simd::float32(b.y(), c.y(), a.y()) - simd::float32(c.y(), a.y(), b.y()))
           * simd::float32(a.x(), b.x(), c.x());
    return r[0] + r[1] + r[2];
}

auto kestrel::math::triangle::check_winding(const vec2 &a, const vec2 &b, const vec2 &c, bool allow_reversed) -> bool
{
    auto det = triangle::det(a, b, c);
    if (det >= 0.0 || allow_reversed) {
        return true;
    }
    return false;
}

auto kestrel::math::triangle::boundary_collision(const vec2 &a, const vec2 &b, const vec2 &c, float eps) -> bool
{
    return triangle::det(a, b, c) < eps;
}