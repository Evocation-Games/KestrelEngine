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

// LuaBridge and Objective-C are not really on speaking terms.
// Make sure that the two don't fight by removing the following
// definition.
#if defined(Nil)
#   undef Nil
#endif

#include "core/environment.hpp"
#include "core/graphics/metal/metal_view.h"
#include "core/graphics/metal/metal_shader.h"
#include "core/graphics/metal/metal_texture.h"
#include "core/graphics/common/scene.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/color.hpp"

// MARK: - Inline Helpers

static inline auto color_vector(const graphics::color& c) -> simd_float4
{
    return vector4(static_cast<float>(c.rgba.components.r / 255.0),
                   static_cast<float>(c.rgba.components.g / 255.0),
                   static_cast<float>(c.rgba.components.b / 255.0),
                   static_cast<float>(c.rgba.components.a / 255.0));
}

// MARK: - Cocoa Interface

@interface MetalView: NSView <MTKViewDelegate>
- (void)drawEntity:(const graphics::entity::lua_reference&)entity;
- (int)registerTexture:(std::shared_ptr<graphics::texture>)texture;
@end


// MARK: - Construction

graphics::metal::view::view()
{
    MetalView *view = [[MetalView alloc] init];
    cocoa::object::set(view);
}

// MARK: - Drawing

auto graphics::metal::view::draw_entity(const graphics::entity::lua_reference& entity) const -> void
{
    [get<MetalView *>() drawEntity:entity];
}

auto graphics::metal::view::register_texture(const std::shared_ptr<graphics::texture> &texture) -> int
{
    return [get<MetalView *>() registerTexture:texture];
}

// MARK: - Cocoa Implementation

@implementation MetalView {
    __strong MTKView *_metalView;
    __strong id<MTLDevice> _device;
    __strong id<MTLCommandQueue> _commandQueue;
    __strong id<MTLRenderCommandEncoder> _commandEncoder;
    __strong NSMutableArray<id<MTLTexture>> *_textures;
    __strong NSMutableArray<id<MTLRenderPipelineState>> *_pipelineStates;
    __strong NSTrackingArea *_trackingArea;
    float _nativeScale;
    vector_uint2 _viewportSize;
}

- (instancetype)init
{
    if (self = [super initWithFrame:CGRectZero]) {
        _device = MTLCreateSystemDefaultDevice();
        _metalView = [[MTKView alloc] initWithFrame:NSZeroRect device:_device];

        // Configure the basic view properties
        [_metalView setColorPixelFormat:MTLPixelFormatBGRA8Unorm];
        [_metalView setClearColor:MTLClearColorMake(0.0, 0.0, 0.0, 1.0)];

        // Configure the Metal Device
        [_metalView setDelegate:self];

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

        // Add the metal view to this one as a subview.
        [_metalView setTranslatesAutoresizingMaskIntoConstraints:NO];
        [self addSubview:_metalView];
        [NSLayoutConstraint activateConstraints:@[
            [_metalView.leftAnchor constraintEqualToAnchor:self.leftAnchor],
            [_metalView.rightAnchor constraintEqualToAnchor:self.rightAnchor],
            [_metalView.topAnchor constraintEqualToAnchor:self.topAnchor],
            [_metalView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor],
        ]];

        // Setup a tracking area for mouse move events.
        NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
                                         NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
        _trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
        [_metalView addTrackingArea:_trackingArea];
    }
    return self;
}

- (BOOL)isFlipped
{
    return YES;
}

// MARK: - Pipelines

- (void)buildPipelineStateForNormalBlendModeUsingLibrary:(id<MTLLibrary>)library
{
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    MTLRenderPipelineDescriptor *pipe = [[MTLRenderPipelineDescriptor alloc] init];
    [pipe setLabel:@"com.kestrel.pipeline.normal"];
    [pipe setVertexFunction:[library newFunctionWithName:@"vertexShader"]];
    [pipe setFragmentFunction:[library newFunctionWithName:@"samplingShader"]];
    [[pipe colorAttachments][0] setPixelFormat:[_metalView colorPixelFormat]];
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
    [pipe setFragmentFunction:[library newFunctionWithName:@"samplingShader"]];
    [[pipe colorAttachments][0] setPixelFormat:[_metalView colorPixelFormat]];
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
    @autoreleasepool {
        id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
        MTLRenderPassDescriptor *pass = [_metalView currentRenderPassDescriptor];
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

            if (auto env = environment::active_environment().lock()) {
                env->window()->tick();
            }

            [_commandEncoder endEncoding];
            _commandEncoder = nil;

            [commandBuffer presentDrawable:[_metalView currentDrawable]];
        }

        [commandBuffer commit];
    }
}

- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;

    // Redo the tracking area.
    [view removeTrackingArea:_trackingArea];

    NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
                                     NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
    _trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    [view addTrackingArea:_trackingArea];
}

// MARK: - Textures

