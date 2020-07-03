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

#if !defined(KESTREL_OPENGL_SESSION_WINDOW_HPP)
#define KESTREL_OPENGL_SESSION_WINDOW_HPP

#include <optional>
#include "core/graphics/common/session_window.hpp"
#include "core/graphics/opengl/opengl.hpp"
#include "core/graphics/opengl/opengl_shader.hpp"
#include "core/graphics/opengl/opengl_sprite_renderer.hpp"

namespace graphics { namespace opengl {

class session_window: public graphics::session_window, public std::enable_shared_from_this<opengl::session_window>
    {
    protected:
        GLFWwindow *m_window { nullptr };
        std::optional<opengl::sprite_renderer> m_sprite_renderer;

        auto configure_viewport(GLdouble width, GLdouble height) -> void;

    public:
        explicit session_window(std::shared_ptr<environment> env);

        auto new_scene() -> std::shared_ptr<graphics::scene> override;

        auto set_title(const std::string& title) -> void override;
        auto set_size(const math::size& size) -> void override;

        auto sprite_renderer() const -> std::optional<opengl::sprite_renderer>;

        auto render() -> void override;

        auto create_texture(const math::size& size, std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture> override;
    };

}};

#endif //KESTREL_OPENGL_SESSION_WINDOW_HPP
