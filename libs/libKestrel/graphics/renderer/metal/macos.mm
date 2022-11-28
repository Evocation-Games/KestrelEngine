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

#include <libKestrel/graphics/renderer/metal/macos.h>

#if TARGET_MACOS
#include <Carbon/Carbon.h>
#include <libKestrel/event/key.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/kestrel.hpp>
#include <LuaBridge/detail/LuaException.h>

@interface KestrelApplication() <NSApplicationDelegate>
- (void)runWithContinuation:(const std::function<auto(KestrelApplication *)->void>&)continuation;
@end

@interface KestrelView : NSView
@end

// MARK: - macOS Application Entry

auto kestrel::platform::macos::start_application(const std::function<auto(KestrelApplication *) -> void> &main) -> void
{
    KestrelApplication *app = [[KestrelApplication alloc] init];
    [app runWithContinuation:main];
    exit(0);
}

// MARK: - Application

@implementation KestrelApplication {
    std::function<auto(KestrelApplication *)->void> _continuation;
}

- (void)runWithContinuation:(const std::function<auto(KestrelApplication *)->void>&)continuation
{
    @try {
        _continuation = continuation;

        // Start up and configure the application environment and run loop.
        @autoreleasepool {
            NSApplication *app = NSApplication.sharedApplication;
            app.activationPolicy = NSApplicationActivationPolicyRegular;

            NSMenu *menubar = [NSMenu new];
            NSMenuItem *appMenuItem = [NSMenuItem new];
            [menubar addItem:appMenuItem];
            [app setMainMenu:menubar];
            NSMenu *appMenu = [NSMenu new];
            NSString *appName = [[NSProcessInfo processInfo] processName];
            NSString *quitTitle = [@"Quit " stringByAppendingString:appName];
            NSMenuItem *quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(terminate:) keyEquivalent:@"q"];
            [appMenu addItem:quitMenuItem];
            [appMenuItem setSubmenu:appMenu];

            app.delegate = self;

            [app activateIgnoringOtherApps:YES];
            [app run];
        }
    }
    @catch (...) {
        try {
            std::rethrow_exception(std::current_exception());
        }
        catch (std::exception& e) {
            NSAlert *alert = [[NSAlert alloc] init];
            alert.messageText = @"Runtime Exception Occurred";
            alert.informativeText = [NSString stringWithUTF8String:e.what()];
            [alert addButtonWithTitle:@"Exit"];
            [alert runModal];
            std::cerr << "Unknown Exception occurred" << std::endl;
        }
        exit(1);
    }
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    try {
        _continuation(self);
    }
    catch (luabridge::LuaException& e) {
        NSAlert *alert = [[NSAlert alloc] init];
        alert.messageText = @"Lua Runtime Exception Occurred";
        alert.informativeText = [NSString stringWithUTF8String:e.what()];
        [alert addButtonWithTitle:@"Exit"];

        auto response = [alert runModal];

        std::cerr << "LuaBridge Exception: " << e.what() << std::endl;
        exit(2);
    }
    catch (...) {
        std::cerr << "Unknown Exception occurred" << std::endl;
        exit(1);
    }
}

// MARK: - Window

