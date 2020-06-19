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

#if !defined(KESTREL_ANGULAR_DIFFERENCE_HPP)
#define KESTREL_ANGULAR_DIFFERENCE_HPP

namespace math
{

    struct angle;

    struct angular_difference
    {
    private:
        double m_phi { 0.0 };

    public:
        angular_difference(const double& phi = 0.0);
        angular_difference(const math::angular_difference& d);

        static auto between(const math::angle& a, const math::angle& b) -> math::angular_difference;

        auto operator== (math::angular_difference& a) const -> bool;
        auto operator!= (math::angular_difference& a) const -> bool;
        auto operator<= (math::angular_difference& a) const -> bool;
        auto operator< (math::angular_difference& a) const -> bool;
        auto operator> (math::angular_difference& a) const -> bool;
        auto operator>= (math::angular_difference& a) const -> bool;

        auto calculate_for(const math::angle& a) const -> math::angle;

        auto is_clockwise() const -> bool;
        auto is_anti_clockwise() const -> bool;

        auto is_opposing(const math::angular_difference& tolerance = 0.0) const -> bool;
        auto is_equal(const math::angular_difference& tolerance = 0.0) const -> bool;

        auto phi() const -> double;
    };

}


#endif //KESTREL_ANGULAR_DIFFERENCE_HPP
