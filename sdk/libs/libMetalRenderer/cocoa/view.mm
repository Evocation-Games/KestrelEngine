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

#include <libMetalRenderer/cocoa/view.h>
#include <Carbon/Carbon.h>
#include <libUI/hid/key.hpp>
#include <libUI/event/event.hpp>

@implementation MetalRendererView {
@private
    NSTrackingArea *_trackingArea;
    renderer::event::controller *_eventController;
}

- (instancetype)init
{
    if (self = [super initWithFrame:NSZeroRect]) {
        NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
                                         NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
        _trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
        [self addTrackingArea:_trackingArea];
    }
    return self;
}

// MARK: - Accessors

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

// MARK: - Event Controller

- (void)attachEventController:(renderer::event::controller *)controller
{
    _eventController = controller;
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
        event::instance raw;
        raw.type = (enum event::type)(event::type::key_down | event::type::key_typed | flags);
        raw.key.pressed = [self translateKeycodeForEvent:e];
        raw.key.character = [[e characters] characterAtIndex:0];
        _eventController->send(raw);
    }
    else if (down) {
        event::instance raw;
        raw.type = (enum event::type)(event::type::key_down | flags);
        raw.key.pressed = [self translateKeycodeForEvent:e];
        raw.key.character = [[e characters] characterAtIndex:0];
        _eventController->send(raw);
    }
    else {
        event::instance raw;
        raw.type = (enum event::type)(event::type::key_up | flags);
        raw.key.pressed = [self translateKeycodeForEvent:e];
        raw.key.character = [[e characters] characterAtIndex:0];
        _eventController->send(raw);
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
    event::instance raw;
    raw.type = event::type::lmb_down;
    raw.location.x = (float)p.x;
    raw.location.y = (float)p.y;
    _eventController->send(raw);
}

- (void)rightMouseDown:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    event::instance raw;
    raw.type = event::type::rmb_down;
    raw.location.x = (float)p.x;
    raw.location.y = (float)p.y;
    _eventController->send(raw);
}

- (void)mouseUp:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    event::instance raw;
    raw.type = event::type::lmb_up;
    raw.location.x = (float)p.x;
    raw.location.y = (float)p.y;
    _eventController->send(raw);
}

- (void)rightMouseUp:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    event::instance raw;
    raw.type = event::type::rmb_up;
    raw.location.x = (float)p.x;
    raw.location.y = (float)p.y;
    _eventController->send(raw);
}

- (void)mouseMoved:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    event::instance raw;
    raw.type = event::type::mouse_move;
    raw.location.x = (float)p.x;
    raw.location.y = (float)p.y;
    _eventController->send(raw);
}

- (void)mouseDragged:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
    event::instance raw;
    raw.type = event::type::mouse_drag;
    raw.location.x = (float)p.x;
    raw.location.y = (float)p.y;
    _eventController->send(raw);
}

// MARK: - HID

