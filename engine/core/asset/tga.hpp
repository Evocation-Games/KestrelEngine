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
#include <libGraphite/quickdraw/support/surface.hpp>
#include <libGraphite/data/data.hpp>

namespace asset
{

    class tga
    {
    protected:

        struct header
        {
            uint8_t id_length;
            uint8_t color_map_type;
            uint8_t data_type_code;
            uint16_t color_map_origin;
            uint16_t color_map_length;
            uint8_t color_map_depth;
            uint16_t x_origin;
            uint16_t y_origin;
            uint16_t width;
            uint16_t height;
            uint8_t bits_per_pixel;
            uint8_t image_descriptor;
        };

        struct pixel
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

    public:
        explicit tga(const std::string& path);
        explicit tga(const graphite::data::block& data);
        explicit tga(graphite::quickdraw::surface& surface);

        [[nodiscard]] auto surface() const -> const graphite::quickdraw::surface&;
        auto data() -> std::vector<char>;

    private:
        graphite::quickdraw::surface m_surface;

        auto decode(graphite::data::reader& reader) -> bool;
        auto merge_bytes(int position, const graphite::data::block& bytes, int offset, int size) -> void;

        auto encode(graphite::data::writer& writer) -> void;
    };

}
