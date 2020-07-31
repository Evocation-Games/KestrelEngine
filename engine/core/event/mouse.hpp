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

#if !defined(KESTREL_MOUSE_HPP)
#define KESTREL_MOUSE_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "math/point.hpp"

namespace event
{

    struct mouse: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<event::mouse> lua_reference;
        static auto enroll_object_apu_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void;

        enum action : int { released, pressed, moved };
        enum button : int { none = -1, left = 0, right = 2, middle = 3 };

    private:
        math::point m_point;
        mouse::action m_action;
        mouse::button m_button;

    public:
        mouse(const enum mouse::action& action, const enum mouse::button& button);
        mouse(const math::point& point, const enum mouse::action& action, const enum mouse::button& button);
        mouse(const int& x, const int& y, const enum mouse::action& action, const enum mouse::button& button);

        lua_api auto get_point() const -> math::point;
        lua_api auto get_action() const -> int;
        lua_api auto get_button() const -> int;

        lua_api auto get_moved() const -> bool;
        lua_api auto get_released() const -> bool;
        lua_api auto get_pressed() const -> bool;
    };

}


#endif //KESTREL_MOUSE_HPP
