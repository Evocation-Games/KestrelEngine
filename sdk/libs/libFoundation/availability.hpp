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

#if (_WIN32 || _WIN64)
#   define TARGET_WINDOWS               true
#else
#   define TARGET_WINDOWS               false
#endif

#if (__linux)
#   define TARGET_LINUX                 true
#else
#   define TARGET_LINUX                 false
#endif

#if (__APPLE__)
#   define TARGET_MACOS                 true
#   if (__arm64__ || __arm64)
#       define TARGET_MACOS_M1          true
#       define TARGET_MACOS_INTEL       false
#   elif (__x86_64__)
#       define TARGET_MACOS_M1          false
#       define TARGET_MACOS_INTEL       true
#   else
#       define TARGET_MACOS_M1          false
#       define TARGET_MACOS_INTEL       false
#   endif
#else
#   define TARGET_MACOS                 false
#   define TARGET_MACOS_M1              false
#   define TARGET_MACOS_INTEL           false
#endif

#define API_VERSION(...)           /* (_version) */