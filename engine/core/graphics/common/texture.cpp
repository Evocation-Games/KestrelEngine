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

graphics::texture::texture(const double& width, const double& height)
    : m_size(width, height), m_data(width * height, 0xFFFFFFFF)
{

}

graphics::texture::texture(const math::size& size)
    : m_size(size), m_data(size.width * size.height, 0xFFFFFFFF)
{

}

graphics::texture::texture(const double& width, const double& height, std::vector<uint32_t> data)
    : m_size(width, height), m_data(std::move(data))
{

}

graphics::texture::texture(const math::size& sz, std::vector<uint32_t> data)
    : m_size(sz), m_data(std::move(data))
{

}

graphics::texture::texture(const math::size &sz, const uint8_t *data)
    : m_size(sz), m_raw_data(data)
{

}

// MARK: - Accessors

auto graphics::texture::size() const -> math::size
{
    return m_size;
}

auto graphics::texture::data() const -> std::vector<uint32_t>
{
    return m_data;
}

auto graphics::texture::raw_data_ptr() const -> const uint8_t *
{
    return m_raw_data;
}

auto graphics::texture::handle() const -> int
{
    return 0;
}

// MARK: - Base Implementations

auto graphics::texture::bind() const -> void
{
    // Empty Implementation
}

auto graphics::texture::destroy() -> void
{

}

