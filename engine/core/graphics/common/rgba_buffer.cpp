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

#if __x86_64__
#include <emmintrin.h>
#endif

#include "core/graphics/common/rgba_buffer.hpp"

// MARK: - Construction

graphics::rgba_buffer::rgba_buffer(const math::size &sz)
    : m_size(sz)
{
    common_construction();
}

graphics::rgba_buffer::rgba_buffer(const std::vector<uint32_t> &data, const math::size &sz)
    : m_size(sz)
{
    // We need
    common_construction();

    // Copy the data into the buffer. This code path is _not_ optimal and there should be some effort in the future
    // to optimise this. However this is a low frequency path and as such is not worth immediate optimisation.
    auto dst = reinterpret_cast<uint32_t *>(m_buffer);
    for (const auto& i : data) {
        *dst++ = i;
    }
}

auto graphics::rgba_buffer::common_construction() -> void
{
    m_count = (static_cast<uint64_t>(m_size.width) * static_cast<uint64_t>(m_size.height)) << 2;
    m_buffer = static_cast<uint8_t *>(calloc(m_count, 1)); // 4 Channel Color (RGBA)
}

// MARK: - Destruction

graphics::rgba_buffer::~rgba_buffer()
{
    free(m_buffer);
}

// MARK: - Accessors

auto graphics::rgba_buffer::count() const -> uint64_t
{
    return m_count;
}

auto graphics::rgba_buffer::size() const -> math::size
{
    return m_size;
}

auto graphics::rgba_buffer::data() const -> uint8_t *
{
    return m_buffer;
}

// MARK: - Look Up

auto graphics::rgba_buffer::corrected(const uint64_t &i) -> uint64_t
{
    return (i << 2);
}

auto graphics::rgba_buffer::index(const math::point &p) const -> uint64_t
{
    return (static_cast<uint64_t>(p.y) * static_cast<uint64_t>(m_size.width)) + static_cast<uint64_t>(p.x);
}

auto graphics::rgba_buffer::at(const uint64_t &i) const -> uint32_t
{
    auto ptr = reinterpret_cast<uint32_t *>(m_buffer) + i;
    return *ptr;
}

auto graphics::rgba_buffer::color(const math::point &p) const -> graphics::color
{
    auto value = at(index(p));
    return graphics::color(value);
}

// MARK: - Operations
// TODO: The following operations need to be optimised to use SIMD instructions where possible.
// Each of these operations will likely need differing versions for Intel/ARM machines.

auto graphics::rgba_buffer::blend_func_i(const uint32_t &bottom, const uint32_t &top) -> uint32_t
{
    uint8_t ta = (top >> 24);
    uint8_t ba = (bottom >> 24);

    uint32_t rb = bottom & 0xff00ff;
    uint32_t g  = bottom & 0x00ff00;
    uint8_t a = ta + ((ba * (0x100 - ta)) >> 8);
    rb += (((top & 0xff00ff) - rb) * ta) >> 8;
    g  += (((top & 0x00ff00) -  g) * ta) >> 8;

    return (rb & 0xff00ff) | (g & 0xff00) | (a << 24);
};

auto graphics::rgba_buffer::blend_func(const union simd_value& bottom, const union simd_value& top) -> union simd_value
{
    union simd_value r;

#if __x86_64__
    r.f[0] = blend_func_i(bottom.f[0], top.f[0]);
    r.f[1] = blend_func_i(bottom.f[1], top.f[1]);
    r.f[2] = blend_func_i(bottom.f[2], top.f[2]);
    r.f[3] = blend_func_i(bottom.f[3], top.f[3]);
#elif __arm64__
    r.f[0] = blend_func_i(bottom.f[0], top.f[0]);
    r.f[1] = blend_func_i(bottom.f[1], top.f[1]);
#else
    r.wide = top.wide;
#endif

    return r;
}

auto graphics::rgba_buffer::clear(const graphics::color &c) -> void
{
    clear_rect(c, {math::point(0), m_size });
}

