// Copyright (c) 2022 Tom Hancocks
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

#pragma once

#include <libKestrel/math/vec2.hpp>

namespace kestrel::math
{
    struct triangle
    {
        vec2 a, b, c;

        triangle() = default;
        triangle(const vec2& a, const vec2& b, const vec2& c) : a(a), b(b), c(c) {}
        triangle(triangle&& tri) noexcept = default;
        triangle(const triangle& tri) = default;

        auto operator=(const triangle& tri) -> triangle& = default;

        auto operator+(const math::point& p) const -> triangle
        {
            return {
                vec2(a.x + p.x, a.y + p.y),
                vec2(b.x + p.x, b.y + p.y),
                vec2(c.x + p.x, c.y + p.y)
            };
        }

        [[nodiscard]] auto determinant() const -> double
        {
            return determinant(a, b, c);
        }

        [[nodiscard]] auto check_winding(bool allow_reversed = false) -> bool
        {
            auto det = determinant();
            if (det < 0) {
                if (allow_reversed) {
                    auto c = this->c;
                    this->c = this->b;
                    this->b = c;
                }
                else {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] auto check_collision(const triangle& tri, double eps = 0, bool allow_reversed = false, bool on_boundary = true) const -> bool
        {
            auto t1 = *this;
            auto t2 = tri;

            if (!t1.check_winding(allow_reversed) || !t2.check_winding(allow_reversed)) {
                return false;
            }

            auto check_function = on_boundary ? boundary_collision_check : boundary_no_collision_check;

            if (check_function(t1.a, t1.b, t2.a, eps) &&
                check_function(t1.a, t1.b, t2.b, eps) &&
                check_function(t1.a, t1.b, t2.c, eps)) {
                return false;
            }

            if (check_function(t1.b, t1.c, t2.a, eps) &&
                check_function(t1.b, t1.c, t2.b, eps) &&
                check_function(t1.b, t1.c, t2.c, eps)) {
                return false;
            }

            if (check_function(t1.c, t1.a, t2.a, eps) &&
                check_function(t1.c, t1.a, t2.b, eps) &&
                check_function(t1.c, t1.a, t2.c, eps)) {
                return false;
            }

            if (check_function(t2.a, t2.b, t1.a, eps) &&
                check_function(t2.a, t2.b, t1.b, eps) &&
                check_function(t2.a, t2.b, t1.c, eps)) {
                return false;
            }

            if (check_function(t2.b, t2.c, t1.a, eps) &&
                check_function(t2.b, t2.c, t1.b, eps) &&
                check_function(t2.b, t2.c, t1.c, eps)) {
                return false;
            }

            if (check_function(t2.c, t2.a, t1.a, eps) &&
                check_function(t2.c, t2.a, t1.b, eps) &&
                check_function(t2.c, t2.a, t1.c, eps)) {
                return false;
            }

            return true;
        }

    private:
        static auto determinant(const vec2& a, const vec2& b, const vec2& c) -> double
        {
            return +a.x * (b.y  - c.y)
                   +b.x * (c.y - a.y)
                   +c.x * (a.y - b.y);
        }

        static auto boundary_collision_check(const vec2& a, const vec2& b, const vec2& c, double eps) -> bool
        {
            return determinant(a, b, c) < eps;
        }

        static auto boundary_no_collision_check(const vec2& a, const vec2& b, const vec2& c, double eps) -> bool
        {
            return determinant(a, b, c) <= eps;
        }
    };
}