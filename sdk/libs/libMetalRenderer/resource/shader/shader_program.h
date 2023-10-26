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

#include <string>
#include <unordered_map>
#include <functional>
#include <MetalKit/MetalKit.h>
#include <libRenderCore/pipeline/pipeline_name.hpp>

namespace renderer::metal::resource::shader
{
    struct program
    {
        program() = default;
        program(id<MTLDevice> device,
                std::uint64_t uid,
                const std::string& name,
                id<MTLFunction> vertex,
                id<MTLFunction> fragment,
                MTLPixelFormat format);

        [[nodiscard]] inline auto is_valid() const -> bool { return !m_pipelines.empty(); }
        [[nodiscard]] inline auto uid() const -> std::uint64_t { return m_uid; }
        [[nodiscard]] inline auto name() const -> std::string { return m_name; }
        [[nodiscard]] auto pipeline(pipeline_name name = pipeline_name::NORMAL) const -> id<MTLRenderPipelineState>;

    private:
        static auto make_pipeline(const std::string& name, id<MTLFunction> vertex, id<MTLFunction> fragment, MTLPixelFormat format) -> MTLRenderPipelineDescriptor *;
        static auto pipeline_state(id<MTLDevice> device, MTLRenderPipelineDescriptor *pipeline, const std::function<auto(MTLRenderPipelineColorAttachmentDescriptor *)->void>& config) -> id<MTLRenderPipelineState>;

    private:
        std::uint64_t m_uid { 0 };
        std::string m_name;
        std::unordered_map<pipeline_name, id<MTLRenderPipelineState>> m_pipelines;
    };
}