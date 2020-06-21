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

#include "core/graphics/common/session_window.hpp"
#include "core/graphics/opengl/opengl.hpp"

namespace graphics { namespace opengl {

    class session_window: public graphics::session_window
    {
    protected:
        GLFWwindow *m_window { nullptr };

        auto configure_viewport(GLdouble width, GLdouble height) -> void;

    public:
        explicit session_window(std::shared_ptr<environment> env);

        auto render() -> void override;
    };

}};

#endif //KESTREL_OPENGL_SESSION_WINDOW_HPP
