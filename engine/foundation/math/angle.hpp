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

#if !defined(KESTREL_ANGLE_HPP)
#define KESTREL_ANGLE_HPP

namespace kestrel { namespace math {

    struct vector;
    struct angular_difference;

    struct angle
    {
    private:
        double m_theta { 0.0 };

    public:
        angle(const double& theta = 0.0);
        angle(const angle& a);

        auto degrees() const -> double;
        auto radians() const -> double;

        auto sin(const double magnitude = 1.0) const -> double;
        auto cos(const double magnitude = 1.0) const -> double;

        auto operator+ (const math::angle& a) const -> math::angle;
        auto operator- (const math::angle& a) const -> math::angle;
        auto operator+ (const math::angular_difference& a) const -> math::angle;
        auto operator- (const math::angular_difference& a) const -> math::angle;

        auto vector(const double magnitude = 1.0) const -> math::vector;

        auto opposite() const -> math::angle;
        auto rotated(const math::angular_difference& phi) const -> math::angle;

        auto normalise() -> void;

        auto is_opposing(const math::angle& a, const math::angular_difference& tolerance) const -> bool;
        auto is_equal(const math::angle& a, const math::angular_difference& tolerance) const -> bool;
    };

}};

#endif //KESTREL_ANGLE_HPP
