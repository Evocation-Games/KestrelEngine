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

@interface MetalView: MTKView

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
    vector_uint2 *_viewportSize;
}

- (instancetype)init
{
    if (self = [super initWithFrame:CGRectZero]) {
        // Configure the basic view properties
        [self setClearColor:MTLClearColorMake(0.0, 0.0, 0.0, 1.0)];

        // Configure the Metal Device
        _device = MTLCreateSystemDefaultDevice();
        [self setDevice:_device];

        // Configure the Metal Library
    }
    return self;
}

@end