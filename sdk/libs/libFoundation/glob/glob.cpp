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

#include <iostream>
#include <libFoundation/availability.hpp>
#include <libFoundation/glob/glob.hpp>

#if TARGET_MACOS || TARGET_LINUX
#include <glob.h>
#include <pwd.h>
#include <unistd.h>
#endif

// MARK: - Glob Support Query

constexpr auto foundation::glob_supported() -> bool
{
#if TARGET_WINDOWS
    return false;
#else
    return false;
#endif
}

// MARK: - Glob Functionality

auto foundation::glob(const filesystem::path &path) -> std::vector<filesystem::path>
{
    std::vector<filesystem::path> paths;

#if TARGET_WINDOWS
    std::cerr << "File globbing is not currently supported in the Windows version of KDL." << std::endl;
    paths.emplace_back(path);

#else
    glob_t result;
    auto err = ::glob(path.c_str(), GLOB_ERR | GLOB_MARK, nullptr, &result);

#endif

    return std::move(paths);
}