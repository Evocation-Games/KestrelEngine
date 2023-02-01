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
#include <libKestrel/math/SIMD/float4.hpp>

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