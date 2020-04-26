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

#include "foundation/gl/sprite.hpp"

// MARK: - Constructors

kestrel::gl::sprite::sprite(const gl::spritesheet& sheet, const math::point& point, const math::size& size)
{
    auto texture = sheet.texture();

    // Calculate some constants ahead of time, such as texture size and uv size.
    auto texture_size = texture->size();
    auto uv_width = (1.0 / texture_size.width()) * size.width();
    auto uv_height = (1.0 / texture_size.height()) * size.height();

    // Calculate the UV mapping of the sprite within the texture.
    auto uv_origin = math::point(point.x() * uv_width, point.y() * uv_height);
    m_uv = math::rect(uv_origin, math::size(uv_width, uv_height));

    // Calculate the frame of the texture
    auto origin = math::point(point.x() * size.width(), point.y() * size.height());
    m_frame = math::rect(origin, size);
}

auto kestrel::gl::sprite::create(const gl::spritesheet& sheet, const math::point& point, const math::size& size) -> std::shared_ptr<sprite>
{
    return std::make_shared<sprite>(sheet, point, size);
}

// MARK: - Accessors

auto kestrel::gl::sprite::frame() const -> math::rect
{
    return m_frame;
}

auto kestrel::gl::sprite::uv() const -> math::rect
{
    return m_uv;
}

auto kestrel::gl::sprite::set_uv_index(const int index) -> void
{
    m_uv_index = index;
}

// MARK: - Drawing

auto kestrel::gl::sprite::draw(const math::point& p, const gl::spritesheet& sheet) const -> void
{
    draw(p.vector(), sheet);
}

auto kestrel::gl::sprite::draw(const math::vector& v, const gl::spritesheet& sheet) const -> void
{
    sheet.texture()->draw(math::rect(math::point(v), m_frame.size()).diamane(), m_uv_index);
}
