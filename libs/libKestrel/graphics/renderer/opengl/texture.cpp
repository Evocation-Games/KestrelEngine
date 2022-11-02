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
#include <libKestrel/graphics/renderer/opengl/texture.hpp>

// MARK: - Construction

kestrel::renderer::opengl::texture::texture(std::uint32_t width, std::uint32_t height)
    : graphics::texture(width, height, true)
{
}

kestrel::renderer::opengl::texture::texture(const math::size& size)
    : graphics::texture(size, true)
{
}

kestrel::renderer::opengl::texture::texture(std::uint32_t width, std::uint32_t height, const graphite::data::block& data)
    : graphics::texture(width, height, data)
{
}

kestrel::renderer::opengl::texture::texture(const math::size& sz, const graphite::data::block& data)
    : graphics::texture(sz, data)
{
}

kestrel::renderer::opengl::texture::texture(const math::size& sz, const std::uint8_t *data)
    : graphics::texture(sz, data)
{
}

kestrel::renderer::opengl::texture::texture(GLuint handle, const math::size& sz)
    : graphics::texture(sz, false), m_id(handle)
{
}

// MARK: - Destruction

kestrel::renderer::opengl::texture::~texture()
{
    if (uploaded()) {
        glDeleteTextures(1, &m_id);
    }
}

// MARK: - Accessors

auto kestrel::renderer::opengl::texture::handle() const -> std::uint64_t
{
    return static_cast<std::uint64_t>(m_id);
}

// MARK: - Upload

auto kestrel::renderer::opengl::texture::upload_to_gpu() -> void
{
    glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(m_size.width), static_cast<GLsizei>(m_size.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.get<void *>());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    graphics::texture::upload_to_gpu();
}