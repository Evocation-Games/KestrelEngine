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

#include "libDiamane/util/geometry.hpp"
#include "libGraphite/quickdraw/geometry.hpp"
#include "foundation/math/vector.hpp"

namespace kestrel { namespace math {

    struct point
    {
    private:
        double m_x { 0.0 };
        double m_y { 0.0 };

    public:
        point();
        point(double x, double y);
        point(const math::point& p);
        point(const math::vector& v);
        point(const diamane::point& p);
        point(const graphite::qd::point& p);
        point(const graphite::qd::fixed_point& p);

        static auto zero() -> math::point;

        auto diamane() const -> diamane::point;
        auto quickdraw() const -> graphite::qd::point;
        auto fixed() const -> graphite::qd::fixed_point;
        auto vector() const -> math::vector;

        auto x() const -> double;
        auto y() const -> double;

        auto set_x(double x) -> void;
        auto set_y(double y) -> void;
    };

}};

#endif //KESTREL_POINT_HPP
