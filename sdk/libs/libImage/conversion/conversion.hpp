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

#pragma once

#include <type_traits>
#include <libData/block.hpp>
#include <libQuickdraw/surface/surface.hpp>

namespace image::conversion
{
    struct format {
        static constexpr const char *png = "PNG";
        static constexpr const char *tga = "TGA";
        static constexpr const char *pict = "PICT";
        static constexpr const char *cicn = "CICN";
        static constexpr const char *ppat = "PPAT";
        static constexpr const char *rled = "rleD";
        static constexpr const char *rlex = "rleX";
    };

    auto surface(const std::string& source, const data::block& data) -> quickdraw::surface;
    auto data(const std::string& result, quickdraw::surface& surface) -> data::block;

    auto surface(const std::string& source, const std::vector<data::block>& data) -> std::vector<quickdraw::surface>;
    auto data(const std::string& result, const std::vector<quickdraw::surface>& surface) -> data::block;

    auto perform(const std::string& source, const std::string& result, const data::block& data) -> data::block;
    auto perform(const std::string& source, const std::string& result, const std::vector<data::block>& data) -> data::block;
}