- (int)registerTexture:(std::shared_ptr<graphics::texture>)texture
{
    MTLTextureDescriptor *textureDescriptor = [[[MTLTextureDescriptor alloc] init] autorelease];
    [textureDescriptor setPixelFormat:MTLPixelFormatRGBA8Unorm];
    [textureDescriptor setMipmapLevelCount:1];

    auto size = texture->size();
    [textureDescriptor setWidth:static_cast<NSUInteger>(size.width)];
    [textureDescriptor setHeight:static_cast<NSUInteger>(size.height)];

    id<MTLTexture> mtlTexture = [[_device newTextureWithDescriptor:textureDescriptor] autorelease];

    MTLRegion region = MTLRegionMake2D(0, 0, static_cast<NSUInteger>(size.width), static_cast<NSUInteger>(size.height));
    region.origin.z = 0;
    region.size.depth = 1;

    NSUInteger bytesPerRow = static_cast<NSUInteger>(size.width) << 2;

    if (texture->raw_data_ptr()) {
        [mtlTexture replaceRegion:region mipmapLevel:0 withBytes:texture->raw_data_ptr() bytesPerRow:bytesPerRow];
    }
    else {
        [mtlTexture replaceRegion:region mipmapLevel:0 withBytes:&texture->data()[0] bytesPerRow:bytesPerRow];
    }

    if (!_textures) {
        _textures = [[NSMutableArray alloc] init];
    }
    [_textures addObject:mtlTexture];
    return static_cast<int>([_textures count] - 1);
}

- (id<MTLTexture>)textureUsing:(std::shared_ptr<graphics::texture>)texture
{
    return [_textures objectAtIndex:static_cast<NSInteger>(texture->handle())];
}

// MARK: - Drawing

- (void)drawEntity:(const graphics::entity::lua_reference&)entity
{
    auto texture = entity->texture();
    if (!texture) {
        return;
    }

    // Setup the vertex positions for the entity
    std::array<graphics::metal::vertex_descriptor, 6> v;
    auto x = (static_cast<float>(entity->position.x) * 2) - _viewportSize.x;
    auto y = (static_cast<float>(entity->position.y) * 2) - _viewportSize.y;
    auto w = static_cast<float>(entity->size.width);
    auto h = static_cast<float>(entity->size.height);

    v[0].position = vector2( x -w, y +h );
    v[1].position = vector2( x +w, y +h );
    v[2].position = vector2( x +w, y -h );
    v[3].position = vector2( x -w, y +h );
    v[4].position = vector2( x -w, y -h );
    v[5].position = vector2( x +w, y -h );

    // Fetch the sprite information in order to apply texture coordinates
    auto sprite = entity->spritesheet()->at(entity->sprite_index);
    auto uv_x = static_cast<float>(sprite.point().x);
    auto uv_y = static_cast<float>(sprite.point().y);
    auto uv_w = static_cast<float>(sprite.size().width);
    auto uv_h = static_cast<float>(sprite.size().height);

    v[0].texture_coord = vector2( uv_x, uv_y +uv_h );
    v[1].texture_coord = vector2( uv_x +uv_w, uv_y +uv_h );
    v[2].texture_coord = vector2( uv_x +uv_w, uv_y );
    v[3].texture_coord = vector2( uv_x, uv_y +uv_h );
    v[4].texture_coord = vector2( uv_x, uv_y );
    v[5].texture_coord = vector2( uv_x +uv_w, uv_y );

    // Apply a color to the vertexes of the entity based upon the entity color.
    auto color = color_vector(graphics::color::white_color().with_alpha(entity->get_alpha() * 255.0));
    v[0].color = color;
    v[1].color = color;
    v[2].color = color;
    v[3].color = color;
    v[4].color = color;
    v[5].color = color;

    // Apply the scale of the window/screen.
    auto scale = static_cast<float>([[[self window] screen] backingScaleFactor]);
    v[0].scale = scale;
    v[1].scale = scale;
    v[2].scale = scale;
    v[3].scale = scale;
    v[4].scale = scale;
    v[5].scale = scale;

    // Enqueue a render command for the entity.
    [self setPipelineStateForIndex:entity->get_blend_lua()];

    [_commandEncoder setVertexBytes:&v[0]
                             length:(sizeof(graphics::metal::vertex_descriptor) * v.size())
                            atIndex:graphics::metal::vertex_input_index::vertices];

    [_commandEncoder setVertexBytes:&_viewportSize
                             length:sizeof(_viewportSize)
                            atIndex:graphics::metal::vertex_input_index::viewport_size];

    [_commandEncoder setFragmentTexture:[self textureUsing:texture]
                                atIndex:graphics::metal::texture_index::base_color];

    [_commandEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                        vertexStart:0
                        vertexCount:v.size()];

    [self restoreDefaultPipelineState];
}

// MARK: - Mouse Events

- (void)mouseDown:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        auto p = [self convertPoint:event.locationInWindow fromView:nil];
        env->post_mouse_event(event::mouse(static_cast<int>(p.x), static_cast<int>(p.y), event::mouse::action::pressed, event::mouse::button::left));
    }
}

- (void)rightMouseDown:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        auto p = [self convertPoint:event.locationInWindow fromView:nil];
        env->post_mouse_event(event::mouse(static_cast<int>(p.x), static_cast<int>(p.y), event::mouse::action::pressed, event::mouse::button::right));
    }
}

- (void)mouseUp:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        auto p = [self convertPoint:event.locationInWindow fromView:nil];
        env->post_mouse_event(event::mouse(static_cast<int>(p.x), static_cast<int>(p.y), event::mouse::action::released, event::mouse::button::left));
    }
}

- (void)rightMouseUp:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        auto p = [self convertPoint:event.locationInWindow fromView:nil];
        env->post_mouse_event(event::mouse(static_cast<int>(p.x), static_cast<int>(p.y), event::mouse::action::released, event::mouse::button::right));
    }
}

- (void)mouseMoved:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        auto p = [self convertPoint:event.locationInWindow fromView:nil];
        env->post_mouse_event(event::mouse(static_cast<int>(p.x), static_cast<int>(p.y), event::mouse::action::moved, event::mouse::button::none));
    }
}

@end
