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

#include <libKestrel/graphics/renderer/common/lua_api.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

// MARK: - API

auto kestrel::renderer::lua::api::last_frame_time() -> float
{
    return renderer::last_frame_time();
}

auto kestrel::renderer::lua::api::effective_frame_rate() -> std::uint32_t
{
    return static_cast<std::uint32_t>(1.f / last_frame_time());
}

auto kestrel::renderer::lua::api::time_since_last_frame() -> float
{
    return renderer::time_since_last_frame();
}

auto kestrel::renderer::lua::api::target_framerate() -> std::uint32_t
{
    return renderer::target_framerate();
}

auto kestrel::renderer::lua::api::target_frame_time() -> float
{
    return renderer::target_frame_time();
}

auto kestrel::renderer::lua::api::requires_new_frame() -> bool
{
    return kestrel::renderer::frame_render_required();
}
