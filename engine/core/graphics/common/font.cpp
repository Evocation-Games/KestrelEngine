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

#include <stdexcept>
#include <complex>
#include <codecvt>
#include "core/graphics/common/font.hpp"

#if __APPLE__
#include "core/support/macos/cocoa/font.h"
#elif __linux__
#include "core/support/linux/font_config.hpp"
#elif (_WIN32 || _WIN64)
#include "core/support/windows/fonts.hpp"
#endif

// MARK: - FreeType Globals

static bool ft_loaded = false;
static FT_Library ft;

// MARK: - Construction

graphics::font::font(const std::string& name)
{
#if __APPLE__
    m_path = cocoa::font::path_for(name);
#elif __linux__
    m_path = linux_os::font_config::path_for_best_fit_font(name);
#else
    m_path = "C:/Windows/Fonts/Arial.ttf";
#endif

    if (!ft_loaded) {
        if (FT_Init_FreeType(&ft)) {
            throw std::logic_error("Failed to initialise FreeType");
        }
        ft_loaded = true;
        FT_Library_SetLcdFilter(ft, FT_LCD_FILTER_NONE);
    }

    if (FT_New_Face(ft, m_path.c_str(), 0, &m_face)) {
        throw std::logic_error("Failed to load font face.");
    }
}

// MARK: - Destruction

graphics::font::~font()
{
    if (m_face) {
        FT_Done_Face(m_face);
    }
}

// MARK: - Accessors

auto graphics::font::face() const -> FT_Face
{
    return m_face;
}

// MARK: - Metrics

auto graphics::font::calculate_glyph_width(const FT_UInt &glyph_index, const FT_UInt &previous_glyph_index,
                                           math::size *kerning) const -> double
{
    FT_Error error = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        return 0;
    }

    auto advance_x = (static_cast<unsigned int>(m_face->glyph->advance.x) >> 6U);

    if (kerning) {
        if (previous_glyph_index && glyph_index) {
            FT_Vector delta;
            FT_Get_Kerning(m_face, previous_glyph_index, glyph_index, FT_KERNING_DEFAULT, &delta);
            *kerning = math::size((static_cast<unsigned int>(delta.x) >> 6U), (static_cast<unsigned int>(delta.y) >> 6U));

            // TODO: Resolve the kerning issue, not just hack it like this.
            if (kerning->width > 50 || kerning->height > 50) {
                *kerning = math::size(0);
            }
        }
        return advance_x + kerning->width;
    }
    else {
        return advance_x;
    }
}

auto graphics::font::line_height() const -> double
{
    return static_cast<double>(static_cast<unsigned int>(m_face->size->metrics.height) >> 6U);
}
