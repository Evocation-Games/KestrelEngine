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

#if !defined(KESTREL_VECTOR_HPP)
#define KESTREL_VECTOR_HPP

namespace kestrel { namespace math {

    struct angle;

    struct vector
    {
    private:
        double m_x { 0.0 };
        double m_y { 0.0 };
        double m_z { 0.0 };

    public:
        vector();
        vector(double x, double y, double z = 1.0);
        vector(const math::vector& v);

        static auto zero() -> math::vector;

        auto x() const -> double;
        auto y() const -> double;
        auto z() const -> double;

        auto set_x(double x) -> void;
        auto set_y(double y) -> void;
        auto set_z(double z) -> void;

        auto operator+ (const math::vector& v) const -> math::vector;
        auto operator- (const math::vector& v) const -> math::vector;
        auto operator* (double s) const -> math::vector;
        auto operator/ (double s) const -> math::vector;

        auto angle() const -> math::angle;
        auto angle(const math::vector& v) const -> math::angle;

        auto magnitude() const -> double;
    };

}};

#endif //KESTREL_VECTOR_HPP
