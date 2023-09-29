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

#include <unordered_map>
#include <functional>
#include <libKestrel/util/availability.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>
#include <libKestrel/graphics/renderer/common/shader/program.hpp>

#if TARGET_MACOS
#include <MetalKit/MetalKit.h>
#include <libKestrel/graphics/renderer/metal/context.h>

namespace kestrel::renderer::metal::shader
{
    struct program : public renderer::shader::program
    {
    public:
        program() = default;

        explicit program(metal::context *ctx, const std::string& name, id<MTLFunction> vertex, id<MTLFunction> fragment, MTLPixelFormat format)
        {
            m_pipelines[blending::normal] = get_state(ctx, make_pipeline(name, vertex, fragment, format), [] (MTLRenderPipelineColorAttachmentDescriptor *color) {
                color.rgbBlendOperation = MTLBlendOperationAdd;
                color.alphaBlendOperation = MTLBlendOperationAdd;
                color.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
                color.sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
                color.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
                color.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            });

            m_pipelines[blending::light] = get_state(ctx, make_pipeline(name, vertex, fragment, format), [] (MTLRenderPipelineColorAttachmentDescriptor *color) {
                color.rgbBlendOperation = MTLBlendOperationAdd;
                color.alphaBlendOperation = MTLBlendOperationAdd;
                color.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
                color.sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
                color.destinationRGBBlendFactor = MTLBlendFactorOne;
                color.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            });
        }

        program(const program& program) = default;

        [[nodiscard]] inline auto pipeline(renderer::blending mode) const -> id<MTLRenderPipelineState>
        {
            return m_pipelines.at(mode);
        }

    private:
        static inline auto make_pipeline(const std::string& name, id<MTLFunction> vertex, id<MTLFunction> fragment, MTLPixelFormat format) -> MTLRenderPipelineDescriptor *
        {
            MTLRenderPipelineDescriptor *pipeline = [[MTLRenderPipelineDescriptor alloc] init];
            pipeline.label = [NSString stringWithUTF8String:name.c_str()];
            pipeline.vertexFunction = vertex;
            pipeline.fragmentFunction = fragment;
            pipeline.colorAttachments[0].pixelFormat = format;
            pipeline.colorAttachments[0].blendingEnabled = YES;
            return pipeline;
        }

        static inline auto get_state(metal::context *ctx, MTLRenderPipelineDescriptor *pipeline, const std::function<auto(MTLRenderPipelineColorAttachmentDescriptor *)->void>& config) -> id<MTLRenderPipelineState>
        {
            config(pipeline.colorAttachments[0]);

            NSError *error;
            return [ctx->device() newRenderPipelineStateWithDescriptor:pipeline error:&error];
        }

    private:
        std::unordered_map<renderer::blending, id<MTLRenderPipelineState>> m_pipelines;
    };
}

#endif

