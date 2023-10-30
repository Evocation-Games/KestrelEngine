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

#include <libOpenGLRenderer/driver/event_handler.hpp>

// MARK: - Storage

static struct {
    renderer::event::controller *controller { nullptr };
} s_opengl_event_receiver;

// MARK: - Setup

auto renderer::opengl::event::receiver::initialize(GLFWwindow *main_window) -> void
{
    glfwSetCharCallback(main_window, &character_typed);
    glfwSetKeyCallback(main_window, &key);
    glfwSetCursorPosCallback(main_window, &mouse_moved);
    glfwSetMouseButtonCallback(main_window, &mouse_button);
}

// MARK: - Events

auto renderer::opengl::event::receiver::character_typed(GLFWwindow *window, std::uint32_t c) -> void
{
    ::event::instance raw;
    raw.type = ::event::type::key_typed;
    raw.key.pressed = ::event::hid::unknown;
    raw.key.character = (std::int32_t)c;
    s_opengl_event_receiver.controller->send(raw);
}

auto renderer::opengl::event::receiver::key(
    GLFWwindow *window,
    std::int32_t code,
    std::int32_t scancode,
    std::int32_t action,
    std::int32_t modifiers
) -> void {
    ::event::instance raw;
    raw.type = static_cast<::event::type>(map_modifiers(modifiers));
    raw.key.pressed = ::event::hid::unknown;
    raw.key.character = map_keycode(code);

    switch (action) {
        case GLFW_PRESS: {
            raw.type = static_cast<::event::type>(raw.type | ::event::type::key_down);
            break;
        }
        case GLFW_RELEASE: {
            raw.type = static_cast<::event::type>(raw.type | ::event::type::key_up);
            break;
        }
        case GLFW_REPEAT: {
            raw.type = static_cast<::event::type>(raw.type | ::event::type::key_repeat);
            break;
        }
        default: break;
    }

    s_opengl_event_receiver.controller->send(raw);
}

auto renderer::opengl::event::receiver::mouse_moved(GLFWwindow *window, double x, double y) -> void
{
    ::event::instance raw;
    raw.type = ::event::type::mouse_move;
    raw.location.x = (float)x;
    raw.location.y = (float)y;
    s_opengl_event_receiver.controller->send(raw);
}

auto renderer::opengl::event::receiver::mouse_button(GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t modifiers) -> void
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::event::instance raw;
    raw.location.x = (float)x;
    raw.location.y = (float)y;

    switch (action) {
        case GLFW_PRESS: {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT: {
                    raw.type = ::event::type::lmb_down;
                    break;
                }
                case GLFW_MOUSE_BUTTON_MIDDLE: {
                    raw.type = ::event::type::mmb_down;
                    break;
                }
                case GLFW_MOUSE_BUTTON_RIGHT: {
                    raw.type = ::event::type::rmb_down;
                    break;
                }
                default: break;
            }
        }
        case GLFW_RELEASE: {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT: {
                    raw.type = ::event::type::lmb_up;
                    break;
                }
                case GLFW_MOUSE_BUTTON_MIDDLE: {
                    raw.type = ::event::type::mmb_up;
                    break;
                }
                case GLFW_MOUSE_BUTTON_RIGHT: {
                    raw.type = ::event::type::rmb_up;
                    break;
                }
                default: break;
            }
        }
        default: break;
    }

    s_opengl_event_receiver.controller->send(raw);
}

// MARK: - Mappings


