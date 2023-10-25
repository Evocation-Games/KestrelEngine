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
//        auto new_event = kestrel::event::key(
//            static_cast<enum ::ui::event::type>(::ui::event::type::key_down | ::ui::event::type::key_typed | flags),
//            [self translateKeycodeForEvent:e],
//            [[e characters] characterAtIndex:0]
//        );
//        kestrel::post_event(new_event);
    }
    else if (down) {
//        auto new_event = kestrel::event::key(
//            static_cast<enum ::ui::event::type>(::ui::event::type::key_down | flags),
//            [self translateKeycodeForEvent:e],
//            [[e characters] characterAtIndex:0]
//        );
//        kestrel::post_event(new_event);
    }
    else {
//        auto new_event = kestrel::event::key(
//            static_cast<enum ::ui::event::type>(::ui::event::type::key_up | flags),
//            [self translateKeycodeForEvent:e],
//            [[e characters] characterAtIndex:0]
//        );
//        kestrel::post_event(new_event);
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
//    auto e = kestrel::event::mouse(::ui::event::type::lmb_down, { static_cast<float>(p.x), static_cast<float>(p.y) });
//    kestrel::post_event(e);
}

- (void)rightMouseDown:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
//    auto e = kestrel::event::mouse(::ui::event::type::rmb_down, { static_cast<float>(p.x), static_cast<float>(p.y) });
//    kestrel::post_event(e);
}

- (void)mouseUp:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
//    auto e = kestrel::event::mouse(::ui::event::type::lmb_up, { static_cast<float>(p.x), static_cast<float>(p.y) });
//    kestrel::post_event(e);
}

- (void)rightMouseUp:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
//    auto e = kestrel::event::mouse(::ui::event::type::rmb_up, { static_cast<float>(p.x), static_cast<float>(p.y) });
//    kestrel::post_event(e);
}

- (void)mouseMoved:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
//    auto e = kestrel::event::mouse(::ui::event::type::mouse_move, { static_cast<float>(p.x), static_cast<float>(p.y) });
//    kestrel::post_event(e);
}

- (void)mouseDragged:(NSEvent *)event
{
    auto p = [self convertPoint:event.locationInWindow fromView:nil];
//    auto e = kestrel::event::mouse(::ui::event::type::mouse_drag, { static_cast<float>(p.x), static_cast<float>(p.y) });
//    kestrel::post_event(e);
}

// MARK: - HID

