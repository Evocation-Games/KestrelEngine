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

#include <MetalKit/MetalKit.h>
#include <array>
#include "core/graphics/metal/metal_view.h"
#include "core/graphics/metal/metal_shader.h"

// MARK: - Cocoa Interface

@interface MetalView: MTKView <MTKViewDelegate>

@end


// MARK: - Construction

graphics::metal::view::view()
{
    MetalView *view = [[MetalView alloc] init];
    cocoa::object::set(view);
}

// MARK: - Cocoa Implementation

@implementation MetalView {
    __strong id<MTLDevice> _device;
    __strong id<MTLCommandQueue> _commandQueue;
    __strong id<MTLRenderCommandEncoder> _commandEncoder;
    __strong NSMutableArray<id<MTLTexture>> *_textures;
    __strong NSMutableArray<id<MTLRenderPipelineState>> *_pipelineStates;
    float _nativeScale;
    vector_uint2 _viewportSize;
}

- (instancetype)init
{
    if (self = [super initWithFrame:CGRectZero]) {
        // Configure the basic view properties
        [self setClearColor:MTLClearColorMake(0.0, 0.0, 0.0, 1.0)];

        // Configure the Metal Device
        _device = MTLCreateSystemDefaultDevice();
        [self setDevice:_device];
        [self setDelegate:self];

        // Configure the Metal Library
        // TODO: Attempt to load this from the Resource Manager first, and then fall back on the default one
        //  in the app bundle.
        NSError *error = nil;
        NSString *libraryPath = [[NSBundle mainBundle] pathForResource:@"default" ofType:@"metallib"];
        id<MTLLibrary> library = [_device newLibraryWithFile:libraryPath error:&error];
        if (!library) {
            throw std::runtime_error("Failed to find 'default.metallib' inside the application bundle.");
        }

        // Configure the pipelines
        _pipelineStates = [[NSMutableArray alloc] initWithCapacity:2];
        [self buildPipelineStateForNormalBlendModeUsingLibrary:library];
        [self buildPipelineStateForLightBlendModeUsingLibrary:library];

        // Request a new command queue
        _commandQueue = [_device newCommandQueue];
    }
    return self;
}

// MARK: - Pipelines

- (void)buildPipelineStateForNormalBlendModeUsingLibrary:(id<MTLLibrary>)library
{
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    MTLRenderPipelineDescriptor *pipe = [[MTLRenderPipelineDescriptor alloc] init];
    [pipe setLabel:@"com.kestrel.pipeline.normal"];
    [pipe setVertexFunction:[library newFunctionWithName:@"vertexShader"]];
    [pipe setFragmentFunction:[library newFunctionWithName:@"fragmentShader"]];
    [[pipe colorAttachments][0] setPixelFormat:[self colorPixelFormat]];
    [[pipe colorAttachments][0] setBlendingEnabled:YES];
    [[pipe colorAttachments][0] setRgbBlendOperation:MTLBlendOperationAdd];
    [[pipe colorAttachments][0] setAlphaBlendOperation:MTLBlendOperationAdd];
    [[pipe colorAttachments][0] setSourceRGBBlendFactor:MTLBlendFactorSourceAlpha];
    [[pipe colorAttachments][0] setSourceAlphaBlendFactor:MTLBlendFactorSourceAlpha];
    [[pipe colorAttachments][0] setDestinationRGBBlendFactor:MTLBlendFactorOneMinusSourceAlpha];
    [[pipe colorAttachments][0] setDestinationAlphaBlendFactor:MTLBlendFactorOneMinusSourceAlpha];

    [_device newRenderPipelineStateWithDescriptor:pipe completionHandler:^(id<MTLRenderPipelineState> state, NSError *error) {
        [self->_pipelineStates addObject:state];
        dispatch_group_leave(group);
    }];

    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
    dispatch_release(group);
    [pipe release];
}

- (void)buildPipelineStateForLightBlendModeUsingLibrary:(id<MTLLibrary>)library
{
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    MTLRenderPipelineDescriptor *pipe = [[MTLRenderPipelineDescriptor alloc] init];
    [pipe setLabel:@"com.kestrel.pipeline.light"];
    [pipe setVertexFunction:[library newFunctionWithName:@"vertexShader"]];
    [pipe setFragmentFunction:[library newFunctionWithName:@"fragmentShader"]];
    [[pipe colorAttachments][0] setPixelFormat:[self colorPixelFormat]];
    [[pipe colorAttachments][0] setBlendingEnabled:YES];
    [[pipe colorAttachments][0] setRgbBlendOperation:MTLBlendOperationAdd];
    [[pipe colorAttachments][0] setAlphaBlendOperation:MTLBlendOperationAdd];
    [[pipe colorAttachments][0] setSourceRGBBlendFactor:MTLBlendFactorSourceAlpha];
    [[pipe colorAttachments][0] setSourceAlphaBlendFactor:MTLBlendFactorSourceAlpha];
    [[pipe colorAttachments][0] setDestinationRGBBlendFactor:MTLBlendFactorOne];
    [[pipe colorAttachments][0] setDestinationAlphaBlendFactor:MTLBlendFactorOneMinusSourceAlpha];

    [_device newRenderPipelineStateWithDescriptor:pipe completionHandler:^(id<MTLRenderPipelineState> state, NSError *error) {
        [self->_pipelineStates addObject:state];
        dispatch_group_leave(group);
    }];

    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
    dispatch_release(group);
    [pipe release];
}

- (void)restoreDefaultPipelineState
{
    [_commandEncoder setRenderPipelineState:_pipelineStates[0]];
}

- (void)setPipelineStateForIndex:(int)index
{
    [_commandEncoder setRenderPipelineState:_pipelineStates[index]];
}

// MARK: - Delegation

- (void)drawInMTKView:(MTKView *)view
{
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];

    MTLRenderPassDescriptor *pass = [self currentRenderPassDescriptor];
    if (pass) {
        // Configure the viewport.
        // TODO: Pass this out the metal::session_window in order to be in line with the OpenGL implementation
        MTLViewport viewport;
        viewport.originX = 0.0;
        viewport.originY = 0.0;
        viewport.width = _viewportSize.x;
        viewport.height = _viewportSize.y;
        viewport.znear = 1.0;
        viewport.zfar = -1.0;

        _commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:pass];
        [_commandEncoder setLabel:@"com.kestrel.render-encoder"];
        [_commandEncoder setViewport:viewport];
        [_commandEncoder setRenderPipelineState:_pipelineStates[0]];

        // TODO: Call the rendering function here...

        [_commandEncoder endEncoding];
        _commandEncoder = nil;

        [commandBuffer presentDrawable:[self currentDrawable]];
    }

    [commandBuffer commit];
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

@end