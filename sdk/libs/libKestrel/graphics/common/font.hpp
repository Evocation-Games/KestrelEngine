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

#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H
#include FT_ADVANCES_H
#include FT_BITMAP_H
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libData/block.hpp>

namespace kestrel::graphics
{
    class font: public std::enable_shared_from_this<graphics::font>
    {
    public:
        explicit font(const std::string& name, std::int16_t size, bool load_font = true);
        ~font();

        [[nodiscard]] auto face() const -> FT_Face;
        [[nodiscard]] auto path() const -> std::string;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto size() const -> std::int16_t;

        [[nodiscard]] auto calculate_glyph_width(const FT_UInt& glyph_index, const FT_UInt& previous_glyph_index, math::size *kerning) const -> double;
        [[nodiscard]] auto line_height() const -> double;

        static auto font_name_at_path(const std::string& path) -> std::string;

        static auto library() -> FT_Library;

    private:
        std::string m_path;
        std::int16_t m_size { 11 };
        FT_Face m_face;
    };
}
