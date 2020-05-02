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

#include "libDiamane/util/geometry.hpp"
#include "libGraphite/quickdraw/geometry.hpp"
#include "scripting/lua/lua.hpp"

namespace kestrel { namespace math {

    struct size : public lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<math::size> lua_reference;

    private:
        double m_width { 0.0 };
        double m_height { 0.0 };

    public:
        static auto register_object() -> void;

        size();
        size(double width, double height);
        size(const math::size& size);
        size(const diamane::size& size);
        size(const graphite::qd::size& size);

        static auto zero() -> size;

        auto diamane() const -> diamane::size;
        auto quickdraw() const -> graphite::qd::size;

        auto width() const -> double;
        auto height() const -> double;

        auto set_width(double width) -> void;
        auto set_height(double height) -> void;

        auto area() const -> double;

        auto operator* (const math::size& size) -> math::size;

    };

}};

#endif //KESTREL_SIZE_HPP
