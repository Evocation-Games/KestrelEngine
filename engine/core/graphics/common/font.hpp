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

#if !defined(KESTREL_FONT_HPP)
#define KESTREL_FONT_HPP

#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H
#include FT_ADVANCES_H
#include "math/size.hpp"
#include "math/point.hpp"
#include "core/graphics/common/color.hpp"

namespace graphics
{

    class font: public std::enable_shared_from_this<graphics::font>
    {
    private:
        std::string m_path;
        FT_Face m_face;

    public:
        explicit font(const std::string& name = "");
        ~font();

        auto face() const -> FT_Face;

        auto calculate_glyph_width(const FT_UInt& glyph_index, const FT_UInt& previous_glyph_index, math::size *kerning) const -> double;
        auto line_height() const -> double;

        static auto font_name_at_path(const std::string& path) -> std::string;
    };

}


#endif //KESTREL_FONT_HPP
