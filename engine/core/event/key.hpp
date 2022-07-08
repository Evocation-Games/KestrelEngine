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

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace hid
{
    enum key : uint16_t
    {
        unknown = 0,
        space = 32,
        apostrophe = 39,
        comma = 44,
        minus = 45,
        period = 46,
        slash = 47,
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
        semi_colon = 59,
        equal = 61,
        a = 65,
        b = 66,
        c = 67,
        d = 68,
        e = 69,
        f = 70,
        g = 71,
        h = 72,
        i = 73,
        j = 74,
        k = 75,
        l = 76,
        m = 77,
        n = 78,
        o = 79,
        p = 80,
        q = 81,
        r = 82,
        s = 83,
        t = 84,
        u = 85,
        v = 86,
        w = 87,
        x = 88,
        y = 89,
        z = 90,
        left_bracket = 91,
        backslash = 92,
        right_bracket = 93,
        grave_accent = 96,
        world_1 = 161,
        world_2 = 162,
        escape = 256,
        enter = 257,
        tab = 258,
        backspace = 259,
        insert = 260,
        del = 261,
        right = 262,
        left = 263,
        down = 264,
        up = 265,
        page_up = 266,
        page_down = 267,
        home = 268,
        end = 269,
        caps_lock = 280,
        scroll_lock = 281,
        num_lock = 282,
        print_screen = 283,
        pause = 284,
        f1 = 290,
        f2 = 291,
        f3 = 292,
        f4 = 293,
        f5 = 294,
        f6 = 295,
        f7 = 296,
        f8 = 297,
        f9 = 298,
        f10 = 299,
        f11 = 300,
        f12 = 301,
        f13 = 302,
        f14 = 303,
        f15 = 304,
        f16 = 305,
        f17 = 306,
        f18 = 307,
        f19 = 308,
        f20 = 309,
        f21 = 310,
        f22 = 311,
        f23 = 312,
        f24 = 313,
        f25 = 314,
        kp_0 = 320,
        kp_1 = 321,
        kp_2 = 322,
        kp_3 = 323,
        kp_4 = 324,
        kp_5 = 325,
        kp_6 = 326,
        kp_7 = 327,
        kp_8 = 328,
        kp_9 = 329,
        kp_decimal = 330,
        kp_divide = 331,
        kp_multiply = 332,
        kp_subtract = 333,
        kp_add = 334,
        kp_enter = 335,
        kp_equal = 336,
        left_shift = 340,
        left_control = 341,
        left_alt = 342,
        left_super = 343,
        right_shift = 344,
        right_control = 345,
        right_alt = 346,
        right_super = 347,
        menu = 348
    };
}