- (event::hid::key)translateKeycodeForEvent:(NSEvent *)event
{
    switch ([event keyCode]) {
        // Special Keys
        case kVK_ANSI_KeypadEnter:
        case kVK_Return:                return event::hid::key::enter;
        case kVK_Tab:                   return event::hid::key::tab;
        case kVK_Delete:                return event::hid::key::backspace;
        case kVK_Escape:                return event::hid::key::escape;

            // Cursor Keys
        case kVK_LeftArrow:             return event::hid::key::left;
        case kVK_RightArrow:            return event::hid::key::right;
        case kVK_DownArrow:             return event::hid::key::down;
        case kVK_UpArrow:               return event::hid::key::up;

            // Letters
        case kVK_ANSI_A:                return event::hid::key::a;
        case kVK_ANSI_S:                return event::hid::key::s;
        case kVK_ANSI_D:                return event::hid::key::d;
        case kVK_ANSI_F:                return event::hid::key::f;
        case kVK_ANSI_H:                return event::hid::key::h;
        case kVK_ANSI_G:                return event::hid::key::g;
        case kVK_ANSI_Z:                return event::hid::key::z;
        case kVK_ANSI_X:                return event::hid::key::x;
        case kVK_ANSI_C:                return event::hid::key::c;
        case kVK_ANSI_V:                return event::hid::key::v;
        case kVK_ANSI_B:                return event::hid::key::b;
        case kVK_ANSI_Q:                return event::hid::key::q;
        case kVK_ANSI_W:                return event::hid::key::w;
        case kVK_ANSI_E:                return event::hid::key::e;
        case kVK_ANSI_R:                return event::hid::key::r;
        case kVK_ANSI_Y:                return event::hid::key::y;
        case kVK_ANSI_T:                return event::hid::key::t;
        case kVK_ANSI_O:                return event::hid::key::o;
        case kVK_ANSI_U:                return event::hid::key::u;
        case kVK_ANSI_I:                return event::hid::key::i;
        case kVK_ANSI_P:                return event::hid::key::p;
        case kVK_ANSI_L:                return event::hid::key::l;
        case kVK_ANSI_J:                return event::hid::key::j;
        case kVK_ANSI_K:                return event::hid::key::k;
        case kVK_ANSI_N:                return event::hid::key::n;
        case kVK_ANSI_M:                return event::hid::key::m;

            // Numbers
        case kVK_ANSI_Keypad0:
        case kVK_ANSI_0:                return event::hid::key::kp_0;
        case kVK_ANSI_Keypad1:
        case kVK_ANSI_1:                return event::hid::key::kp_1;
        case kVK_ANSI_Keypad2:
        case kVK_ANSI_2:                return event::hid::key::kp_2;
        case kVK_ANSI_Keypad3:
        case kVK_ANSI_3:                return event::hid::key::kp_3;
        case kVK_ANSI_Keypad4:
        case kVK_ANSI_4:                return event::hid::key::kp_4;
        case kVK_ANSI_Keypad5:
        case kVK_ANSI_5:                return event::hid::key::kp_5;
        case kVK_ANSI_Keypad6:
        case kVK_ANSI_6:                return event::hid::key::kp_6;
        case kVK_ANSI_Keypad7:
        case kVK_ANSI_7:                return event::hid::key::kp_7;
        case kVK_ANSI_Keypad8:
        case kVK_ANSI_8:                return event::hid::key::kp_8;
        case kVK_ANSI_Keypad9:
        case kVK_ANSI_9:                return event::hid::key::kp_9;

            // Special
        case kVK_Space:                 return event::hid::key::space;
        case kVK_ANSI_Period:           return event::hid::key::period;
        case kVK_ANSI_Comma:            return event::hid::key::comma;
        case kVK_ANSI_Semicolon:        return event::hid::key::semi_colon;
        case kVK_ANSI_Quote:            return event::hid::key::apostrophe;
        case kVK_ANSI_LeftBracket:      return event::hid::key::left_bracket;
        case kVK_ANSI_RightBracket:     return event::hid::key::right_bracket;
        case kVK_ANSI_Grave:            return event::hid::key::grave_accent;
        case kVK_ANSI_Backslash:        return event::hid::key::backslash;
        case kVK_ANSI_Slash:            return event::hid::key::slash;
        case kVK_ANSI_Minus:            return event::hid::key::minus;
        case kVK_ANSI_Equal:            return event::hid::key::equal;

        case kVK_F1:                    return event::hid::key::f1;
        case kVK_F2:                    return event::hid::key::f2;
        case kVK_F3:                    return event::hid::key::f3;
        case kVK_F4:                    return event::hid::key::f4;
        case kVK_F5:                    return event::hid::key::f5;
        case kVK_F6:                    return event::hid::key::f6;
        case kVK_F7:                    return event::hid::key::f7;
        case kVK_F8:                    return event::hid::key::f8;
        case kVK_F9:                    return event::hid::key::f9;
        case kVK_F10:                   return event::hid::key::f10;
        case kVK_F11:                   return event::hid::key::f11;
        case kVK_F12:                   return event::hid::key::f12;

            // Modifiers
        case kVK_Command:               return event::hid::key::left_super;
        case kVK_RightCommand:          return event::hid::key::right_super;
        case kVK_Control:               return event::hid::key::left_control;
        case kVK_RightControl:          return event::hid::key::right_control;
        case kVK_Option:                return event::hid::key::left_alt;
        case kVK_RightOption:           return event::hid::key::right_alt;
        case kVK_Shift:                 return event::hid::key::left_shift;
        case kVK_RightShift:            return event::hid::key::right_shift;

            // Unknown
        default:                        return event::hid::key::unknown;
    }
}

- (enum event::type)translateKeyModifiersForEvent:(NSEvent *)event
{
    std::uint32_t modifier_flags = 0;

    if ([event modifierFlags] & NSEventModifierFlagShift) {
        modifier_flags |= static_cast<std::uint32_t>(event::type::has_shift_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagOption) {
        modifier_flags |= static_cast<std::uint32_t>(event::type::has_alt_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagCapsLock) {
        modifier_flags |= static_cast<std::uint32_t>(event::type::has_caps_lock_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagControl) {
        modifier_flags |= static_cast<std::uint32_t>(event::type::has_control_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagCommand) {
        modifier_flags |= static_cast<std::uint32_t>(event::type::has_super_modifier);
    }

    return (enum event::type)modifier_flags;
}

@end
