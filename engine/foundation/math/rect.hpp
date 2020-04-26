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


#include "foundation/math/point.hpp"
#include "foundation/math/size.hpp"
#include "foundation/math/vector.hpp"
#include "libDiamane/util/geometry.hpp"
#include "libGraphite/quickdraw/geometry.hpp"

namespace kestrel { namespace math {

    struct rect
    {
    private:
        math::point m_origin { math::point::zero() };
        math::size m_size { math::size::zero() };

    public:
        rect();
        rect(double x, double y, double w, double h);
        rect(const math::point& origin, const math::size& sz);
        rect(const math::rect& r);
        rect(const math::vector& v, const math::size& sz);
        rect(const diamane::rect& r);
        rect(const graphite::qd::rect& r);
        rect(const graphite::qd::fixed_rect& r);

        static auto zero() -> math::rect;

        auto diamane() const -> diamane::rect;
        auto quickdraw() const -> graphite::qd::rect;
        auto fixed() const -> graphite::qd::fixed_rect;

        auto x() const -> double;
        auto y() const -> double;
        auto width() const -> double;
        auto height() const -> double;
        auto max_x() const -> double;
        auto max_y() const -> double;
        auto origin() const -> math::point;
        auto size() const -> math::size;

        auto set_x(double x) -> void;
        auto set_y(double y) -> void;
        auto set_width(double w) -> void;
        auto set_height(double h) -> void;
        auto set_max_x(double x) -> void;
        auto set_max_y(double y) -> void;
        auto set_origin(const math::point& origin) -> void;
        auto set_size(const math::size& sz) -> void;

        auto center() const -> math::point;

        auto operator+ (const math::vector& v) const -> math::rect;
        auto operator- (const math::vector& v) const -> math::rect;
    };

}};

#endif //KESTREL_RECT_HPP
