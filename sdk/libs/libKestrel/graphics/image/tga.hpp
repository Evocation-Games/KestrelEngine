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

#pragma once

#include <vector>
#include <libQuickdraw/surface/surface.hpp>
#include <libData/block.hpp>

namespace kestrel::image
{
    class tga
    {
    protected:
        struct header
        {
            std::uint8_t id_length;
            std::uint8_t color_map_type;
            std::uint8_t data_type_code;
            std::uint16_t color_map_origin;
            std::uint16_t color_map_length;
            std::uint8_t color_map_depth;
            std::uint16_t x_origin;
            std::uint16_t y_origin;
            std::uint16_t width;
            std::uint16_t height;
            std::uint8_t bits_per_pixel;
            std::uint8_t image_descriptor;
        };

        struct pixel
        {
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;
            std::uint8_t a;
        };

    public:
        explicit tga(const std::string& path);
        explicit tga(const data::block& data);
        explicit tga(quickdraw::surface& surface);

        [[nodiscard]] auto surface() const -> const quickdraw::surface&;
        auto data() -> std::vector<char>;

    private:
        quickdraw::surface m_surface;

        auto decode(data::reader& reader) -> bool;
        auto merge_bytes(int position, const data::block& bytes, int offset, int size) -> void;

        auto encode(data::writer& writer) -> void;
    };
}
