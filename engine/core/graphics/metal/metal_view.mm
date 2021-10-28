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

// Warning: Dependance on Carbon here for Key Codes, due to Cocoa not having them?
#include <Carbon/Carbon.h>

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

@interface WrappedMTKView : MTKView

@end

@implementation WrappedMTKView

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (void)keyDown:(NSEvent *)event
{
    [[self superview] keyDown:event];
}

- (void)keyUp:(NSEvent *)event
{
    [[self superview] keyUp:event];
}

- (void)flagsChanged:(NSEvent *)event
{
    [[self superview] flagsChanged:event];
}

@end

@interface MetalView: NSView <MTKViewDelegate>
- (instancetype)initWithScale:(double)scale;
- (void)drawEntity:(const graphics::entity::lua_reference&)entity;
- (int)registerTexture:(std::shared_ptr<graphics::texture>)texture;
- (void)destroyTexture:(const int)handle;
@end


// MARK: - Construction

graphics::metal::view::view(const double& scale)
{
    MetalView *view = [[MetalView alloc] initWithScale:scale];
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

auto graphics::metal::view::destroy_texture(const int &handle) -> void
{
    [get<MetalView *>() destroyTexture:handle];
}

// MARK: - Cocoa Implementation

#define kMaxFramesInFlight 3

@implementation MetalView {
    __strong WrappedMTKView *_metalView;
    __strong id<MTLDevice> _device;
    __strong id<MTLCommandQueue> _commandQueue;
    __strong id<MTLRenderCommandEncoder> _commandEncoder;
    __strong NSMutableArray *_textures;
    __strong NSMutableArray<id<MTLRenderPipelineState>> *_pipelineStates;
    __strong NSTrackingArea *_trackingArea;
    float _scale;
    vector_uint2 _viewportSize;
    NSEventModifierFlags _activeModifierFlags;
    dispatch_semaphore_t _renderSemaphore;
    NSUInteger _currentBufferIndex;
}

- (instancetype)initWithScale:(double)scale
{
    if (self = [super initWithFrame:CGRectZero]) {
        _scale = static_cast<float>(scale);
        _device = MTLCreateSystemDefaultDevice();
        _metalView = [[WrappedMTKView alloc] initWithFrame:NSZeroRect device:_device];

        _renderSemaphore = dispatch_semaphore_create(kMaxFramesInFlight);

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
        id <MTLLibrary> library = [_device newLibraryWithFile:libraryPath error:&error];
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

- (void)renderWithPassDescriptor:(MTLRenderPassDescriptor *)pass
{
    dispatch_semaphore_wait(_renderSemaphore, DISPATCH_TIME_FOREVER);
    _currentBufferIndex = (_currentBufferIndex + 1) % kMaxFramesInFlight;

    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];

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

    [commandBuffer addCompletedHandler:^(id <MTLCommandBuffer> buffer) {
        dispatch_semaphore_signal(_renderSemaphore);
    }];

    [commandBuffer commit];
}

- (void)drawInMTKView:(MTKView *)view
{
    @autoreleasepool {
        MTLRenderPassDescriptor *pass = [_metalView currentRenderPassDescriptor];
        if (pass) {
            [self renderWithPassDescriptor:pass];
        }
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

    id<MTLTexture> mtlTexture = [_device newTextureWithDescriptor:textureDescriptor];

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
    [mtlTexture release];
    return static_cast<int>([_textures count] - 1);
}

- (void)destroyTexture:(const int)handle
{
    [_textures replaceObjectAtIndex:handle withObject:[NSNull null]];
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
    std::array<graphics::metal::vertex_descriptor, 6> v{};
    auto x = static_cast<float>(entity->position.x);
    auto y = static_cast<float>(entity->position.y);
    auto w = static_cast<float>(entity->render_size.width);
    auto h = static_cast<float>(entity->render_size.height);

    v[0].position = vector2( x, y +h );
    v[1].position = vector2( x +w, y +h );
    v[2].position = vector2( x +w, y );
    v[3].position = vector2( x, y +h );
    v[4].position = vector2( x, y );
    v[5].position = vector2( x +w, y );

    // Fetch the sprite information in order to apply texture coordinates
    auto sprite = entity->spritesheet()->at(entity->sprite_index);
    auto uv_x = static_cast<float>(sprite.point().x);
    auto uv_y = static_cast<float>(sprite.point().y);
    auto uv_w = static_cast<float>(sprite.size().width);
    auto uv_h = static_cast<float>(sprite.size().height);

    if (entity->has_clipping_area()) {
        uv_x = static_cast<float>(sprite.point().x + entity->clipping_offset_uv().x);
        uv_y = static_cast<float>(sprite.point().y + entity->clipping_offset_uv().y);
        uv_w = static_cast<float>(entity->clipping_area_uv().width);
        uv_h = static_cast<float>(entity->clipping_area_uv().height);
    }

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
    v[0].scale = _scale;
    v[1].scale = _scale;
    v[2].scale = _scale;
    v[3].scale = _scale;
    v[4].scale = _scale;
    v[5].scale = _scale;

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

- (void)mouseDragged:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        auto p = [self convertPoint:event.locationInWindow fromView:nil];
        env->post_mouse_event(event::mouse(static_cast<int>(p.x), static_cast<int>(p.y), event::mouse::action::moved, event::mouse::button::left));
    }
}

// MARK: - Key Events

- (void)viewDidMoveToWindow
{
    [[self window] makeFirstResponder:_metalView];
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)keyEvent:(NSEvent *)event down:(BOOL)down
{
    if (auto env = environment::active_environment().lock()) {
        env->post_key_event(event::key([self translateKeycodeForEvent:event], 0, down ? event::key::pressed : event::key::released));
    }
}

- (enum event::key::code)translateKeycodeForEvent:(NSEvent *)event
{
    switch ([event keyCode]) {
        // Special Keys
        case kVK_ANSI_KeypadEnter:
        case kVK_Return:                return event::key::code::enter;
        case kVK_Tab:                   return event::key::code::tab;
        case kVK_Delete:                return event::key::code::backspace;
        case kVK_Escape:                return event::key::code::escape;

        // Cursor Keys
        case kVK_LeftArrow:             return event::key::code::left;
        case kVK_RightArrow:            return event::key::code::right;
        case kVK_DownArrow:             return event::key::code::down;
        case kVK_UpArrow:               return event::key::code::up;

        // Letters
        case kVK_ANSI_A:                return event::key::code::a;
        case kVK_ANSI_S:                return event::key::code::s;
        case kVK_ANSI_D:                return event::key::code::d;
        case kVK_ANSI_F:                return event::key::code::f;
        case kVK_ANSI_H:                return event::key::code::h;
        case kVK_ANSI_G:                return event::key::code::g;
        case kVK_ANSI_Z:                return event::key::code::z;
        case kVK_ANSI_X:                return event::key::code::x;
        case kVK_ANSI_C:                return event::key::code::c;
        case kVK_ANSI_V:                return event::key::code::v;
        case kVK_ANSI_B:                return event::key::code::b;
        case kVK_ANSI_Q:                return event::key::code::q;
        case kVK_ANSI_W:                return event::key::code::w;
        case kVK_ANSI_E:                return event::key::code::e;
        case kVK_ANSI_R:                return event::key::code::r;
        case kVK_ANSI_Y:                return event::key::code::y;
        case kVK_ANSI_T:                return event::key::code::t;
        case kVK_ANSI_O:                return event::key::code::o;
        case kVK_ANSI_U:                return event::key::code::u;
        case kVK_ANSI_I:                return event::key::code::i;
        case kVK_ANSI_P:                return event::key::code::p;
        case kVK_ANSI_L:                return event::key::code::l;
        case kVK_ANSI_J:                return event::key::code::j;
        case kVK_ANSI_K:                return event::key::code::k;
        case kVK_ANSI_N:                return event::key::code::n;
        case kVK_ANSI_M:                return event::key::code::m;

        // Numbers
        case kVK_ANSI_Keypad0:
        case kVK_ANSI_0:                return event::key::code::kp_0;
        case kVK_ANSI_Keypad1:
        case kVK_ANSI_1:                return event::key::code::kp_1;
        case kVK_ANSI_Keypad2:
        case kVK_ANSI_2:                return event::key::code::kp_2;
        case kVK_ANSI_Keypad3:
        case kVK_ANSI_3:                return event::key::code::kp_3;
        case kVK_ANSI_Keypad4:
        case kVK_ANSI_4:                return event::key::code::kp_4;
        case kVK_ANSI_Keypad5:
        case kVK_ANSI_5:                return event::key::code::kp_5;
        case kVK_ANSI_Keypad6:
        case kVK_ANSI_6:                return event::key::code::kp_6;
        case kVK_ANSI_Keypad7:
        case kVK_ANSI_7:                return event::key::code::kp_7;
        case kVK_ANSI_Keypad8:
        case kVK_ANSI_8:                return event::key::code::kp_8;
        case kVK_ANSI_Keypad9:
        case kVK_ANSI_9:                return event::key::code::kp_9;

        // Special
        case kVK_Space:                 return event::key::code::space;

        // Unknown
        default:                        return event::key::code::unknown;
    }
}

- (void)keyDown:(NSEvent *)event
{
    [self keyEvent:event down:YES];
}

- (void)keyUp:(NSEvent *)event
{
    [self keyEvent:event down:NO];
}

- (void)flagsChanged:(NSEvent *)event
{
    if (auto env = environment::active_environment().lock()) {
        if ([event modifierFlags] & NSEventModifierFlagCapsLock) {
            env->post_key_event(event::key(event::key::caps_lock, 0, event::key::pressed));
        }
        else if ([event modifierFlags] | NSEventModifierFlagCapsLock) {
            env->post_key_event(event::key(event::key::caps_lock, 0, event::key::released));
        }

        if ([event modifierFlags] & NSEventModifierFlagShift) {
            env->post_key_event(event::key(event::key::left_shift, 0, event::key::pressed));
        }
        else if ([event modifierFlags] | NSEventModifierFlagShift) {
            env->post_key_event(event::key(event::key::left_shift, 0, event::key::released));
        }

        if ([event modifierFlags] & NSEventModifierFlagControl) {
            env->post_key_event(event::key(event::key::left_control, 0, event::key::pressed));
        }
        else if ([event modifierFlags] | NSEventModifierFlagControl) {
            env->post_key_event(event::key(event::key::left_control, 0, event::key::released));
        }

        if ([event modifierFlags] & NSEventModifierFlagOption) {
            env->post_key_event(event::key(event::key::left_alt, 0, event::key::pressed));
        }
        else if ([event modifierFlags] | NSEventModifierFlagOption) {
            env->post_key_event(event::key(event::key::left_alt, 0, event::key::released));
        }

        if ([event modifierFlags] & NSEventModifierFlagCommand) {
            env->post_key_event(event::key(event::key::left_super, 0, event::key::pressed));
        }
        else if ([event modifierFlags] | NSEventModifierFlagCommand) {
            env->post_key_event(event::key(event::key::left_super, 0, event::key::released));
        }
    }
}

@end