auto graphics::rgba_buffer::clear_rect(const graphics::color &c, const math::rect &r) -> void
{
    auto lo_x = static_cast<uint32_t>(std::max(r.get_x(), 0.0));
    auto lo_y = static_cast<uint32_t>(std::max(r.get_y(), 0.0));
    auto hi_x = static_cast<uint32_t>(std::min(r.get_width() + r.get_x(), m_size.width));
    auto hi_y = static_cast<uint32_t>(std::min(r.get_height() + r.get_y(), m_size.height));
    auto width = hi_x - lo_x;
    auto height = hi_y - lo_y;

    auto stride = static_cast<uint64_t>(m_size.width);
    auto pitch = stride - width;
    auto ptr = reinterpret_cast<uint32_t *>(m_buffer) + (lo_y * static_cast<uint64_t>(m_size.width)) + lo_x;

    union simd_value v {};
    for (unsigned int & i : v.f) {
        i = c.rgba.value;
    }

    for (auto scanline = 0; scanline < height; ++scanline) {
#if __x86_64__
        uint32_t n = 0;
        uint64_t i = 0;
        while (n < stride) {

            if ((reinterpret_cast<uint64_t>(ptr) & 0xF) || (stride - n) < 4) {
                *ptr = v.f[n & 3];
                ++ptr;
                ++n;
            }
            else {
                *reinterpret_cast<__int128*>(ptr) = v.wide;
                ptr += 4;
                n += 4;
            }

        }
        ptr += pitch;

#elif __arm64__
        uint32_t n = 0;
        uint64_t i = 0;
        while (n < stride) {
            if ((reinterpret_cast<uint64_t>(ptr) & 0x7) || (stride - n) < 2) {
                *ptr = v.f[n & 1];
                ++ptr;
                ++n;
            }
            else {
                *reinterpret_cast<uint64_t*>(ptr) = v.wide;
                ptr += 2;
                n += 2;
            }
        }
#else
#warning Using a naive graphics::rgba_buffer::clear_rect implementation for architecture.
        // Fallback on a default naive implementation.
        for (auto x = lo_x; x < hi_x; ++x) {
            *ptr++ = v.wide;
        }
        ptr += pitch;
#endif
    }
}

auto graphics::rgba_buffer::draw_pixel(const graphics::color &c, const math::point &p) -> void
{
    if (p.x < 0 || p.x >= m_size.width || p.y < 0 || p.y >= m_size.height) {
        return;
    }
    apply_run(c, p.x, p.x + 1, p.y);
}

auto graphics::rgba_buffer::fill_rect(const graphics::color &c, const math::rect &r) -> void
{
    auto lo_x = std::max(static_cast<int>(r.get_x()), 0);
    auto lo_y = std::max(static_cast<int>(r.get_y()), 0);
    auto hi_x = std::min(static_cast<int>(r.get_width() + r.get_x()), static_cast<int>(m_size.width - 1));
    auto hi_y = std::min(static_cast<int>(r.get_height() + r.get_y()), static_cast<int>(m_size.height));

    if (lo_x >= hi_x || lo_y >= hi_y) {
        return;
    }

    for (auto y = lo_y; y < hi_y; ++y) {
        apply_run(c, lo_x, hi_x, y);
    }
}

