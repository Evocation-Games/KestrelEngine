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

#pragma once

#include <type_traits>
#include <libKestrel/graphics/texture/texture.hpp>
#include <libKestrel/graphics/renderer/opengl/opengl.hpp>
#include <libData/block.hpp>

namespace kestrel::renderer::opengl
{
    class texture : public graphics::texture
    {
    public:
        texture(std::uint32_t width, std::uint32_t height);
        explicit texture(const math::size& size);
        texture(std::uint32_t width, std::uint32_t height, const data::block& data);
        texture(const math::size& size, const data::block& data);
        texture(const math::size& size, const std::uint8_t *data);
        texture(GLuint handle, const math::size& sz);

        ~texture();

        auto handle() const -> std::uint64_t override;

        auto upload_to_gpu() -> void override;

    private:
        GLuint m_id { 0 };
        GLuint m_internal_format { GL_RGBA };
        GLuint m_image_format { GL_RGBA };
        GLuint m_wrap_s { GL_REPEAT };
        GLuint m_wrap_t { GL_REPEAT };
        GLuint m_filter_min { GL_NEAREST };
        GLuint m_filter_max { GL_NEAREST };
    };
}
