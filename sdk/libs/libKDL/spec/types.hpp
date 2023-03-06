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
#include <vector>
#include <string>

namespace kdl::spec::types
{
    static constexpr const char *integer = "Integer";
    static constexpr const char *string = "String";
    static constexpr const char *bitmask = "Bitmask";
    static constexpr const char *file = "File";
    static constexpr const char *image = "Image";
    static constexpr const char *image_set = "ImageSet";
    static constexpr const char *sound = "Sound";
    static constexpr const char *boolean = "Boolean";
    static constexpr const char *data = "BinaryData";

    static auto vector() -> std::vector<std::string>
    {
        return {
            integer, string, bitmask, file, image, image_set, boolean, data, sound
        };
    }
}