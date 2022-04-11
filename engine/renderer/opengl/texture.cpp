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
#include "renderer/opengl/texture.hpp"

// MARK: - Construction

renderer::opengl::texture::texture(const double& width, const double& height)
    : graphics::texture(width, height, true)
{
}

renderer::opengl::texture::texture(const math::size& size)
    : graphics::texture(size, true)
{
}

renderer::opengl::texture::texture(const double& width, const double& height, std::vector<uint32_t> data)
    : graphics::texture(width, height, data)
{
}

renderer::opengl::texture::texture(const math::size& sz, std::vector<uint32_t> data)
    : graphics::texture(sz, data)
{
}

renderer::opengl::texture::texture(const math::size& sz, const uint8_t *data)
    : graphics::texture(sz, data)
{
}

renderer::opengl::texture::texture(GLuint handle, const math::size& sz)
    : graphics::texture(sz, false), m_id(handle)
{
}

// MARK: - Destruction

renderer::opengl::texture::~texture()
{
    glDeleteTextures(1, &m_id);
}

// MARK: - Accessors

auto renderer::opengl::texture::handle() const -> uint64_t
{
    return static_cast<uint64_t>(m_id);
}
