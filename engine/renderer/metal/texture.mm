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


#include <utility>
#include "renderer/metal/texture.h"

// MARK: - Construction

renderer::metal::texture::texture(uint32_t width, uint32_t height)
    : graphics::texture(width, height, true)
{
}

renderer::metal::texture::texture(const math::size& size)
    : graphics::texture(size, true)
{
}

renderer::metal::texture::texture(uint32_t width, uint32_t height, const graphite::data::block& data)
    : graphics::texture(width, height, data)
{
}

renderer::metal::texture::texture(const math::size& sz, const graphite::data::block& data)
    : graphics::texture(sz, data)
{
}

renderer::metal::texture::texture(const math::size& sz, const uint8_t *data)
    : graphics::texture(sz, data)
{
}

renderer::metal::texture::texture(const void *handle, const math::size& sz)
    : graphics::texture(sz, false), m_handle(reinterpret_cast<uint64_t>(const_cast<void *>(handle)))
{
}

// MARK: - Destruction

renderer::metal::texture::~texture()
{
}

// MARK: - Accessors

auto renderer::metal::texture::handle() const -> uint64_t
{
    return m_handle;
}
