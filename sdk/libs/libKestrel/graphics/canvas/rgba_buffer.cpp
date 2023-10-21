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
#   include <emmintrin.h>
#endif

#include <libKestrel/graphics/canvas/rgba_buffer.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>

// MARK: - Construction

kestrel::graphics::rgba_buffer::rgba_buffer(const math::size &sz)
    : m_size(sz)
{
    common_construction();
}

kestrel::graphics::rgba_buffer::rgba_buffer(const std::vector<color::value>& data, const math::size& sz)
    : m_size(sz)
{
    // We need
    common_construction();

    // Copy the data into the buffer. This code path is _not_ optimal and there should be some effort in the future
    // to optimise this. However this is a low frequency path and as such is not worth immediate optimisation.
    auto dst = reinterpret_cast<std::uint32_t *>(m_buffer);
    for (const auto& i : data) {
        *dst++ = i;
    }
}

auto kestrel::graphics::rgba_buffer::common_construction() -> void
{
    m_count = static_cast<std::uint32_t>(m_size.area()) << 2;
    m_buffer = static_cast<std::uint8_t *>(calloc(m_count, 1)); // 4 Channel Color (RGBA)
    clear_clipping_rect();
}

// MARK: - Destruction

kestrel::graphics::rgba_buffer::~rgba_buffer()
{
    free(m_buffer);
}

// MARK: - Clipping

auto kestrel::graphics::rgba_buffer::set_clipping_rect(const math::rect &r) -> void
{
    m_clipping_rect = r;
}

auto kestrel::graphics::rgba_buffer::clear_clipping_rect() -> void
{
    m_clipping_rect = math::rect(math::point(0, 0), m_size);
}

// MARK: - Accessors

auto kestrel::graphics::rgba_buffer::count() const -> std::size_t
{
    return m_count;
}

auto kestrel::graphics::rgba_buffer::size() const -> math::size
{
    return m_size;
}

auto kestrel::graphics::rgba_buffer::data() const -> std::uint8_t *
{
    return m_buffer;
}

auto kestrel::graphics::rgba_buffer::data_block() const -> data::block
{
    return std::move(data::block(m_buffer, m_count, false, data::native_byte_order()));
}

// MARK: - Look Up

auto kestrel::graphics::rgba_buffer::corrected(std::uint64_t i) -> std::uint64_t
{
    return (i << 2);
}

auto kestrel::graphics::rgba_buffer::index(const math::point &p) const -> std::uint64_t
{
    return (static_cast<std::uint64_t>(p.y()) * static_cast<std::uint64_t>(m_size.width())) + static_cast<std::uint64_t>(p.x());
}

auto kestrel::graphics::rgba_buffer::at(std::uint64_t i) const -> color::value
{
    auto ptr = reinterpret_cast<color::value *>(m_buffer) + i;
    return *ptr;
}

auto kestrel::graphics::rgba_buffer::color(const math::point &p) const -> graphics::color
{
    auto value = at(index(p));
    return graphics::color(value);
}

// MARK: - Operations
// TODO: The following operations need to be optimised to use SIMD instructions where possible.
// Each of these operations will likely need differing versions for Intel/ARM machines.

auto kestrel::graphics::rgba_buffer::blend_func_i(color::value bottom, color::value top) -> color::value
{
    std::uint8_t ta = (top >> 24);
    std::uint8_t ba = (bottom >> 24);

    color::value rb = bottom & 0xff00ff;
    color::value g  = bottom & 0x00ff00;
    std::uint8_t a = ta + ((ba * (0x100 - ta)) >> 8);
    rb += (((top & 0xff00ff) - rb) * ta) >> 8;
    g  += (((top & 0x00ff00) -  g) * ta) >> 8;

    return (rb & 0xff00ff) | (g & 0xff00) | (a << 24);
};

auto kestrel::graphics::rgba_buffer::blend_func(const union simd_value& bottom, const union simd_value& top) -> union simd_value
{
    union simd_value r;

#if TARGET_SUPPORTS_128BIT
    r.f[0] = blend_func_i(bottom.f[0], top.f[0]);
    r.f[1] = blend_func_i(bottom.f[1], top.f[1]);
    r.f[2] = blend_func_i(bottom.f[2], top.f[2]);
    r.f[3] = blend_func_i(bottom.f[3], top.f[3]);
#elif TARGET_64BIT
    r.f[0] = blend_func_i(bottom.f[0], top.f[0]);
    r.f[1] = blend_func_i(bottom.f[1], top.f[1]);
#else
    r.wide = top.wide;
#endif

    return r;
}

auto kestrel::graphics::rgba_buffer::clear(const graphics::color &c) -> void
{
    clear_rect(c, {math::point(0), m_size });
}

