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

#include "core/graphics/metal/metal_texture.h"

// MARK: - Construction

graphics::metal::texture::texture(const double &width, const double &height)
    : graphics::texture(width, height)
{

}

graphics::metal::texture::texture(const math::size &size)
    : graphics::texture(size)
{

}

graphics::metal::texture::texture(const double &width, const double &height, std::vector<uint32_t> data)
    : graphics::texture(width, height, data)
{

}

graphics::metal::texture::texture(const math::size &size, std::vector<uint32_t> data)
    : graphics::texture(size, data)
{

}

graphics::metal::texture::texture(const math::size &size, const uint8_t *data)
    : graphics::texture(size, data)
{

}

// MARK: - Accessors

auto graphics::metal::texture::handle() const -> int
{
    return m_handle;
}

auto graphics::metal::texture::set_handle(const int &handle) -> void
{
    m_handle = handle;
}

// MARK: - Texture Management

auto graphics::metal::texture::bind() const -> void
{

}

