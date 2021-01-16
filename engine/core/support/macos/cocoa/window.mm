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

#include "core/support/macos/cocoa/window.h"
#include "core/support/macos/cocoa/cocoa_utils.h"
#include "core/support/macos/cocoa/view.h"

// MARK: - Construction

cocoa::window::window()
{
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 200, 200)
                                                   styleMask:NSWindowStyleMaskTitled
                                                     backing:NSBackingStoreBuffered
                                                       defer:YES];
    [window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [window makeKeyAndOrderFront:nil];
    [window center];
    cocoa::object::set(window);
}

cocoa::window::window(void *handle)
{
    cocoa::object::set(handle);
}

// MARK: - Accessors

auto cocoa::window::set_title(const std::string &title) -> void
{
    [cocoa::object::get<NSWindow *>() setTitle:cocoa::string::to(title)];
}

auto cocoa::window::title() const -> std::string
{
    return cocoa::string::from([cocoa::object::get<NSWindow *>() title]);
}

auto cocoa::window::set_size(const int& width, const int& height) -> void
{
    [cocoa::object::get<NSWindow *>() setFrame:NSMakeRect(0, 0, width, height) display:YES];
    [cocoa::object::get<NSWindow *>() center];
}

// MARK: - Subviews

auto cocoa::window::set_content_view(const std::shared_ptr<cocoa::view> &view) -> void
{
    auto cocoa_view = view->get<NSView *>();
    [cocoa_view setTranslatesAutoresizingMaskIntoConstraints:NO];
    [cocoa_view setFrame:[get<NSWindow *>() frame]];

    // Remove existing subview.
    for (NSView *view in [[get<NSWindow *>() contentView] subviews]) {
        [view removeFromSuperview];
    }

    auto content_view = [get<NSWindow *>() contentView];
    [content_view addSubview:cocoa_view];

    [NSLayoutConstraint activateConstraints:@[
        [cocoa_view.leftAnchor constraintEqualToAnchor:content_view.leftAnchor],
        [cocoa_view.rightAnchor constraintEqualToAnchor:content_view.rightAnchor],
        [cocoa_view.topAnchor constraintEqualToAnchor:content_view.topAnchor],
        [cocoa_view.bottomAnchor constraintEqualToAnchor:content_view.bottomAnchor],
    ]];
}
