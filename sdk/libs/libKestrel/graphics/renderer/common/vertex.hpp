// Copyright (c) 2022 Tom Hancocks
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

#pragma once

#include <libKestrel/math/vec2.hpp>
#include <libKestrel/math/vec4.hpp>

namespace kestrel::renderer
{
    struct vec4
    {
        float x, y, z, w;
        vec4() = default;
        explicit vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
        explicit vec4(const math::vec4& v) : x(v.x()), y(v.y()), z(v.z()), w(v.w()) {};
        explicit vec4(const math::vec3& v) : x(v.x()), y(v.y()), z(v.z()), w(0) {};
        explicit vec4(const math::vec2& v) : x(v.x()), y(v.y()), z(0), w(0) {};
    };

    struct vec3
    {
        float x, y, z;
        vec3() = default;
        explicit vec3(const math::vec3& v) : x(v.x()), y(v.y()), z(v.z()) {};
        explicit vec3(const math::vec2& v) : x(v.x()), y(v.y()), z(0) {};
    };

    struct vec2
    {
        float x, y;
        vec2() = default;
        explicit vec2(const math::vec2& v) : x(v.x()), y(v.y()) {};
        explicit vec2(const math::size& s) : x(s.width()), y(s.height()) {};
        explicit vec2(const math::point& p) : x(p.x()), y(p.y()) {};
    };

    struct vertex
    {
        struct vec4 position;
        struct vec4 color;
        struct vec4 attachments[8];
        struct vec2 tex_coord;
        float texture { 0 };
        float unused { 0 };
    };
}