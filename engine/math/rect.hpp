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

#if !defined(KESTREL_RECT_HPP)
#define KESTREL_RECT_HPP

#include "math/point.hpp"
#include "math/size.hpp"

namespace math
{

    /**
     * Represents a frame/area in 2D space.
     */
    struct rect
    {
        math::point origin;
        math::size size;

        rect();
        rect(const math::point& o, const math::size& s);
        rect(const double& x, const double& y, const double& w, const double& h);
        rect(const math::rect& r);

        auto operator+(const math::point& p) const -> math::rect;
        auto operator+(const math::size& s) const -> math::rect;
        auto operator-(const math::point& p) const -> math::rect;
        auto operator-(const math::size& s) const -> math::rect;
        auto operator*(const double& f) const -> math::rect;
        auto operator/(const double& f) const -> math::rect;
        auto operator==(const math::rect& r) const -> bool;
        auto operator!=(const math::rect& r) const -> bool;

        auto area() const -> double;
        auto contains(const math::point& p) const -> bool;
        auto contains(const math::rect& r) const -> bool;
        auto intersects(const math::rect& r) const -> bool;
    };

}

#endif //KESTREL_RECT_HPP