auto graphics::rgba_buffer::apply_run(const graphics::color &c, const uint64_t &start, const uint64_t &end,
                                      const uint64_t &line) -> void
{
    if (line < 0 || line >= m_size.height || start >= end) {
        return;
    }

    auto ptr = reinterpret_cast<uint32_t *>(m_buffer) + (line * static_cast<uint64_t>(m_size.width)) + start;
    auto len = end - start;

    union simd_value v {};
    for (unsigned int & i : v.f) {
        i = c.rgba.value;
    }

#if __x86_64__
    uint32_t n = 0;
    while (n < len) {

        if ((reinterpret_cast<uint64_t>(ptr) & 0xF) || (len - n) < 4) {
            *ptr = blend_func_i(*ptr, v.f[n & 3]);
            ++ptr;
            ++n;
        }
        else {
            *reinterpret_cast<__int128 *>(ptr) = blend_func({ .wide = *reinterpret_cast<__int128*>(ptr) }, v).wide;
            ptr += 4;
            n += 4;
        }

    }
#elif __arm64__
    uint32_t n = 0;
    while (n < len) {
        if ((reinterpret_cast<uint64_t>(ptr) & 0x7) || (len - n) < 2) {
            *ptr = blend_func_i(*ptr, v.f[n & 1]);
            ++ptr;
            ++n;
        }
        else {
            *reinterpret_cast<uint64_t*>(ptr) = blend_func({ .wide = *reinterpret_cast<uint64_t*>(ptr) }, v).wide;
            ptr += 2;
            n += 2;
        }
    }
#else
    // Fallback on a default naive implementation.
    for (auto x = start; x < end; ++x) {
        *ptr++ = v.wide;
    }
#endif
}

auto graphics::rgba_buffer::apply_run(const std::vector<graphics::color> &cv, const uint64_t &start,
                                      const uint64_t &line) -> void
{
    if (line < 0 || line >= m_size.height) {
        return;
    }

    auto ptr = reinterpret_cast<uint32_t *>(m_buffer) + (line * static_cast<uint64_t>(m_size.width)) + start;
    auto len = cv.size();

    union simd_value v {};

#if __x86_64__
    uint32_t n = 0;
    uint64_t i = 0;
    while (n < len) {

        if ((reinterpret_cast<uint64_t>(ptr) & 0xF) || (len - n) < 4) {
            *ptr = blend_func_i(*ptr, cv[i++].rgba.value);
            ++ptr;
            ++n;
        }
        else {
            v.f[0] = cv[i++].rgba.value;
            v.f[1] = cv[i++].rgba.value;
            v.f[2] = cv[i++].rgba.value;
            v.f[3] = cv[i++].rgba.value;

            *reinterpret_cast<__int128 *>(ptr) = blend_func({ .wide = *reinterpret_cast<__int128*>(ptr) }, v).wide;
            ptr += 4;
            n += 4;
        }

    }
#elif __arm64__
    uint32_t n = 0;
    uint32_t i = 0;
    while (n < len) {
        if ((reinterpret_cast<uint64_t>(ptr) & 0x7) || (len - n) < 2) {
            *ptr = blend_func_i(*ptr, cv[i++].rgba.value);
            ++ptr;
            ++n;
        }
        else {
            v.f[0] = cv[i++].rgba.value;
            v.f[1] = cv[i++].rgba.value;
            *reinterpret_cast<uint64_t*>(ptr) = blend_func({ .wide = *reinterpret_cast<uint64_t*>(ptr) }, v).wide;
            ptr += 2;
            n += 2;
        }
    }
#else
    // Fallback on a default naive implementation.
    for (auto x = start; x < end; ++x) {
        *ptr++ = cv[i++].rgba.value;
    }
#endif
}

// MARK: - Masking

auto graphics::rgba_buffer::apply_mask(const graphics::rgba_buffer &buffer) -> void
{
    if (buffer.count() != count()) {
        // TODO: Warning here maybe?
        return;
    }

    auto len = count();
    auto ptr = reinterpret_cast<uint32_t *>(m_buffer);
    auto mask_ptr = reinterpret_cast<uint32_t *>(buffer.m_buffer);

    union simd_value v {};

    // MARK: This needs to be fixed and made more efficient.

    uint32_t n = 0;
    uint64_t i = 0;
    while (n < len) {
        *ptr = (*ptr & 0x00FFFFFF) | ((*mask_ptr & 0xFF) << 24);
        ptr++;
        mask_ptr++;
        n--;
    }
}
