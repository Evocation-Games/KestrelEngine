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

#include "core/support/macos/cocoa/application.h"
#include "core/support/macos/cocoa/cocoa_utils.h"
#include <Cocoa/Cocoa.h>

// MARK: - Application Bundle

auto cocoa::application::bundle_path() -> std::string
{
    return cocoa::string::from([[NSBundle mainBundle] bundlePath]);
}

auto cocoa::application::application_support_path() -> std::string
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    // TODO: Make this directory customizable.
    NSString *applicationSupportDirectory = [[paths firstObject] stringByAppendingPathComponent:@"Kestrel"];
    return cocoa::string::from(applicationSupportDirectory);
}

// MARK: - Screen

auto cocoa::application::screen_scale_factor() -> double
{
    return cocoa::screen::scale_factor();
}
