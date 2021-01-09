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

#include "core/graphics/opengl/opengl_texture.hpp"

#include <utility>

// MARK: - Construction

graphics::opengl::texture::texture(const double& width, const double& height)
    : graphics::texture(width, height)
{
    glGenTextures(1, &m_id);
    upload();
}

graphics::opengl::texture::texture(const math::size& size)
    : graphics::texture(size)
{
    glGenTextures(1, &m_id);
    upload();
}

graphics::opengl::texture::texture(const double& width, const double& height, std::vector<uint32_t> data)
    : graphics::texture(width, height, std::move(data))
{
    glGenTextures(1, &m_id);
    upload();
}

graphics::opengl::texture::texture(const math::size& sz, std::vector<uint32_t> data)
    : graphics::texture(sz, std::move(data))
{
    glGenTextures(1, &m_id);
    upload();
}

graphics::opengl::texture::texture(const math::size &sz, const uint8_t *data)
    : graphics::texture(sz, data)
{
    glGenTextures(1, &m_id);
    upload();
}

// MARK: - Destruction

graphics::opengl::texture::~texture()
{
    glDeleteTextures(1, &m_id);
    m_id = 0;
}

// MARK: - Texture Management

auto graphics::opengl::texture::upload() const -> void
{
    if (m_id <= 0) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_id);

    if (m_raw_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format,
                     static_cast<int>(m_size.width), static_cast<int>(m_size.height), 0,
                     m_image_format, GL_UNSIGNED_BYTE, m_raw_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format,
                     static_cast<int>(m_size.width), static_cast<int>(m_size.height), 0,
                     m_image_format, GL_UNSIGNED_BYTE, m_data.data());
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter_max);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_env_mode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto graphics::opengl::texture::bind() const -> void
{
    if (m_id > 0) {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}
