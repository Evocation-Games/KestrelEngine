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

#if !defined(KESTREL_SIZE_HPP)
#define KESTREL_SIZE_HPP


#include <cmath>

namespace math
{

    /**
     * Represents a 2D size.
     */
    struct size
    {
    public:
        double width;
        double height;

        size() : width(0), height(0) {};
        size(const double& v) : width(v), height(v) {};
        size(const double& w, const double& h) : width(w), height(h) {};

        auto operator+(const math::size& s) const -> math::size
        {
            return math::size(width + s.width, height + s.height);
        }

        auto operator-(const math::size& s) const -> math::size
        {
            return math::size(width - s.width, height - s.height);
        }

        auto operator*(const double& f) const -> math::size
        {
            return math::size(width * f, height * f);
        }

        auto operator/(const double& f) const -> math::size
        {
            return math::size(width / f, height / f);
        }

        auto operator==(const math::size& s) const -> bool
        {
            return (width == s.width) && (height == s.height);
        }

        auto operator!=(const math::size& s) const -> bool
        {
            return !(*this == s);
        }

        auto area() const -> double
        {
            return width * height;
        }

    };

};


#endif //KESTREL_SIZE_HPP
