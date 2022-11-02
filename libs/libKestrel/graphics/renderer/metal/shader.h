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
#include <libKestrel/graphics/renderer/common/shader.hpp>

#if TARGET_MACOS
#include <MetalKit/MetalKit.h>

namespace kestrel::renderer::metal::shader
{
    struct program : public renderer::shader::program
    {
    public:
        program() = default;
        explicit program(id<MTLRenderPipelineState> state) : m_state(state) {};
        program(const program& program) = default;

        [[nodiscard]] inline auto get_state() const -> id<MTLRenderPipelineState> { return m_state; }

    private:
        id<MTLRenderPipelineState> m_state;
    };
}

#endif

