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

#include <libMetalRenderer/cocoa/application.h>
#include <libMetalRenderer/cocoa/view.h>
#include <MetalKit/MetalKit.h>

// MARK: -

@interface MetalRendererApplication () <NSApplicationDelegate>
@end

@implementation MetalRendererApplication {
@private
    NSApplication *_app;
    NSMenu *_menuBar;
    NSMenuItem *_appMenuItem;
    NSMenu *_appMenu;
    NSMenuItem *_quitMenuItem;
    NSString *_appName;
    std::function<auto(MetalRendererApplication *)->NSInteger> _continuation;
}

- (NSInteger)runWithContinuation:(std::function<auto(MetalRendererApplication *)->NSInteger>)continuation
{
    @try {
        _continuation = continuation;
        [self configureApplication];
        [self configureMenuBar];
        return [self run];
    }
    @catch (...) {
        // TODO: Handle any exceptions
    }
}

- (void)configureApplication
{
    _app = NSApplication.sharedApplication;
    _app.activationPolicy = NSApplicationActivationPolicyRegular;
    _app.delegate = self;

    // Get some basic information about the process itself.
    _appName = NSProcessInfo.processInfo.processName;
}

- (void)configureMenuBar
{
    // Create the main app menu
    _appMenu = [NSMenu new];
    _appMenuItem = [NSMenuItem new];
    [_appMenuItem setSubmenu:_appMenu];

    _quitMenuItem = [[NSMenuItem alloc] initWithTitle:[@"Quit " stringByAppendingString:_appName] action:@selector(terminate:) keyEquivalent:@"q"];
    [_appMenu addItem:_quitMenuItem];

    // Finally create the menu bar itself
    _menuBar = [NSMenu new];
    [_menuBar addItem:_appMenuItem];
    _app.mainMenu = _menuBar;
}

- (NSInteger)run
{
    [_app activateIgnoringOtherApps:YES];
    [_app run];
    return 0;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    try {
        _continuation(self);
    }
    catch (std::exception& e) {
        // TODO: Determine what is the best course of action to take with received
        // exceptions.
        exit(1);
    }
}

- (NSWindow *)createWindowWithTitle:(NSString *)title size:(CGSize)size
{
    NSWindow *wnd = [[NSWindow alloc] initWithContentRect:NSMakeRect(0.0, 0.0, size.width, size.height)
                                                styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                                  backing:NSBackingStoreBuffered
                                                    defer:YES];
    [wnd cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [wnd makeKeyAndOrderFront:nil];
    [wnd becomeMainWindow];
    [wnd becomeKeyWindow];
    [wnd center];
    [wnd setTitle:title];
    [[wnd contentView] setWantsLayer:YES];
    [[wnd contentView] setLayerContentsRedrawPolicy:NSViewLayerContentsRedrawDuringViewResize];

    auto titlebar_height = [wnd frame].size.height - [[wnd contentView] frame].size.height;
    [[wnd contentView] setFrame:NSMakeRect(0.0, 0.0, size.width, size.height + titlebar_height)];

    // Setup view
    MetalRendererView *view = [[MetalRendererView alloc] init];
    [view setFrame:[[wnd contentView] frame]];
    [wnd setContentView:view];

    // Create and add a metal rendering layer.
    CAMetalLayer *metal = [CAMetalLayer layer];
    [[wnd contentView] setLayer:metal];

    return wnd;
}

@end