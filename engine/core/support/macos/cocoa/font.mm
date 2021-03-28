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

#include <iostream>
#include "core/support/macos/cocoa/font.h"
#include "core/support/macos/cocoa/cocoa_utils.h"

auto cocoa::font::path_for(const std::string &name) -> std::string
{
    // 1. Try and load the font through standard means via NSFont. This will allow us
    // to then look up the font on disk and return the path.
    NSFont *font = [NSFont fontWithName:cocoa::string::to(name) size:12.0];
    if (font) {
        CTFontDescriptorRef fontRef = CTFontDescriptorCreateWithNameAndSize ((CFStringRef)[font fontName], [font pointSize]);
        CFURLRef url = (CFURLRef)CTFontDescriptorCopyAttribute(fontRef, kCTFontURLAttribute);
        NSString *fontPath = [NSString stringWithString:[(NSURL *)CFBridgingRelease(url) path]];
        CFRelease(fontRef);
//        CFRelease(url);
        if ([[fontPath pathExtension] isEqualToString:@"ttf"] || [[fontPath pathExtension] isEqualToString:@"ttc"]) {
            return cocoa::string::from(fontPath);
        }
    }

    // 2. Fallback on a known system default.
    return "/System/Library/Fonts/SFCompactDisplay.ttf";
}