- (enum ui::hid::key)translateKeycodeForEvent:(NSEvent *)event
{
    switch ([event keyCode]) {
        // Special Keys
        case kVK_ANSI_KeypadEnter:
        case kVK_Return:                return ui::hid::key::enter;
        case kVK_Tab:                   return ui::hid::key::tab;
        case kVK_Delete:                return ui::hid::key::backspace;
        case kVK_Escape:                return ui::hid::key::escape;

            // Cursor Keys
        case kVK_LeftArrow:             return ui::hid::key::left;
        case kVK_RightArrow:            return ui::hid::key::right;
        case kVK_DownArrow:             return ui::hid::key::down;
        case kVK_UpArrow:               return ui::hid::key::up;

            // Letters
        case kVK_ANSI_A:                return ui::hid::key::a;
        case kVK_ANSI_S:                return ui::hid::key::s;
        case kVK_ANSI_D:                return ui::hid::key::d;
        case kVK_ANSI_F:                return ui::hid::key::f;
        case kVK_ANSI_H:                return ui::hid::key::h;
        case kVK_ANSI_G:                return ui::hid::key::g;
        case kVK_ANSI_Z:                return ui::hid::key::z;
        case kVK_ANSI_X:                return ui::hid::key::x;
        case kVK_ANSI_C:                return ui::hid::key::c;
        case kVK_ANSI_V:                return ui::hid::key::v;
        case kVK_ANSI_B:                return ui::hid::key::b;
        case kVK_ANSI_Q:                return ui::hid::key::q;
        case kVK_ANSI_W:                return ui::hid::key::w;
        case kVK_ANSI_E:                return ui::hid::key::e;
        case kVK_ANSI_R:                return ui::hid::key::r;
        case kVK_ANSI_Y:                return ui::hid::key::y;
        case kVK_ANSI_T:                return ui::hid::key::t;
        case kVK_ANSI_O:                return ui::hid::key::o;
        case kVK_ANSI_U:                return ui::hid::key::u;
        case kVK_ANSI_I:                return ui::hid::key::i;
        case kVK_ANSI_P:                return ui::hid::key::p;
        case kVK_ANSI_L:                return ui::hid::key::l;
        case kVK_ANSI_J:                return ui::hid::key::j;
        case kVK_ANSI_K:                return ui::hid::key::k;
        case kVK_ANSI_N:                return ui::hid::key::n;
        case kVK_ANSI_M:                return ui::hid::key::m;

            // Numbers
        case kVK_ANSI_Keypad0:
        case kVK_ANSI_0:                return ui::hid::key::kp_0;
        case kVK_ANSI_Keypad1:
        case kVK_ANSI_1:                return ui::hid::key::kp_1;
        case kVK_ANSI_Keypad2:
        case kVK_ANSI_2:                return ui::hid::key::kp_2;
        case kVK_ANSI_Keypad3:
        case kVK_ANSI_3:                return ui::hid::key::kp_3;
        case kVK_ANSI_Keypad4:
        case kVK_ANSI_4:                return ui::hid::key::kp_4;
        case kVK_ANSI_Keypad5:
        case kVK_ANSI_5:                return ui::hid::key::kp_5;
        case kVK_ANSI_Keypad6:
        case kVK_ANSI_6:                return ui::hid::key::kp_6;
        case kVK_ANSI_Keypad7:
        case kVK_ANSI_7:                return ui::hid::key::kp_7;
        case kVK_ANSI_Keypad8:
        case kVK_ANSI_8:                return ui::hid::key::kp_8;
        case kVK_ANSI_Keypad9:
        case kVK_ANSI_9:                return ui::hid::key::kp_9;

            // Special
        case kVK_Space:                 return ui::hid::key::space;
        case kVK_ANSI_Period:           return ui::hid::key::period;
        case kVK_ANSI_Comma:            return ui::hid::key::comma;
        case kVK_ANSI_Semicolon:        return ui::hid::key::semi_colon;
        case kVK_ANSI_Quote:            return ui::hid::key::apostrophe;
        case kVK_ANSI_LeftBracket:      return ui::hid::key::left_bracket;
        case kVK_ANSI_RightBracket:     return ui::hid::key::right_bracket;
        case kVK_ANSI_Grave:            return ui::hid::key::grave_accent;
        case kVK_ANSI_Backslash:        return ui::hid::key::backslash;
        case kVK_ANSI_Slash:            return ui::hid::key::slash;
        case kVK_ANSI_Minus:            return ui::hid::key::minus;
        case kVK_ANSI_Equal:            return ui::hid::key::equal;

        case kVK_F1:                    return ui::hid::key::f1;
        case kVK_F2:                    return ui::hid::key::f2;
        case kVK_F3:                    return ui::hid::key::f3;
        case kVK_F4:                    return ui::hid::key::f4;
        case kVK_F5:                    return ui::hid::key::f5;
        case kVK_F6:                    return ui::hid::key::f6;
        case kVK_F7:                    return ui::hid::key::f7;
        case kVK_F8:                    return ui::hid::key::f8;
        case kVK_F9:                    return ui::hid::key::f9;
        case kVK_F10:                   return ui::hid::key::f10;
        case kVK_F11:                   return ui::hid::key::f11;
        case kVK_F12:                   return ui::hid::key::f12;

            // Modifiers
        case kVK_Command:               return ui::hid::key::left_super;
        case kVK_RightCommand:          return ui::hid::key::right_super;
        case kVK_Control:               return ui::hid::key::left_control;
        case kVK_RightControl:          return ui::hid::key::right_control;
        case kVK_Option:                return ui::hid::key::left_alt;
        case kVK_RightOption:           return ui::hid::key::right_alt;
        case kVK_Shift:                 return ui::hid::key::left_shift;
        case kVK_RightShift:            return ui::hid::key::right_shift;

            // Unknown
        default:                        return ui::hid::key::unknown;
    }
}

- (enum ui::event::type)translateKeyModifiersForEvent:(NSEvent *)event
{
    std::uint32_t modifier_flags = 0;

    if ([event modifierFlags] & NSEventModifierFlagShift) {
        modifier_flags |= static_cast<std::uint32_t>(::ui::event::type::has_shift_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagOption) {
        modifier_flags |= static_cast<std::uint32_t>(::ui::event::type::has_alt_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagCapsLock) {
        modifier_flags |= static_cast<std::uint32_t>(::ui::event::type::has_caps_lock_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagControl) {
        modifier_flags |= static_cast<std::uint32_t>(::ui::event::type::has_control_modifier);
    }
    else if ([event modifierFlags] & NSEventModifierFlagCommand) {
        modifier_flags |= static_cast<std::uint32_t>(::ui::event::type::has_super_modifier);
    }

    return static_cast<enum ::ui::event::type>(modifier_flags);
}

@end