auto renderer::opengl::event::receiver::map_keycode(std::int32_t scancode) -> ::event::hid::key
{
    switch (scancode) {
        // Special Keys
        case GLFW_KEY_ESCAPE:               return ::event::hid::key::escape;
        case GLFW_KEY_ENTER:                return ::event::hid::key::enter;
        case GLFW_KEY_BACKSPACE:            return ::event::hid::key::backspace;
        case GLFW_KEY_TAB:                  return ::event::hid::key::tab;

            // Cursor Keys
        case GLFW_KEY_LEFT:                 return ::event::hid::key::left;
        case GLFW_KEY_RIGHT:                return ::event::hid::key::right;
        case GLFW_KEY_UP:                   return ::event::hid::key::up;
        case GLFW_KEY_DOWN:                 return ::event::hid::key::down;

            // Letters
        case GLFW_KEY_A:                    return ::event::hid::key::a;
        case GLFW_KEY_B:                    return ::event::hid::key::b;
        case GLFW_KEY_C:                    return ::event::hid::key::c;
        case GLFW_KEY_D:                    return ::event::hid::key::d;
        case GLFW_KEY_E:                    return ::event::hid::key::e;
        case GLFW_KEY_F:                    return ::event::hid::key::f;
        case GLFW_KEY_G:                    return ::event::hid::key::g;
        case GLFW_KEY_H:                    return ::event::hid::key::h;
        case GLFW_KEY_I:                    return ::event::hid::key::i;
        case GLFW_KEY_J:                    return ::event::hid::key::j;
        case GLFW_KEY_K:                    return ::event::hid::key::k;
        case GLFW_KEY_L:                    return ::event::hid::key::l;
        case GLFW_KEY_M:                    return ::event::hid::key::m;
        case GLFW_KEY_N:                    return ::event::hid::key::n;
        case GLFW_KEY_O:                    return ::event::hid::key::o;
        case GLFW_KEY_P:                    return ::event::hid::key::p;
        case GLFW_KEY_Q:                    return ::event::hid::key::q;
        case GLFW_KEY_R:                    return ::event::hid::key::r;
        case GLFW_KEY_S:                    return ::event::hid::key::s;
        case GLFW_KEY_T:                    return ::event::hid::key::t;
        case GLFW_KEY_U:                    return ::event::hid::key::u;
        case GLFW_KEY_V:                    return ::event::hid::key::v;
        case GLFW_KEY_W:                    return ::event::hid::key::w;
        case GLFW_KEY_X:                    return ::event::hid::key::x;
        case GLFW_KEY_Y:                    return ::event::hid::key::y;
        case GLFW_KEY_Z:                    return ::event::hid::key::z;

            // Numbers
        case GLFW_KEY_0:
        case GLFW_KEY_KP_0:                 return ::event::hid::key::kp_0;
        case GLFW_KEY_1:
        case GLFW_KEY_KP_1:                 return ::event::hid::key::kp_1;
        case GLFW_KEY_2:
        case GLFW_KEY_KP_2:                 return ::event::hid::key::kp_2;
        case GLFW_KEY_3:
        case GLFW_KEY_KP_3:                 return ::event::hid::key::kp_3;
        case GLFW_KEY_4:
        case GLFW_KEY_KP_4:                 return ::event::hid::key::kp_4;
        case GLFW_KEY_5:
        case GLFW_KEY_KP_5:                 return ::event::hid::key::kp_5;
        case GLFW_KEY_6:
        case GLFW_KEY_KP_6:                 return ::event::hid::key::kp_6;
        case GLFW_KEY_7:
        case GLFW_KEY_KP_7:                 return ::event::hid::key::kp_7;
        case GLFW_KEY_8:
        case GLFW_KEY_KP_8:                 return ::event::hid::key::kp_8;
        case GLFW_KEY_9:
        case GLFW_KEY_KP_9:                 return ::event::hid::key::kp_9;

            // Function Keys
        case GLFW_KEY_F1:                   return ::event::hid::key::f1;
        case GLFW_KEY_F2:                   return ::event::hid::key::f2;
        case GLFW_KEY_F3:                   return ::event::hid::key::f3;
        case GLFW_KEY_F4:                   return ::event::hid::key::f4;
        case GLFW_KEY_F5:                   return ::event::hid::key::f5;
        case GLFW_KEY_F6:                   return ::event::hid::key::f6;
        case GLFW_KEY_F7:                   return ::event::hid::key::f7;
        case GLFW_KEY_F8:                   return ::event::hid::key::f8;
        case GLFW_KEY_F9:                   return ::event::hid::key::f9;
        case GLFW_KEY_F10:                  return ::event::hid::key::f10;
        case GLFW_KEY_F11:                  return ::event::hid::key::f11;
        case GLFW_KEY_F12:                  return ::event::hid::key::f12;

            // Misc
        case GLFW_KEY_SPACE:                return ::event::hid::key::space;
        case GLFW_KEY_LEFT_BRACKET:         return ::event::hid::key::left_bracket;
        case GLFW_KEY_RIGHT_BRACKET:        return ::event::hid::key::right_bracket;
        case GLFW_KEY_APOSTROPHE:           return ::event::hid::key::apostrophe;
        case GLFW_KEY_PERIOD:               return ::event::hid::key::period;
        case GLFW_KEY_COMMA:                return ::event::hid::key::comma;
        case GLFW_KEY_MINUS:                return ::event::hid::key::minus;
        case GLFW_KEY_EQUAL:                return ::event::hid::key::equal;

            // Modifiers
        case GLFW_KEY_LEFT_SHIFT:           return ::event::hid::key::left_shift;
        case GLFW_KEY_RIGHT_SHIFT:          return ::event::hid::key::right_shift;
        case GLFW_KEY_LEFT_ALT:             return ::event::hid::key::left_alt;
        case GLFW_KEY_RIGHT_ALT:            return ::event::hid::key::right_alt;
        case GLFW_KEY_LEFT_CONTROL:         return ::event::hid::key::left_control;
        case GLFW_KEY_RIGHT_CONTROL:        return ::event::hid::key::right_control;
        case GLFW_KEY_LEFT_SUPER:           return ::event::hid::key::left_super;
        case GLFW_KEY_RIGHT_SUPER:          return ::event::hid::key::right_super;

            // Unknown
        default:                            return ::event::hid::key::unknown;
    }
}

auto renderer::opengl::event::receiver::map_modifiers(std::int32_t modifiers) -> ::event::type
{
    std::uint32_t modifier_flags = 0;

    if (modifiers & GLFW_MOD_SHIFT) {
        modifier_flags |= static_cast<std::uint32_t>(::event::type::has_shift_modifier);
    }
    else if (modifiers & GLFW_MOD_ALT) {
        modifier_flags |= static_cast<std::uint32_t>(::event::type::has_alt_modifier);
    }
    else if (modifiers & GLFW_MOD_CAPS_LOCK) {
        modifier_flags |= static_cast<std::uint32_t>(::event::type::has_caps_lock_modifier);
    }
    else if (modifiers & GLFW_MOD_CONTROL) {
        modifier_flags |= static_cast<std::uint32_t>(::event::type::has_control_modifier);
    }
    else if (modifiers & GLFW_MOD_SUPER) {
        modifier_flags |= static_cast<std::uint32_t>(::event::type::has_super_modifier);
    }

    return static_cast<::event::type>(modifier_flags);
}