- (NSWindow *)createWindowWithTitle:(NSString *)string withSize:(kestrel::math::size)size
{
    // Setup the basic window...
    NSWindow *wnd = [[NSWindow alloc] initWithContentRect:NSMakeRect(0.0, 0.0, size.width, size.height)
                                                styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                                  backing:NSBackingStoreBuffered
                                                    defer:YES];
    [wnd cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [wnd makeKeyAndOrderFront:nil];
    [wnd becomeMainWindow];
    [wnd becomeKeyWindow];
    [wnd center];
    [[wnd contentView] setWantsLayer:YES];
    [[wnd contentView] setLayerContentsRedrawPolicy:NSViewLayerContentsRedrawDuringViewResize];

    auto titlebar_height = [wnd frame].size.height - [[wnd contentView] frame].size.height;
    [[wnd contentView] setFrame:NSMakeRect(0.0, 0.0, size.width, size.height + titlebar_height)];

    // Setup view
    KestrelView *view = [[KestrelView alloc] init];
    [view setFrame:[[wnd contentView] frame]];
    [wnd setContentView:view];

    // Create and add a metal rendering layer.
    CAMetalLayer *metal = [CAMetalLayer layer];
    [[wnd contentView] setLayer:metal];

    return wnd;
}

@end

// MARK: - View

@implementation KestrelView {
@private
    NSTrackingArea *m_trackingArea;
}

- (instancetype)init
{
    if (self = [super initWithFrame:NSZeroRect]) {
        // Setup a tracking area for mouse move events.
        NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
            NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
        m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
        [self addTrackingArea:m_trackingArea];
    }
    return self;
}

- (BOOL)isFlipped
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

// MARK: - Key Events

- (void)keyEvent:(NSEvent *)e down:(BOOL)down
{
    bool(^is_typed)(NSString *) = ^bool(NSString *str) {
        const unichar backspace = '\u007f';
        for (int i = 0; i < str.length; ++i) {
            unichar c = [str characterAtIndex:i];
            switch (c) {
                case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
                case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
                case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
                case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f:
                case U'\u007f': // backspace
                case U'\uf700': // up
                case U'\uf701': // down
                case U'\uf702': // left
                case U'\uf703': // right
                case U'\uf728': // delete?
                    return NO;
                default:
                    break;
            }
        }
        return YES;
    };

    auto flags = [self translateKeyModifiersForEvent:e];

    if (down && is_typed([e characters])) {
        auto new_event = kestrel::event::key(
            static_cast<enum kestrel::event_type>(kestrel::event_type::key_down | kestrel::event_type::key_typed | static_cast<std::uint32_t>(flags)),
            [self translateKeycodeForEvent:e],
            [[e characters] characterAtIndex:0]
        );
        kestrel::post_event(new_event);
    }
    else if (down) {
        auto new_event = kestrel::event::key(
            static_cast<enum kestrel::event_type>(kestrel::event_type::key_down | static_cast<std::uint32_t>(flags)),
            [self translateKeycodeForEvent:e],
            [[e characters] characterAtIndex:0]
        );
        kestrel::post_event(new_event);
    }
    else {
        auto new_event = kestrel::event::key(
            static_cast<enum kestrel::event_type>(kestrel::event_type::key_up | static_cast<std::uint32_t>(flags)),
            [self translateKeycodeForEvent:e],
            [[e characters] characterAtIndex:0]
        );
        kestrel::post_event(new_event);
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

}

// MARK: - Mouse Events

- (void)mouseDown:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    auto e = kestrel::event::mouse(kestrel::event_type::lmb_down, { p.x, p.y });
    kestrel::post_event(e);
}

- (void)rightMouseDown:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    auto e = kestrel::event::mouse(kestrel::event_type::rmb_down, { p.x, p.y });
    kestrel::post_event(e);
}

- (void)mouseUp:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    auto e = kestrel::event::mouse(kestrel::event_type::lmb_up, { p.x, p.y });
    kestrel::post_event(e);
}

- (void)rightMouseUp:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    auto e = kestrel::event::mouse(kestrel::event_type::rmb_up, { p.x, p.y });
    kestrel::post_event(e);
}

- (void)mouseMoved:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    auto e = kestrel::event::mouse(kestrel::event_type::mouse_move, { p.x, p.y });
    kestrel::post_event(e);
}

- (void)mouseDragged:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    auto e = kestrel::event::mouse(kestrel::event_type::mouse_drag, { p.x, p.y });
    kestrel::post_event(e);
}

// MARK: - HID

