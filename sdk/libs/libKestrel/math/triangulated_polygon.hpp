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

#include <libKestrel/math/triangle.hpp>
#include <libKestrel/math/polygon.hpp>
#include <libKestrel/math/vec2.hpp>

namespace kestrel::math
{
    struct triangulated_polygon
    {
        triangulated_polygon() = default;
        explicit triangulated_polygon(const polygon& poly);
        triangulated_polygon(const triangulated_polygon&) noexcept = default;
        triangulated_polygon(triangulated_polygon&&) = default;

        auto operator=(const triangulated_polygon&) noexcept -> triangulated_polygon& = default;
        auto operator=(triangulated_polygon&&) -> triangulated_polygon& = default;

        [[nodiscard]] auto is_valid() const -> bool;
        [[nodiscard]] auto triangle_count() const -> std::size_t;
        [[nodiscard]] auto triangle_at(std::uint32_t idx) const -> triangle;

        [[nodiscard]] auto center() const -> vec2;

        [[nodiscard]] auto operator*(const math::size& s) const -> triangulated_polygon;

    private:
        explicit triangulated_polygon(const std::vector<triangle>& triangles, const math::vec2& center);

        auto triangulate_polygon(const polygon& poly) -> void;

    private:
        std::vector<triangle> m_triangles;
        vec2 m_center;
    };
}