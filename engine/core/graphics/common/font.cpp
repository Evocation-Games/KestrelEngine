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
#include "core/graphics/common/font.hpp"

// MARK: - FreeType Globals

static bool ft_loaded = false;
static FT_Library ft;

// MARK: - Construction

graphics::font::font(const std::string& path)
    : m_path(path)
{
    if (!ft_loaded) {
        if (FT_Init_FreeType(&ft)) {
            throw std::logic_error("Failed to initialise FreeType");
        }
    }

    if (FT_New_Face(ft, m_path.c_str(), 0, &m_face)) {
        throw std::logic_error("Failed to load font face.");
    }
}

// MARK: - Text Rendering

auto graphics::font::text_size(const std::string &text, const int& font_size) const -> math::size
{
    FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
    FT_Set_Char_Size(m_face, 0, font_size << 6, 72, 72);
    auto line_height = (m_face->size->metrics.ascender - m_face->size->metrics.descender) >> 6;
    auto tab_size = 20;

    math::size required_size { 0, line_height };
    double line_width = 0.0;
    for (auto c = text.begin(); c != text.end(); ++c) {
        if (FT_Load_Char(m_face, *c, FT_LOAD_ADVANCE_ONLY)) {
            continue;
        }

        // Handle some special characters such as newline so that we are getting the correct/expected behaviour.
        if (*c == '\n') {
            required_size.set_height(required_size.height + line_height);
            line_width = 0;
            continue;
        }
        else if (*c == '\t') {
            auto tab_advance = line_width + tab_size;
            line_width += tab_advance - std::fmod(tab_advance, tab_size);
            required_size.set_width(std::max(line_width, required_size.width));
            continue;
        }
        else if (*c < 0x20 || *c == 0x7F) {
            // Unprintable characters, ignore them.
            continue;
        }

        // Look up the character and determine what it's dimensions are.
        line_width += m_face->glyph->advance.x >> 6;
        required_size.set_width(std::max(line_width, required_size.width));
    }
    return required_size;
}

auto graphics::font::render_text(const std::string &text, const math::size& sz, const int &font_size, const graphics::color& color) const -> std::vector<uint32_t>
{
    std::vector<uint32_t> text_data(static_cast<int>(sz.width * sz.height), 0x00000000);

    FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
    FT_Set_Char_Size(m_face, 0, font_size << 6, 72, 72);
    auto line_height = (m_face->size->metrics.ascender - m_face->size->metrics.descender) >> 6;
    auto tab_size = 20;
    double x = 0;
    double y = 0;

    for (auto c = text.begin(); c != text.end(); ++c) {
        if (FT_Load_Char(m_face, *c, FT_LOAD_RENDER)) {
            continue;
        }

        // Handle some special characters such as newline so that we are getting the correct/expected behaviour.
        if (*c == '\n') {
            y += line_height;
            x = 0;
            continue;
        }
        else if (*c == '\t') {
            auto tab_advance = x + tab_size;
            x += tab_advance - std::fmod(tab_advance, tab_size);
            continue;
        }
        else if (*c < 0x20 || *c == 0x7F) {
            // Unprintable characters, ignore them.
            continue;
        }

        // Draw the character data into the output data.
        auto y_offset = line_height - m_face->glyph->bitmap_top;
        auto x_offset = m_face->glyph->bitmap_left;
        for (auto yy = 0; yy < m_face->glyph->bitmap.rows; ++yy) {
            for (auto xx = 0; xx < m_face->glyph->bitmap.width; ++xx) {
                auto red = m_face->glyph->bitmap.buffer[(yy * m_face->glyph->bitmap.width) + xx];
                auto hex_color = color.value() & 0x00FFFFFF;
                auto color = hex_color | (red << 24); // Color of the glyph becomes the alpha for the text.
                text_data[((y + y_offset + yy) * sz.width) + x + x_offset + xx] = color;
            }
        }

        // Look up the character and determine what it's dimensions are.
        x += m_face->glyph->advance.x >> 6;

    }

    return text_data;
}
