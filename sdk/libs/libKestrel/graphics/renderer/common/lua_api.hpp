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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::renderer::lua
{
    namespace lua_api(Renderer, Available_0_8) api
    {
        has_lua_api;

        lua_getter(lastFrameTime, Available_0_8) auto last_frame_time() -> float;
        lua_getter(effectiveFrameRate, Available_0_8) auto effective_frame_rate() -> std::uint32_t;
        lua_getter(timeSinceLastFrame, Available_0_8) auto time_since_last_frame() -> float;
        lua_getter(targetFrameRate, Available_0_8) auto target_framerate() -> std::uint32_t;
        lua_getter(targetFrameTime, Available_0_8) auto target_frame_time() -> float;
        lua_getter(requiresNewFrame, Available_0_8) auto requires_new_frame() -> bool;
    }
}