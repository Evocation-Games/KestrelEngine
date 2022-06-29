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

#pragma once

#include <vector>
#include <optional>
#include "math/size.hpp"
#include "math/point.hpp"
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"
#include <libGraphite/data/data.hpp>

namespace graphics
{

    class rgba_buffer
    {
    private:
        uint8_t *m_buffer { nullptr };
        uint64_t m_count { 0 };
        math::size m_size { 0 };
        math::rect m_clipping_rect;

        struct components
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        union alignas(16) simd_value
        {
        #if __x86_64__
            __int128 wide;
            uint32_t f[4];
            struct components c[4];
        #elif __arm64__
            uint64_t wide;
            uint32_t f[2];
            struct components c[2];
        #else
            uint32_t wide;
            uint32_t f[1];
            struct components c[1];
        #endif
        };

        auto common_construction() -> void;
        [[nodiscard]] static auto corrected(const uint64_t& i) -> uint64_t;
        [[nodiscard]] auto index(const math::point& p) const -> uint64_t;

        static inline auto blend_func_i(const uint32_t& bottom, const uint32_t& top) -> uint32_t;
        static inline auto blend_func(const union simd_value& bottom, const union simd_value& top) -> union simd_value;

    public:
        explicit rgba_buffer(const math::size& sz);
        rgba_buffer(const std::vector<uint32_t>& data, const math::size& sz);

        ~rgba_buffer();

        [[nodiscard]] auto count() const -> uint64_t;
        [[nodiscard]] auto size() const -> math::size;

        [[nodiscard]] auto at(const uint64_t& i) const -> uint32_t;
        [[nodiscard]] auto color(const math::point& p) const -> graphics::color;

        [[nodiscard]] auto data() const -> uint8_t *;

        auto clear(const graphics::color& c) -> void;
        auto clear_rect(const graphics::color& c, const math::rect& r) -> void;

        auto set_clipping_rect(const math::rect& r) -> void;
        auto clear_clipping_rect() -> void;

        auto draw_pixel(const graphics::color& c, const math::point& p) -> void;
        auto fill_rect(const graphics::color& c, const math::rect& r) -> void;

        auto apply_run(const graphics::color& c, const uint64_t& start, const uint64_t& end, const uint64_t& line) -> void;
        auto apply_run(const std::vector<graphics::color>& cv, const uint64_t& start, const uint64_t& line) -> void;
        auto apply_run(const graphite::data::block& cv, uint64_t start, uint64_t line) -> void;

        auto apply_mask(const rgba_buffer& buffer) -> void;
    };

}
