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

#pragma once

#include <cstdint>
#include <libKestrel/lua/scripting.hpp>
#include <libUI/hid/key.hpp>

namespace kestrel::hid
{
    enum lua_api(Key, Available_0_8) key : std::uint16_t
    {
        unknown lua_case(Null, Available_0_8) = ::ui::hid::key::unknown,
        space lua_case(Space, Available_0_8) = ::ui::hid::key::space,
        apostrophe lua_case(Apostrophe, Available_0_8) = ::ui::hid::key::apostrophe,
        comma lua_case(Comma, Available_0_8) = ::ui::hid::key::comma,
        minus lua_case(Minus, Available_0_8) = ::ui::hid::key::minus,
        period lua_case(Period, Available_0_8) = ::ui::hid::key::period,
        slash lua_case(Slash, Available_0_8) = ::ui::hid::key::slash,
        num0 = ::ui::hid::key::num0,
        num1 = ::ui::hid::key::num1,
        num2 = ::ui::hid::key::num2,
        num3 = ::ui::hid::key::num3,
        num4 = ::ui::hid::key::num4,
        num5 = ::ui::hid::key::num5,
        num6 = ::ui::hid::key::num6,
        num7 = ::ui::hid::key::num7,
        num8 = ::ui::hid::key::num8,
        num9 = ::ui::hid::key::num9,
        semi_colon lua_case(Semicolon, Available_0_8) = ::ui::hid::key::semi_colon,
        equal lua_case(Equal, Available_0_8) = ::ui::hid::key::equal,
        a lua_case(A, Available_0_8) = ::ui::hid::key::a,
        b lua_case(B, Available_0_8) = ::ui::hid::key::b,
        c lua_case(C, Available_0_8) = ::ui::hid::key::c,
        d lua_case(D, Available_0_8) = ::ui::hid::key::d,
        e lua_case(E, Available_0_8) = ::ui::hid::key::e,
        f lua_case(F, Available_0_8) = ::ui::hid::key::f,
        g lua_case(G, Available_0_8) = ::ui::hid::key::g,
        h lua_case(H, Available_0_8) = ::ui::hid::key::h,
        i lua_case(I, Available_0_8) = ::ui::hid::key::i,
        j lua_case(J, Available_0_8) = ::ui::hid::key::j,
        k lua_case(K, Available_0_8) = ::ui::hid::key::k,
        l lua_case(L, Available_0_8) = ::ui::hid::key::l,
        m lua_case(M, Available_0_8) = ::ui::hid::key::m,
        n lua_case(N, Available_0_8) = ::ui::hid::key::n,
        o lua_case(O, Available_0_8) = ::ui::hid::key::o,
        p lua_case(P, Available_0_8) = ::ui::hid::key::p,
        q lua_case(Q, Available_0_8) = ::ui::hid::key::q,
        r lua_case(R, Available_0_8) = ::ui::hid::key::r,
        s lua_case(S, Available_0_8) = ::ui::hid::key::s,
        t lua_case(T, Available_0_8) = ::ui::hid::key::t,
        u lua_case(U, Available_0_8) = ::ui::hid::key::u,
        v lua_case(V, Available_0_8) = ::ui::hid::key::v,
        w lua_case(W, Available_0_8) = ::ui::hid::key::w,
        x lua_case(X, Available_0_8) = ::ui::hid::key::x,
        y lua_case(Y, Available_0_8) = ::ui::hid::key::y,
        z lua_case(Z, Available_0_8) = ::ui::hid::key::z,
        left_bracket lua_case(LeftBracket, Available_0_8) = ::ui::hid::key::left_bracket,
        backslash lua_case(BackSlash, Available_0_8) = ::ui::hid::key::backslash,
        right_bracket lua_case(RightBracket, Available_0_8) = ::ui::hid::key::right_bracket,
        grave_accent lua_case(GraveAccent, Available_0_8) = ::ui::hid::key::grave_accent,
        world_1 = ::ui::hid::key::world_1,
        world_2 = ::ui::hid::key::world_2,
        escape lua_case(Escape, Available_0_8) = ::ui::hid::key::escape,
        enter lua_case(Enter, Available_0_8) = ::ui::hid::key::enter,
        tab lua_case(Tab, Available_0_8) = ::ui::hid::key::tab,
        backspace lua_case(Backspace, Available_0_8) = ::ui::hid::key::backspace,
        insert lua_case(Insert, Available_0_8) = ::ui::hid::key::insert,
        del lua_case(Delete, Available_0_8) = ::ui::hid::key::del,
        right lua_case(Right, Available_0_8) = ::ui::hid::key::right,
        left lua_case(Left, Available_0_8) = ::ui::hid::key::left,
        down lua_case(Down, Available_0_8) = ::ui::hid::key::down,
        up lua_case(Up, Available_0_8) = ::ui::hid::key::up,
        page_up lua_case(PageUp, Available_0_8) = ::ui::hid::key::page_up,
        page_down lua_case(PageDown, Available_0_8) = ::ui::hid::key::page_down,
        home lua_case(Home, Available_0_8) = ::ui::hid::key::home,
        end = ::ui::hid::key::end,
        caps_lock = ::ui::hid::key::caps_lock,
        scroll_lock = ::ui::hid::key::scroll_lock,
        num_lock = ::ui::hid::key::num_lock,
        print_screen = ::ui::hid::key::print_screen,
        pause = ::ui::hid::key::pause,
        f1 lua_case(F1, Available_0_8) = ::ui::hid::key::f1,
        f2 lua_case(F2, Available_0_8) = ::ui::hid::key::f2,
        f3 lua_case(F3, Available_0_8) = ::ui::hid::key::f3,
        f4 lua_case(F4, Available_0_8) = ::ui::hid::key::f4,
        f5 lua_case(F5, Available_0_8) = ::ui::hid::key::f5,
        f6 lua_case(F6, Available_0_8) = ::ui::hid::key::f6,
        f7 lua_case(F7, Available_0_8) = ::ui::hid::key::f7,
        f8 lua_case(F8, Available_0_8) = ::ui::hid::key::f8,
        f9 lua_case(F9, Available_0_8) = ::ui::hid::key::f9,
        f10 lua_case(F10, Available_0_8) = ::ui::hid::key::f10,
        f11 lua_case(F11, Available_0_8) = ::ui::hid::key::f11,
        f12 lua_case(F12, Available_0_8) = ::ui::hid::key::f12,
        f13 lua_case(F13, Available_0_8) = ::ui::hid::key::f13,
        f14 lua_case(F14, Available_0_8) = ::ui::hid::key::f14,
        f15 lua_case(F15, Available_0_8) = ::ui::hid::key::f15,
        f16 lua_case(F16, Available_0_8) = ::ui::hid::key::f16,
        f17 lua_case(F17, Available_0_8) = ::ui::hid::key::f17,
        f18 lua_case(F18, Available_0_8) = ::ui::hid::key::f18,
        f19 lua_case(F19, Available_0_8) = ::ui::hid::key::f19,
        f20 lua_case(F20, Available_0_8) = ::ui::hid::key::f20,
        f21 lua_case(F21, Available_0_8) = ::ui::hid::key::f21,
        f22 lua_case(F22, Available_0_8) = ::ui::hid::key::f22,
        f23 lua_case(F23, Available_0_8) = ::ui::hid::key::f23,
        f24 lua_case(F24, Available_0_8) = ::ui::hid::key::f24,
        f25 lua_case(F25, Available_0_8) = ::ui::hid::key::f25,
        kp_0 lua_case(N0, Available_0_8) = ::ui::hid::key::kp_0,
        kp_1 lua_case(N1, Available_0_8) = ::ui::hid::key::kp_1,
        kp_2 lua_case(N2, Available_0_8) = ::ui::hid::key::kp_2,
        kp_3 lua_case(N3, Available_0_8) = ::ui::hid::key::kp_3,
        kp_4 lua_case(N4, Available_0_8) = ::ui::hid::key::kp_4,
        kp_5 lua_case(N5, Available_0_8) = ::ui::hid::key::kp_5,
        kp_6 lua_case(N6, Available_0_8) = ::ui::hid::key::kp_6,
        kp_7 lua_case(N7, Available_0_8) = ::ui::hid::key::kp_7,
        kp_8 lua_case(N8, Available_0_8) = ::ui::hid::key::kp_8,
        kp_9 lua_case(N9, Available_0_8) = ::ui::hid::key::kp_9,
        kp_decimal = ::ui::hid::key::kp_decimal,
        kp_divide = ::ui::hid::key::kp_divide,
        kp_multiply = ::ui::hid::key::kp_multiply,
        kp_subtract = ::ui::hid::key::kp_subtract,
        kp_add = ::ui::hid::key::kp_add,
        kp_enter = ::ui::hid::key::kp_enter,
        kp_equal = ::ui::hid::key::kp_equal,
        left_shift lua_case(LeftShift, Available_0_8) = ::ui::hid::key::left_shift,
        left_control lua_case(LeftControl, Available_0_8) = ::ui::hid::key::left_control,
        left_alt lua_case(LeftAlt, Available_0_8) = ::ui::hid::key::left_alt,
        left_super lua_case(LeftSuper, Available_0_8) = ::ui::hid::key::left_super,
        right_shift = ::ui::hid::key::right_shift,
        right_control = ::ui::hid::key::right_control,
        right_alt = ::ui::hid::key::right_alt,
        right_super = ::ui::hid::key::right_super,
        menu = ::ui::hid::key::menu,
    };
}
