// Copyright (c) 2023 Tom Hancocks
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

#include <libOpenGLRenderer/resource/texture.hpp>

namespace renderer::opengl
{
    struct texture
    {
        renderer::texture::device_id device_id { 0 };
        GLuint handle { 0 };
        GLuint internal_format { GL_RGBA };
        GLuint image_format { GL_RGBA };
        GLuint wrap_s { GL_REPEAT };
        GLuint wrap_t { GL_REPEAT };
        GLuint filter_min { GL_NEAREST };
        GLuint filter_mag { GL_NEAREST };
        GLsizei width { 0 };
        GLsizei height { 0 };
        bool uploaded { false };
    };
}