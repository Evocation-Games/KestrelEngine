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

#pragma once

#if !defined(LIB_BUILD_VERSION)
#   define LIB_BUILD_VERSION(_major, _minor, _fix)  _major._minor._fix
#endif

#define LIB_KESTREL_VERSION_LEGACY  LIB_BUILD_VERSION(0,0,0)
#define LIB_KESTREL_VERSION_0_8     LIB_BUILD_VERSION(0,8,0)
#define LIB_KESTREL_VERSION_0_9     LIB_BUILD_VERSION(0,9,0)
#define LIB_KESTREL_VERSION_1_0     LIB_BUILD_VERSION(1,0,0)

#define LIB_KESTREL_VERSION_CURRENT LIB_KESTREL_VERSION_0_8