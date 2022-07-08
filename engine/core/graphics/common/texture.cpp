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

#include "core/graphics/common/texture.hpp"

// MARK: - Construction

graphics::texture::texture(uint32_t width, uint32_t height, const graphite::data::block &data)
    : m_data(data),
      m_size(width, height)
{
}

graphics::texture::texture(const math::size &size, const graphite::data::block &data)
    : m_data(data),
      m_size(size)
{
}

graphics::texture::texture(uint32_t width, uint32_t height, bool populate)
    : m_data(width * height * 4),
      m_size(width, height)
{
    if (populate) {
        m_data.set(static_cast<uint32_t>(0xFFFF00FF), m_data.size());
    }
}

graphics::texture::texture(const math::size &size, bool populate)
    : m_data(size.width * size.height * 4),
      m_size(size)
{
    if (populate) {
        m_data.set(static_cast<uint32_t>(0xFFFF00FF), m_data.size());
    }
}

// MARK: - Accessors

auto graphics::texture::size() const -> math::size
{
    return m_size;
}

auto graphics::texture::data() const -> const graphite::data::block&
{
    return m_data;
}

auto graphics::texture::raw_data_ptr() const -> const void *
{
    return m_data.get<void *>();
}

auto graphics::texture::set_data(const graphite::data::block& data) -> void
{
    m_data = data;
}

auto graphics::texture::handle() const -> uint64_t
{
    return 0;
}

// MARK: - Base Implementations

auto graphics::texture::destroy() -> void
{
    // Empty Implementation
}

auto graphics::texture::upload_to_gpu() -> void
{
    m_uploaded = true;
}

auto graphics::texture::uploaded() const -> bool
{
    return m_uploaded;
}