auto kestrel::graphics::rgba_buffer::clear_rect(const graphics::color &c, const math::rect &r) -> void
{
    auto lo_x = static_cast<std::uint32_t>(std::max(r.x(), m_clipping_rect.x()));
    auto lo_y = static_cast<std::uint32_t>(std::max(r.y(), m_clipping_rect.y()));
    auto hi_x = static_cast<std::uint32_t>(std::min(r.width() + r.x(), m_clipping_rect.max_x()));
    auto hi_y = static_cast<std::uint32_t>(std::min(r.height() + r.y(), m_clipping_rect.max_y()));
    auto width = hi_x - lo_x;
    auto height = hi_y - lo_y;

    auto stride = static_cast<std::uint64_t>(m_size.width());
    auto pitch = stride - width;
    auto ptr = reinterpret_cast<color::value *>(m_buffer) + (lo_y * static_cast<std::uint64_t>(m_size.width())) + lo_x;
    auto ptr_value = reinterpret_cast<std::uint64_t>(m_buffer);

    union simd_value v {};
    for (unsigned int & i : v.f) {
        i = c.rgba.value;
    }

    for (auto scanline = 0; scanline < height; ++scanline) {
#if TARGET_SUPPORTS_128BIT && 0
        std::uint32_t n = 0;
        std::uint64_t i = 0;
        while (n < stride) {

            if ((reinterpret_cast<std::uint64_t>(ptr) & 0xF) || (stride - n) < 4) {
                *ptr = v.f[n & 3];
                ++ptr;
                ++n;
            }
            else {
                *reinterpret_cast<unsigned __int128*>(ptr) = v.wide;
                ptr += 4;
                n += 4;
            }

        }
        ptr += pitch;

#elif TARGET_64BIT
        std::uint32_t n = 0;
        while (n < stride) {
            if ((ptr_value & 0x7) || (stride - n) < 2) {
                *ptr = v.f[n & 1];
                ++ptr;
                ++n;
                ptr_value += 4;
            }
            else {
                *reinterpret_cast<std::uint64_t*>(ptr) = v.wide;
                ptr += 2;
                n += 2;
                ptr_value += 8;
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

auto kestrel::graphics::rgba_buffer::draw_pixel(const graphics::color& c, const math::point& p) -> void
{
    if (p.x() < m_clipping_rect.x() || p.x() >= m_clipping_rect.max_x() || p.y() < m_clipping_rect.y() || p.y() >= m_clipping_rect.max_y()) {
        return;
    }
    apply_run(c, p.x(), p.x() + 1, p.y());
}

auto kestrel::graphics::rgba_buffer::fill_rect(const graphics::color& c, const math::rect& r) -> void
{
    auto lo_x = std::max(static_cast<int>(r.x()), 0);
    auto lo_y = std::max(static_cast<int>(r.y()), 0);
    auto hi_x = std::min(static_cast<int>(r.width() + r.x()), static_cast<int>(m_size.width() - 1));
    auto hi_y = std::min(static_cast<int>(r.height() + r.y()), static_cast<int>(m_size.height()));

    if (lo_x >= hi_x || lo_y >= hi_y) {
        return;
    }

    for (auto y = lo_y; y < hi_y; ++y) {
        apply_run(c, lo_x, hi_x, y);
    }
}

auto kestrel::graphics::rgba_buffer::apply_run(const graphics::color &c, std::uint64_t start, std::uint64_t end, std::uint64_t line) -> void
{
    if (line < m_clipping_rect.y() ||
        line >= m_clipping_rect.max_y() ||
        start >= end ||
        start >= m_clipping_rect.max_x()
    ) {
        return;
    }

    auto ptr = reinterpret_cast<color::value *>(m_buffer) + (line * static_cast<std::uint64_t>(m_size.width())) + start;
    auto len = end - start;

    union simd_value v {};
    for (unsigned int & i : v.f) {
        i = c.rgba.value;
    }

#if TARGET_SUPPORTS_128BIT
    std::uint32_t n = 0;
    while (n < len) {

        if ((reinterpret_cast<std::uint64_t>(ptr) & 0xF) || (len - n) < 4) {
            *ptr = blend_func_i(*ptr, v.f[n & 3]);
            ++ptr;
            ++n;
        }
        else {
            *reinterpret_cast<unsigned __int128 *>(ptr) = blend_func({ .wide = *reinterpret_cast<unsigned __int128*>(ptr) }, v).wide;
            ptr += 4;
            n += 4;
        }

    }
#elif TARGET_64BIT
    std::uint32_t n = 0;
    while (n < len) {
        if ((reinterpret_cast<std::uint64_t>(ptr) & 0x7) || (len - n) < 2) {
            *ptr = blend_func_i(*ptr, v.f[n & 1]);
            ++ptr;
            ++n;
        }
        else {
            *reinterpret_cast<std::uint64_t*>(ptr) = blend_func({ .wide = *reinterpret_cast<std::uint64_t*>(ptr) }, v).wide;
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

auto kestrel::graphics::rgba_buffer::apply_run(const std::vector<graphics::color> &cv, std::uint64_t start, std::uint64_t line) -> void
{
    if (line < m_clipping_rect.y() || line >= m_clipping_rect.max_y()) {
        return;
    }

    auto ptr = reinterpret_cast<color::value *>(m_buffer) + (line * static_cast<std::uint64_t>(m_size.width())) + start;
    auto len = cv.size();
    len = (len > m_clipping_rect.width()) ? m_clipping_rect.width() : len;

    union simd_value v {};

#if TARGET_SUPPORTS_128BIT
    std::uint32_t n = 0;
    std::uint64_t i = 0;
    while (n < len) {

        if ((reinterpret_cast<std::uint64_t>(ptr) & 0xF) || (len - n) < 4) {
            *ptr = blend_func_i(*ptr, cv[i++].rgba.value);
            ++ptr;
            ++n;
        }
        else {
            v.f[0] = cv[i++].rgba.value;
            v.f[1] = cv[i++].rgba.value;
            v.f[2] = cv[i++].rgba.value;
            v.f[3] = cv[i++].rgba.value;

            *reinterpret_cast<unsigned __int128 *>(ptr) = blend_func({ .wide = *reinterpret_cast<unsigned __int128*>(ptr) }, v).wide;
            ptr += 4;
            n += 4;
        }

    }
#elif TARGET_64BIT
    std::uint32_t n = 0;
    std::uint32_t i = 0;
    while (n < len) {
        if ((reinterpret_cast<std::uint64_t>(ptr) & 0x7) || (len - n) < 2) {
            *ptr = blend_func_i(*ptr, cv[i++].rgba.value);
            ++ptr;
            ++n;
        }
        else {
            v.f[0] = cv[i++].rgba.value;
            v.f[1] = cv[i++].rgba.value;
            *reinterpret_cast<std::uint64_t*>(ptr) = blend_func({ .wide = *reinterpret_cast<std::uint64_t*>(ptr) }, v).wide;
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

auto kestrel::graphics::rgba_buffer::apply_run(const data::block& cv, std::uint64_t start, std::uint64_t line) -> void
{
    if (line < m_clipping_rect.y() || line >= m_clipping_rect.max_y()) {
        return;
    }

    auto ptr = reinterpret_cast<color::value *>(m_buffer) + (line * static_cast<std::uint64_t>(m_size.width())) + start;
    auto len = cv.size() >> 2; // We're looking at bytes, not colors, so divide by 4 to account for the components
    len = (len > m_clipping_rect.width()) ? m_clipping_rect.width() : len;

    union simd_value v {};

#if TARGET_SUPPORTS_128BIT
    std::uint32_t n = 0;
    std::uint64_t i = 0;
    while (n < len) {

        if ((reinterpret_cast<std::uint64_t>(ptr) & 0xF) || (len - n) < 4) {
            *ptr = blend_func_i(*ptr, cv.get<color::value>(i));
            i += 4;
            ++ptr;
            ++n;
        }
        else {
            v.f[0] = cv.get<uint32_t>(i); i += 4;
            v.f[1] = cv.get<uint32_t>(i); i += 4;
            v.f[2] = cv.get<uint32_t>(i); i += 4;
            v.f[3] = cv.get<uint32_t>(i); i += 4;

            *reinterpret_cast<unsigned __int128 *>(ptr) = blend_func({ .wide = *reinterpret_cast<unsigned __int128*>(ptr) }, v).wide;
            ptr += 4;
            n += 4;
        }

    }
#elif TARGET_64BIT
    std::uint32_t n = 0;
    std::uint32_t i = 0;
    while (n < len) {
        if ((reinterpret_cast<std::uint64_t>(ptr) & 0x7) || (len - n) < 2) {
            *ptr = blend_func_i(*ptr, cv.get<color::value>(i));
            i += 4;
            ++ptr;
            ++n;
        }
        else {
            v.f[0] = cv.get<color::value>(i);
            i += 4;
            v.f[1] = cv.get<color::value>(i);
            i += 4;
            *reinterpret_cast<std::uint64_t*>(ptr) = blend_func({ .wide = *reinterpret_cast<std::uint64_t*>(ptr) }, v).wide;
            ptr += 2;
            n += 2;
        }
    }
#else
    // Fallback on a default naive implementation.
    for (auto x = start; x < end; ++x) {
        *ptr++ = cv.get<color::value>(i); i += 4;
    }
#endif
}

// MARK: - Masking

auto kestrel::graphics::rgba_buffer::apply_mask(const graphics::rgba_buffer &buffer) -> void
{
    if (buffer.count() != count()) {
        // TODO: Warning here maybe?
        return;
    }

    auto len = count() >> 2;
    auto ptr = reinterpret_cast<color::value *>(m_buffer);
    auto mask_ptr = reinterpret_cast<color::value *>(buffer.m_buffer);

    // TODO: This needs to be made more efficient on each architecture

    std::uint32_t n = 0;
    do {
        ptr[n] &= 0x00FFFFFF;
        ptr[n] |= ((0xFF - (mask_ptr[n] & 0xFF)) << 24);
    } while (++n < len);
}
