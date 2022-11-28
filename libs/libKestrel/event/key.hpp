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

namespace kestrel::hid
{
    enum lua_api(Key, Available_0_8) key : std::uint16_t
    {
        unknown lua_case(Null, Available_0_8) = 0,
        space lua_case(Space, Available_0_8) = 32,
        apostrophe lua_case(Apostrophe, Available_0_8) = 39,
        comma lua_case(Comma, Available_0_8) = 44,
        minus lua_case(Minus, Available_0_8) = 45,
        period lua_case(Period, Available_0_8) = 46,
        slash lua_case(Slash, Available_0_8) = 47,
        num0 = 48,
        num1 = 49,
        num2 = 50,
        num3 = 51,
        num4 = 52,
        num5 = 53,
        num6 = 54,
        num7 = 55,
        num8 = 56,
        num9 = 57,
        semi_colon lua_case(Semicolon, Available_0_8) = 59,
        equal lua_case(Equal, Available_0_8) = 61,
        a lua_case(A, Available_0_8) = 65,
        b lua_case(B, Available_0_8) = 66,
        c lua_case(C, Available_0_8) = 67,
        d lua_case(D, Available_0_8) = 68,
        e lua_case(E, Available_0_8) = 69,
        f lua_case(F, Available_0_8) = 70,
        g lua_case(G, Available_0_8) = 71,
        h lua_case(H, Available_0_8) = 72,
        i lua_case(I, Available_0_8) = 73,
        j lua_case(J, Available_0_8) = 74,
        k lua_case(K, Available_0_8) = 75,
        l lua_case(L, Available_0_8) = 76,
        m lua_case(M, Available_0_8) = 77,
        n lua_case(N, Available_0_8) = 78,
        o lua_case(O, Available_0_8) = 79,
        p lua_case(P, Available_0_8) = 80,
        q lua_case(Q, Available_0_8) = 81,
        r lua_case(R, Available_0_8) = 82,
        s lua_case(S, Available_0_8) = 83,
        t lua_case(T, Available_0_8) = 84,
        u lua_case(U, Available_0_8) = 85,
        v lua_case(V, Available_0_8) = 86,
        w lua_case(W, Available_0_8) = 87,
        x lua_case(X, Available_0_8) = 88,
        y lua_case(Y, Available_0_8) = 89,
        z lua_case(Z, Available_0_8) = 90,
        left_bracket lua_case(LeftBracket, Available_0_8) = 91,
        backslash lua_case(BackSlash, Available_0_8) = 92,
        right_bracket lua_case(RightBracket, Available_0_8) = 93,
        grave_accent lua_case(GraveAccent, Available_0_8) = 96,
        world_1 = 161,
        world_2 = 162,
        escape lua_case(Escape, Available_0_8) = 256,
        enter lua_case(Enter, Available_0_8) = 257,
        tab lua_case(Tab, Available_0_8) = 258,
        backspace lua_case(Backspace, Available_0_8) = 259,
        insert lua_case(Insert, Available_0_8) = 260,
        del lua_case(Delete, Available_0_8) = 261,
        right lua_case(Right, Available_0_8) = 262,
        left lua_case(Left, Available_0_8) = 263,
        down lua_case(Down, Available_0_8) = 264,
        up lua_case(Up, Available_0_8) = 265,
        page_up lua_case(PageUp, Available_0_8) = 266,
        page_down lua_case(PageDown, Available_0_8) = 267,
        home lua_case(Home, Available_0_8) = 268,
        end = 269,
        caps_lock = 280,
        scroll_lock = 281,
        num_lock = 282,
        print_screen = 283,
        pause = 284,
        f1 lua_case(F1, Available_0_8) = 290,
        f2 lua_case(F2, Available_0_8) = 291,
        f3 lua_case(F3, Available_0_8) = 292,
        f4 lua_case(F4, Available_0_8) = 293,
        f5 lua_case(F5, Available_0_8) = 294,
        f6 lua_case(F6, Available_0_8) = 295,
        f7 lua_case(F7, Available_0_8) = 296,
        f8 lua_case(F8, Available_0_8) = 297,
        f9 lua_case(F9, Available_0_8) = 298,
        f10 lua_case(F10, Available_0_8) = 299,
        f11 lua_case(F11, Available_0_8) = 300,
        f12 lua_case(F12, Available_0_8) = 301,
        f13 lua_case(F13, Available_0_8) = 302,
        f14 lua_case(F14, Available_0_8) = 303,
        f15 lua_case(F15, Available_0_8) = 304,
        f16 lua_case(F16, Available_0_8) = 305,
        f17 lua_case(F17, Available_0_8) = 306,
        f18 lua_case(F18, Available_0_8) = 307,
        f19 lua_case(F19, Available_0_8) = 308,
        f20 lua_case(F20, Available_0_8) = 309,
        f21 lua_case(F21, Available_0_8) = 310,
        f22 lua_case(F22, Available_0_8) = 311,
        f23 lua_case(F23, Available_0_8) = 312,
        f24 lua_case(F24, Available_0_8) = 313,
        f25 lua_case(F25, Available_0_8) = 314,
        kp_0 lua_case(N0, Available_0_8) = 320,
        kp_1 lua_case(N1, Available_0_8) = 321,
        kp_2 lua_case(N2, Available_0_8) = 322,
        kp_3 lua_case(N3, Available_0_8) = 323,
        kp_4 lua_case(N4, Available_0_8) = 324,
        kp_5 lua_case(N5, Available_0_8) = 325,
        kp_6 lua_case(N6, Available_0_8) = 326,
        kp_7 lua_case(N7, Available_0_8) = 327,
        kp_8 lua_case(N8, Available_0_8) = 328,
        kp_9 lua_case(N9, Available_0_8) = 329,
        kp_decimal = 330,
        kp_divide = 331,
        kp_multiply = 332,
        kp_subtract = 333,
        kp_add = 334,
        kp_enter = 335,
        kp_equal = 336,
        left_shift lua_case(LeftShift, Available_0_8) = 340,
        left_control lua_case(LeftControl, Available_0_8) = 341,
        left_alt lua_case(LeftAlt, Available_0_8) = 342,
        left_super lua_case(LeftSuper, Available_0_8) = 343,
        right_shift = 344,
        right_control = 345,
        right_alt = 346,
        right_super = 347,
        menu = 348
    };
}
