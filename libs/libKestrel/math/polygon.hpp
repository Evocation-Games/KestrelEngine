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

#include <vector>
#include <libKestrel/math/vec2.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>

namespace kestrel::math
{
    struct polygon
    {
        polygon() = default;
        explicit polygon(const std::vector<point>& vertices);
        explicit polygon(std::vector<vec2>& vertices);
        polygon(polygon&& poly) noexcept = default;
        polygon(const polygon& poly) = default;

        auto operator=(const polygon& poly) noexcept -> polygon& = default;
        auto operator=(polygon&& poly) -> polygon& = default;

        [[nodiscard]] auto is_valid() const -> bool;
        [[nodiscard]] auto is_triangle() const -> bool;
        [[nodiscard]] auto is_convex() const -> bool;

        [[nodiscard]] auto vertex_count() const -> std::size_t;
        [[nodiscard]] auto edge_count() const -> std::size_t;

        [[nodiscard]] auto vertex_at(std::int32_t idx) const -> vec2;
        [[nodiscard]] auto edge_at(std::int32_t idx) const -> vec2;

        [[nodiscard]] auto center() const -> vec2;

        [[nodiscard]] auto approximate_convex_polygon() const -> polygon;

    private:
        auto calculate_edges() -> void;
        auto calculate_center() -> void;

        [[nodiscard]] auto corrected_index(std::int32_t idx) const -> std::int32_t;

    private:
        vec2 m_center_point;
        std::vector<vec2> m_vertices;
        std::vector<vec2> m_edges;
    };
}