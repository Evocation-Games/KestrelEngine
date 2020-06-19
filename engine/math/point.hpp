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

#if !defined(KESTREL_POINT_HPP)
#define KESTREL_POINT_HPP

namespace math
{

    /**
     * Represents a 2D point.
     */
    struct point
    {
    public:
        double x;
        double y;

        point();
        point(const double& v);
        point(const double& x, const double& y);
        point(const math::point& p);

        auto operator+(const math::point& p) const -> math::point;
        auto operator-(const math::point& p) const -> math::point;
        auto operator*(const double& f) const -> math::point;
        auto operator/(const double& f) const -> math::point;
        auto operator==(const math::point& p) const -> bool;
        auto operator!=(const math::point& p) const -> bool;

        auto distance_to(const math::point& p) const -> double;
    };

};

#endif //KESTREL_POINT_HPP
