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
#endif

// MARK: - FreeType Globals

static bool ft_loaded = false;
static FT_Library ft;

// MARK: - Construction

graphics::font::font(const std::string& name)
{
#if __APPLE__
    m_path = cocoa::font::path_for(name);
#else

#endif

    if (!ft_loaded) {
        if (FT_Init_FreeType(&ft)) {
            throw std::logic_error("Failed to initialise FreeType");
        }
        ft_loaded = true;
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

// MARK: - Sema

static inline auto is_non_breaking(const wchar_t& c) -> bool
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// MARK: - Text Rendering

static inline auto calculate_glyph_width(FT_Face face, FT_UInt glyph_index, FT_UInt previous_glyph, unsigned int* kern_x) -> unsigned int
{
    FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error) {
        return 0;
    }

    auto advance_x = (static_cast<unsigned int>(face->glyph->advance.x) >> 6U);
    if (previous_glyph && glyph_index) {
        FT_Vector delta;
        FT_Get_Kerning(face, previous_glyph, glyph_index, FT_KERNING_DEFAULT, &delta);
        *kern_x = (static_cast<unsigned int>(delta.x) >> 6U);
    }

    return advance_x + *kern_x;
}

auto graphics::font::layout_text(const std::string &text, const int& font_size) -> math::size
{
    return layout_text_with_bounds(text, font_size, { 9999, 9999 });
}

auto graphics::font::layout_text_with_bounds(const std::string &text, const int& font_size, const math::size& max) -> math::size
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

    // Reset the layout of the text...
    m_rendered_text = convert.from_bytes(text);
    m_rendered_font_size = font_size;
    m_layout.erase(m_layout.begin(), m_layout.end());

    // Layout the text, and keep track of the determined layout for future rendering
    FT_GlyphSlot slot = m_face->glyph;
    FT_Error error;

    FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
    FT_Set_Char_Size(m_face, 0, static_cast<unsigned int>(font_size) << 6U, 72, 72);
    auto line_height = ((m_face->size->metrics.ascender - m_face->size->metrics.descender) >> 6);
    auto word_start = m_rendered_text.begin();
    auto word_width = 0;
    FT_UInt previous_glyph = 0;
    auto pen_x = 0;
    auto pen_y = 0;
    auto required_width = 0;
    auto required_height = 0;

    // Search forwards, looking for the end of the word, and calculating the width of the word. We need
    // to determine if it is going to exceed the maximum width of the text. If it does, then go to the next
    // line and draw it there.
    word_width = 0;
    std::vector<std::tuple<wchar_t, math::point>> word_layout;
    for (auto i = m_rendered_text.begin(); i != m_rendered_text.end();) {
        // Check for special characters and handle them.
        if (*i == '\n') {
            // Moving to a new line, commit what we currently have...
            if (!word_layout.empty()) {
                m_layout.insert(m_layout.end(), word_layout.begin(), word_layout.end());
                word_layout.erase(word_layout.begin(), word_layout.end());
                word_width = 0;
            }

            pen_y += line_height;
            pen_x = 0;
            word_start = ++i;
            continue;
        }
        else if (*i < 0x20 || *i == 0x7F) {
            ++i;
            continue;
        }

        FT_UInt glyph_index = FT_Get_Char_Index(m_face, *i);

        // Fetch the dimensions of the character.
        unsigned int kern_x = 0;
        unsigned int advance_x = calculate_glyph_width(m_face, glyph_index, previous_glyph, &kern_x);

        // Are we going beyond the end of the bounds?
        if ((pen_x + word_width + advance_x) >= max.width) {
            if (is_non_breaking(*i)) {
                // The character is a none breaking one, and thus we need to wrap the entire word to the next
                // line. Erase the current calculation for the word, and then restart it on a new line.
                word_layout.erase(word_layout.begin(), word_layout.end());
                required_width = std::min(required_width, pen_x);
                pen_x = 0;
                pen_y += line_height;
                word_width = 0;

                // Move back to the beginning of the word, and restart the search.
                i = word_start;
                continue;
            }
            else {
                // The character is a breaking one. Commit the current progress and proceed to a new line, and then
                // exit this search and prepare for a new one.
                required_width = std::min(required_width, pen_x + word_width);
                m_layout.insert(m_layout.end(), word_layout.begin(), word_layout.end());
                word_layout.erase(word_layout.begin(), word_layout.end());
                pen_x = 0;
                pen_y += line_height;
                word_width = 0;
                word_start = i;
                word_layout.emplace_back(std::make_tuple(*i, math::point(pen_x + word_width, pen_y)));
            }
        }
        else {
            if (is_non_breaking(*i)) {
                word_layout.emplace_back(std::make_tuple(*i, math::point(pen_x + word_width, pen_y)));
                word_width += advance_x;
                required_width = std::max(required_width, pen_x + word_width);
            }
            else {
                m_layout.insert(m_layout.end(), word_layout.begin(), word_layout.end());
                word_layout.erase(word_layout.begin(), word_layout.end());
                m_layout.emplace_back(std::make_tuple(*i, math::point(pen_x + word_width, pen_y)));
                pen_x += word_width + advance_x;
                word_width = 0;
                word_start = i;
                required_width = std::max(required_width, pen_x + word_width);
            }
        }

        // Update which glyph was previously looked at.
        previous_glyph = glyph_index;
        ++i;
    }

    if (!word_layout.empty()) {
        m_layout.insert(m_layout.end(), word_layout.begin(), word_layout.end());
    }

    m_rendered_text_size = math::size(required_width, static_cast<double>(pen_y) + line_height);
    return m_rendered_text_size;
}

auto graphics::font::render_text(const graphics::color& color) const -> std::vector<uint32_t>
{
    std::vector<uint32_t> text_data(static_cast<int>(m_rendered_text_size.width * m_rendered_text_size.height), 0x00000000);

    FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
    FT_Set_Char_Size(m_face, 0, m_rendered_font_size << 6, 72, 72);
    auto line_height = (m_face->size->metrics.ascender - m_face->size->metrics.descender) >> 6;

    for (const auto &i : m_layout) {
        auto c = std::get<0>(i);
        auto pos = std::get<1>(i);

        double x = pos.x;
        double y = pos.y;

        FT_UInt glyph_index = FT_Get_Char_Index(m_face, c);
        if (FT_Load_Glyph(m_face, glyph_index, FT_LOAD_RENDER)) {
            continue;
        }

        // Draw the character data into the output data.
        auto y_offset = line_height - m_face->glyph->bitmap_top;
        auto x_offset = m_face->glyph->bitmap_left;
        for (auto yy = 0; yy < m_face->glyph->bitmap.rows; ++yy) {
            for (auto xx = 0; xx < m_face->glyph->bitmap.width; ++xx) {
                auto alpha = m_face->glyph->bitmap.buffer[(yy * m_face->glyph->bitmap.width) + xx];
                auto hex_color = color.value() & 0x00FFFFFF;
                auto color = hex_color | (alpha << 24); // Color of the glyph becomes the alpha for the text.
                auto offset = ((y + y_offset + yy) * m_rendered_text_size.width) + x + x_offset + xx;
                if (offset < text_data.size()) {
                    text_data[offset] = color;
                }
            }
        }

    }

    return text_data;
}

auto graphics::font::requires_layout() const -> bool
{
    return m_rendered_text.empty();
}

auto graphics::font::clear() -> void
{
    m_rendered_text.clear();
    m_layout.erase(m_layout.begin(), m_layout.end());
}
