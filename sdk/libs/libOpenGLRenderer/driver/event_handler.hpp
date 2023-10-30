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

#include <libOpenGLRenderer/opengl.hpp>
#include <libRenderCore/event/event_controller.hpp>

namespace renderer::opengl::event::receiver
{
    auto initialize(GLFWwindow *main_window) -> void;
    auto attach_controller(renderer::event::controller *controller) -> void;

    auto character_typed(GLFWwindow *window, std::uint32_t c) -> void;
    auto key(GLFWwindow *window, std::int32_t code, std::int32_t scancode, std::int32_t action, std::int32_t modifiers) -> void;
    auto mouse_moved(GLFWwindow *window, double x, double y) -> void;
    auto mouse_button(GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t modifiers) -> void;

    auto map_keycode(std::int32_t scancode) -> ::event::hid::key;
    auto map_modifiers(std::int32_t modifiers) -> ::event::type;
}