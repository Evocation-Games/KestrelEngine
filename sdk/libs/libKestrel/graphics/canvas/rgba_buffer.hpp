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
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libGraphite/data/data.hpp>
#include <libKestrel/util/availability.hpp>

namespace kestrel::graphics
{
    class rgba_buffer
    {
    private:
        std::uint8_t *m_buffer { nullptr };
        std::uint64_t m_count { 0 };
        math::size m_size { 0 };
        math::rect m_clipping_rect;

        struct components
        {
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;
            std::uint8_t a;
        };

        union alignas(16) simd_value
        {
        #if TARGET_SUPPORTS_128BIT
            unsigned __int128 wide;
            std::uint32_t f[4];
            struct components c[4];
        #elif TARGET_64BIT
            std::uint64_t wide;
            std::uint32_t f[2];
            struct components c[2];
        #else
            std::uint32_t wide;
            std::uint32_t f[1];
            struct components c[1];
        #endif
        };

        auto common_construction() -> void;
        [[nodiscard]] static auto corrected(std::uint64_t i) -> std::uint64_t;
        [[nodiscard]] auto index(const math::point& p) const -> std::uint64_t;

        static inline auto blend_func_i(color::value bottom, color::value top) -> color::value;
        static inline auto blend_func(const union simd_value& bottom, const union simd_value& top) -> union simd_value;

    public:
        explicit rgba_buffer(const math::size& sz);
        rgba_buffer(const std::vector<color::value>& data, const math::size& sz);

        ~rgba_buffer();

        [[nodiscard]] auto count() const -> std::size_t;
        [[nodiscard]] auto size() const -> math::size;

        [[nodiscard]] auto at(std::uint64_t i) const -> color::value;
        [[nodiscard]] auto color(const math::point& p) const -> color;

        [[nodiscard]] auto data() const -> std::uint8_t *;
        [[nodiscard]] auto data_block() const -> graphite::data::block;

        auto clear(const graphics::color& c) -> void;
        auto clear_rect(const graphics::color& c, const math::rect& r) -> void;

        auto set_clipping_rect(const math::rect& r) -> void;
        auto clear_clipping_rect() -> void;

        auto draw_pixel(const graphics::color& c, const math::point& p) -> void;
        auto fill_rect(const graphics::color& c, const math::rect& r) -> void;

        auto apply_run(const graphics::color& c, std::uint64_t start, std::uint64_t end, std::uint64_t line) -> void;
        auto apply_run(const std::vector<graphics::color>& cv, std::uint64_t start, std::uint64_t line) -> void;
        auto apply_run(const graphite::data::block& cv, std::uint64_t start, std::uint64_t line) -> void;

        auto apply_mask(const rgba_buffer& buffer) -> void;
    };

}
