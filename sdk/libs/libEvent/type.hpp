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

#include <cstdint>

namespace event
{
    enum type : std::uint64_t
    {
        none = 0,

#pragma region Mouse Events
        any_mouse_event = 0xFFF,
        lmb_down = 1 << 0,
        lmb_up = 1 << 1,
        mmb_down = 1 << 2,
        mmb_up = 1 << 3,
        rmb_down = 1 << 4,
        rmb_up = 1 << 5,
        mouse_move = 1 << 6,
        mouse_drag = 1 << 7,
        mouse_scroll = 1 << 8,
        any_mouse_down = lmb_down | mmb_down | rmb_down,
        any_mouse_up = lmb_up | mmb_up | rmb_up,
#pragma endregion

#pragma region Key Events
        any_key_event = 0xF000,
        key_down = 1 << 12,
        key_up = 1 << 13,
        key_repeat = 1 << 14,
        key_typed = 1 << 15,
#pragma endregion

#pragma region Modifiers
        has_caps_lock_modifier = 1 << 16,
        has_shift_modifier = 1 << 17,
        has_control_modifier = 1 << 18,
        has_alt_modifier = 1 << 19,
        has_super_modifier = 1 << 20,
#pragma endregion
    };
}