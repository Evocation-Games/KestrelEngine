// Copyright (c) 2022 Tom Hancocks
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

#include <libKestrel/util/availability.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::platform
{
    enum class lua_api(Platform.Family, Available_0_8) family
    {
        unknown lua_case(Unknown, Available_0_8),
        macos lua_case(MacOS, Available_0_8),
        linux lua_case(Linux, Available_0_8),
        windows lua_case(Windows, Available_0_8)
    };

    static auto current() -> enum family
    {
#if TARGET_WINDOWS
        return family::windows;
#elif TARGET_LINUX
        return family::linux;
#elif TARGET_MACOS
        return family::macos;
#else
        return platform::unknown;
#endif
    };
}