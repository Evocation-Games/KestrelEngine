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

#if !defined(KESTREL_OPENGL_SHADER_HPP) && __x86_64__
#define KESTREL_OPENGL_SHADER_HPP

#include <string>
#include <memory>
#include "core/graphics/common/shader.hpp"
#include "core/graphics/opengl/opengl.hpp"

namespace graphics::opengl
{

    class shader: public graphics::shader
    {
    protected:
        GLuint m_id { 0 };

        auto check(GLuint object, const std::string& type) -> void;
    public:
        shader(const int64_t& vertex_id, const int64_t& fragment_id);

        auto use() -> std::shared_ptr<graphics::shader> override;

        auto set_float(const std::string& name, float value, bool use_shader = false) -> void;
        auto set_integer(const std::string& name, float value, bool use_shader = false) -> void;
        auto set_vec2(const std::string& name, float x, float y, bool use_shader = false) -> void;
        auto set_vec2(const std::string& name, glm::vec2 &value, bool use_shader = false) -> void;
        auto set_vec3(const std::string& name, float x, float y, float z, bool use_shader = false) -> void;
        auto set_vec3(const std::string& name, glm::vec3 &value, bool use_shader = false) -> void;
        auto set_vec4(const std::string& name, float x, float y, float z, float w, bool use_shader = false) -> void;
        auto set_vec4(const std::string& name, glm::vec4 &value, bool use_shader = false) -> void;
        auto set_mat4(const std::string& name, glm::mat4 &matrix, bool use_shader = false) -> void;
    };

}

#endif //KESTREL_OPENGL_SHADER_HPP
