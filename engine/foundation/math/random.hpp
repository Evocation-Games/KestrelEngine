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

#if !defined(KESTREL_RANDOM_HPP)
#define KESTREL_RANDOM_HPP

#include <random>
#include <type_traits>

namespace kestrel { namespace math {

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    T random(const T& mininum, const T& maximum)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<T> r(mininum, maximum);
        return r(gen);
    }

    template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    T random(const T& mininum, const T& maximum)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> r(mininum, maximum);
        return r(gen);
    }

}};

#endif //KESTREL_RANDOM_HPP
