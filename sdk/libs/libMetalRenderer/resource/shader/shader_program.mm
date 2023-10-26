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

#include <libMacOS/cocoa/string.h>
#include <libMetalRenderer/resource/shader/shader_program.h>

// MARK: - Construction

renderer::metal::resource::shader::program::program(
    id<MTLDevice> device,
    std::uint64_t uid,
    const std::string &name,
    id<MTLFunction> vertex,
    id<MTLFunction> fragment,
    MTLPixelFormat format
) {
    m_pipelines[pipeline_name::NORMAL] = pipeline_state(device, make_pipeline(name, vertex, fragment, format), [] (auto color) {
        color.rgbBlendOperation = MTLBlendOperationAdd;
        color.alphaBlendOperation = MTLBlendOperationAdd;
        color.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        color.sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        color.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        color.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    });

    m_pipelines[pipeline_name::LIGHT] = pipeline_state(device, make_pipeline(name, vertex, fragment, format), [] (auto color) {
        color.rgbBlendOperation = MTLBlendOperationAdd;
        color.alphaBlendOperation = MTLBlendOperationAdd;
        color.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        color.sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        color.destinationRGBBlendFactor = MTLBlendFactorOne;
        color.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    });
}

// MARK: - Accessors

auto renderer::metal::resource::shader::program::pipeline(renderer::pipeline_name name) const -> id<MTLRenderPipelineState>
{
    return m_pipelines.at(name);
}

// MARK: - Pipeline Management

auto renderer::metal::resource::shader::program::make_pipeline(
    const std::string &name,
    id <MTLFunction> vertex,
    id <MTLFunction> fragment,
    MTLPixelFormat format
) -> MTLRenderPipelineDescriptor * {
    MTLRenderPipelineDescriptor *pipeline = [[MTLRenderPipelineDescriptor alloc] init];
    pipeline.label = macos::cocoa::string::to(name);
    pipeline.vertexFunction = vertex;
    pipeline.fragmentFunction = fragment;
    pipeline.colorAttachments[0].pixelFormat = format;
    pipeline.colorAttachments[0].blendingEnabled = YES;
    return pipeline;
}

auto renderer::metal::resource::shader::program::pipeline_state(
    id<MTLDevice> device,
    MTLRenderPipelineDescriptor *pipeline,
    const std::function<auto(MTLRenderPipelineColorAttachmentDescriptor *)->void> &config
) -> id <MTLRenderPipelineState> {
    NSError *error;
    config(pipeline.colorAttachments[0]);
    id<MTLRenderPipelineState> state = [device newRenderPipelineStateWithDescriptor:pipeline error:&error];
    return state;
}