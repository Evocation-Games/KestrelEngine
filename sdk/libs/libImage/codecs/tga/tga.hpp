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

#include <libImage/codecs/codec.hpp>

namespace image::codec
{
    struct tga: format
    {
        explicit tga(const std::string& path);
        explicit tga(const foundation::filesystem::path& path);
        explicit tga(const graphite::data::block& data);
        explicit tga(graphite::quickdraw::surface& surface);

        [[nodiscard]] auto byte_order() const -> graphite::data::byte_order override;

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

    private:
        auto decode(graphite::data::reader& reader) -> void override;
        auto encode(graphite::data::writer& writer) const -> void override;
        auto merge_bytes(std::int32_t position, const std::vector<char>& bytes, std::int32_t offset, std::size_t size) -> void;
    };
}