- (kestrel::hid::key)translateKeycodeForEvent:(NSEvent *)event
{
    switch ([event keyCode]) {
        // Special Keys
        case kVK_ANSI_KeypadEnter:
        case kVK_Return:                return kestrel::hid::key::enter;
        case kVK_Tab:                   return kestrel::hid::key::tab;
        case kVK_Delete:                return kestrel::hid::key::backspace;
        case kVK_Escape:                return kestrel::hid::key::escape;

        // Cursor Keys
        case kVK_LeftArrow:             return kestrel::hid::key::left;
        case kVK_RightArrow:            return kestrel::hid::key::right;
        case kVK_DownArrow:             return kestrel::hid::key::down;
        case kVK_UpArrow:               return kestrel::hid::key::up;

        // Letters
        case kVK_ANSI_A:                return kestrel::hid::key::a;
        case kVK_ANSI_S:                return kestrel::hid::key::s;
        case kVK_ANSI_D:                return kestrel::hid::key::d;
        case kVK_ANSI_F:                return kestrel::hid::key::f;
        case kVK_ANSI_H:                return kestrel::hid::key::h;
        case kVK_ANSI_G:                return kestrel::hid::key::g;
        case kVK_ANSI_Z:                return kestrel::hid::key::z;
        case kVK_ANSI_X:                return kestrel::hid::key::x;
        case kVK_ANSI_C:                return kestrel::hid::key::c;
        case kVK_ANSI_V:                return kestrel::hid::key::v;
        case kVK_ANSI_B:                return kestrel::hid::key::b;
        case kVK_ANSI_Q:                return kestrel::hid::key::q;
        case kVK_ANSI_W:                return kestrel::hid::key::w;
        case kVK_ANSI_E:                return kestrel::hid::key::e;
        case kVK_ANSI_R:                return kestrel::hid::key::r;
        case kVK_ANSI_Y:                return kestrel::hid::key::y;
        case kVK_ANSI_T:                return kestrel::hid::key::t;
        case kVK_ANSI_O:                return kestrel::hid::key::o;
        case kVK_ANSI_U:                return kestrel::hid::key::u;
        case kVK_ANSI_I:                return kestrel::hid::key::i;
        case kVK_ANSI_P:                return kestrel::hid::key::p;
        case kVK_ANSI_L:                return kestrel::hid::key::l;
        case kVK_ANSI_J:                return kestrel::hid::key::j;
        case kVK_ANSI_K:                return kestrel::hid::key::k;
        case kVK_ANSI_N:                return kestrel::hid::key::n;
        case kVK_ANSI_M:                return kestrel::hid::key::m;

        // Numbers
        case kVK_ANSI_Keypad0:
        case kVK_ANSI_0:                return kestrel::hid::key::kp_0;
        case kVK_ANSI_Keypad1:
        case kVK_ANSI_1:                return kestrel::hid::key::kp_1;
        case kVK_ANSI_Keypad2:
        case kVK_ANSI_2:                return kestrel::hid::key::kp_2;
        case kVK_ANSI_Keypad3:
        case kVK_ANSI_3:                return kestrel::hid::key::kp_3;
        case kVK_ANSI_Keypad4:
        case kVK_ANSI_4:                return kestrel::hid::key::kp_4;
        case kVK_ANSI_Keypad5:
        case kVK_ANSI_5:                return kestrel::hid::key::kp_5;
        case kVK_ANSI_Keypad6:
        case kVK_ANSI_6:                return kestrel::hid::key::kp_6;
        case kVK_ANSI_Keypad7:
        case kVK_ANSI_7:                return kestrel::hid::key::kp_7;
        case kVK_ANSI_Keypad8:
        case kVK_ANSI_8:                return kestrel::hid::key::kp_8;
        case kVK_ANSI_Keypad9:
        case kVK_ANSI_9:                return kestrel::hid::key::kp_9;

        // Special
        case kVK_Space:                 return kestrel::hid::key::space;
        case kVK_ANSI_Period:           return kestrel::hid::key::period;
        case kVK_ANSI_Comma:            return kestrel::hid::key::comma;
        case kVK_ANSI_Semicolon:        return kestrel::hid::key::semi_colon;
        case kVK_ANSI_Quote:            return kestrel::hid::key::apostrophe;
        case kVK_ANSI_LeftBracket:      return kestrel::hid::key::left_bracket;
        case kVK_ANSI_RightBracket:     return kestrel::hid::key::right_bracket;
        case kVK_ANSI_Grave:            return kestrel::hid::key::grave_accent;
        case kVK_ANSI_Backslash:        return kestrel::hid::key::backslash;
        case kVK_ANSI_Slash:            return kestrel::hid::key::slash;
        case kVK_ANSI_Minus:            return kestrel::hid::key::minus;
        case kVK_ANSI_Equal:            return kestrel::hid::key::equal;

        case kVK_F1:                    return kestrel::hid::key::f1;
        case kVK_F2:                    return kestrel::hid::key::f2;
        case kVK_F3:                    return kestrel::hid::key::f3;
        case kVK_F4:                    return kestrel::hid::key::f4;
        case kVK_F5:                    return kestrel::hid::key::f5;
        case kVK_F6:                    return kestrel::hid::key::f6;
        case kVK_F7:                    return kestrel::hid::key::f7;
        case kVK_F8:                    return kestrel::hid::key::f8;
        case kVK_F9:                    return kestrel::hid::key::f9;
        case kVK_F10:                   return kestrel::hid::key::f10;
        case kVK_F11:                   return kestrel::hid::key::f11;
        case kVK_F12:                   return kestrel::hid::key::f12;

        // Modifiers
        case kVK_Command:               return kestrel::hid::key::left_super;
        case kVK_RightCommand:          return kestrel::hid::key::right_super;
        case kVK_Control:               return kestrel::hid::key::left_control;
        case kVK_RightControl:          return kestrel::hid::key::right_control;
        case kVK_Option:                return kestrel::hid::key::left_alt;
        case kVK_RightOption:           return kestrel::hid::key::right_alt;
        case kVK_Shift:                 return kestrel::hid::key::left_shift;
        case kVK_RightShift:            return kestrel::hid::key::right_shift;

        // Unknown
        default:                        return kestrel::hid::key::unknown;
    }
}

- (enum kestrel::event_type)translateKeyModifiersForEvent:(NSEvent *)event
{
    std::uint32_t modifier_flags = 0;

    if ([event modifierFlags] & NSEventModifierFlagShift) {
        modifier_flags |= static_cast<std::uint32_t>(kestrel::event_type::has_shift_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagOption) {
        modifier_flags |= static_cast<std::uint32_t>(kestrel::event_type::has_alt_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagCapsLock) {
        modifier_flags |= static_cast<std::uint32_t>(kestrel::event_type::has_caps_lock_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagControl) {
        modifier_flags |= static_cast<std::uint32_t>(kestrel::event_type::has_control_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagCommand) {
        modifier_flags |= static_cast<std::uint32_t>(kestrel::event_type::has_super_modifier);
    }

    return static_cast<enum kestrel::event_type>(modifier_flags);
}

@end

#endif
