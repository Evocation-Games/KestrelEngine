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

#include <codecvt>
#include <algorithm>
#include <locale>
#include <cmath>
#include "core/graphics/common/text/typesetter.hpp"

// MARK: - Construction

graphics::typesetter::typesetter(const std::string &text)
    : m_base_font(std::make_shared<graphics::font>()),
      m_base_font_size(12),
      m_font_color(graphics::color::white_color()),
      m_max_size(9999), m_min_size(0),
      m_pos(0),
      m_line_height(0), m_buffer_width(0)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    m_text = convert.from_bytes(text);
}

// MARK: - Accessors

auto graphics::typesetter::set_margins(const math::size &margins) -> void
{
    m_max_size = margins;
}


auto graphics::typesetter::set_font(const std::string &font_name) -> void
{
    m_base_font = std::make_shared<graphics::font>(font_name);
}

auto graphics::typesetter::set_font_size(const int &size) -> void
{
    m_base_font_size = static_cast<unsigned int>(size);
}

auto graphics::typesetter::set_font_color(const graphics::color &color) -> void
{
    m_font_color = color;
}

auto graphics::typesetter::set_text(const std::string &text) -> void
{
    reset();
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    m_text = convert.from_bytes(text);
}

auto graphics::typesetter::get_bounding_size() const -> math::size
{
    return m_min_size;
}

// MARK: - Utilities

auto graphics::typesetter::reset() -> void
{
    m_layout.clear();
    m_buffer.clear();
    m_pos = math::point(0);
    m_buffer_width = 0;
    m_min_size = math::size(0);
}

auto graphics::typesetter::requires_layout() const -> bool
{
    return m_layout.empty();
}

// MARK: - Layout / Calculations / Metrics

auto graphics::typesetter::layout() -> void
{
    FT_Error error;
    FT_GlyphSlot slot = m_base_font->face()->glyph;

    // In the event that we're performing layout for a second, third, etc... time, then we need to remove the
    // existing layout information so that we can run the layout for a clean slate.
    reset();

    // Ensure that the character set and font size is correctly configured, otherwise the layout will be invalid.
    FT_Select_Charmap(m_base_font->face(), FT_ENCODING_UNICODE);
    FT_Set_Char_Size(m_base_font->face(), 0, ((m_base_font_size - 2) << 6U), 100, 100);

    // Prepare a buffer to keep track of character layouts, before committing them to the actual layout.
    std::vector<character> buffer;
    auto word_start = m_text.begin();
    FT_UInt previous_glyph_index = 0;

    // Loop over all of the characters in the text buffer, and attempt to add it to the layout. Special characters are
    // also considered, and update the layout accordingly, along with word breaks being calculated.
    for (auto i = word_start; i != m_text.end();) {
        // Handle special characters first of all... these primarily include each of the ASCII control codes, though
        // most are ignored.
        if (*i < 0x20 || *i == 0x7F) {
            if (*i == '\n') {
                // Newline
                commit_buffer();
                newline();
                word_start = ++i;
                continue;
            }
            else if (*i == '\t') {
                // Tab - this needs to be determined from a setting on the typesetter.
                advance(50);
            }
            else if (*i == ' ' && m_pos.x < 1) {
                ++i;
                continue;
            }

            ++i;
            continue;
        }

        // Fetch the current glyph, and calculate some basic metrics regarding it.
        FT_UInt glyph_index = FT_Get_Char_Index(m_base_font->face(), *i);
        math::size glyph_kerning { 0 };
        auto glyph_advance = m_base_font->calculate_glyph_width(glyph_index, previous_glyph_index, &glyph_kerning) + glyph_kerning.width;

        if (is_non_breaking(*i)) {
            // The current character is one that can not result in a line break. Check if the character extends beyond
            // the right margin. If it does then back track to the start of the current word, insert a new line and
            // restart.
            if ((m_pos.x + m_buffer_width + glyph_advance) >= m_max_size.width) {
                m_min_size.width = std::max(m_min_size.width, m_pos.x);
                newline();
                drop_buffer();
                i = word_start;
                continue;
            }

            // We still have space to place the glyph on the current line.
            else {
                place(*i);
                advance(glyph_advance, true);
                m_min_size.width = std::max(m_min_size.width, m_pos.x + m_buffer_width);
                ++i;
            }
        }
        else {
            // The current character is one that can break on to a new line if required.
            // The first thing to do here is to commit the current buffer, and then place the new character in the
            // newly emptied buffer.
            commit_buffer();
            auto c = i;
            word_start = ++i;

            // Are we going to overrun the boundary?
            if ((m_pos.x + glyph_advance) >= m_max_size.width - 2) {
                newline();
            }

            // Add the character directly to the layout.
            place(*c, true);
            advance(glyph_advance, false);
            m_min_size.width = std::max(m_min_size.width, m_pos.x);
        }

        previous_glyph_index = glyph_index;
    }

    // Ensure there is nothing remaining in the buffer, and if there is commit it.
    commit_buffer();
    m_min_size.height = m_pos.y + m_base_font->line_height() + 5; // TODO: Calculate an actual excess for the text.
    m_min_size.width = std::max(m_min_size.width, m_pos.x);
}

// MARK: - Rendering

auto graphics::typesetter::render() -> std::vector<graphics::color>
{
    FT_Error error;
    FT_GlyphSlot slot = m_base_font->face()->glyph;
    auto line_height = m_base_font->line_height();

    std::vector<graphics::color> buffer(static_cast<unsigned int>(m_min_size.width * m_min_size.height), graphics::color::clear_color());

    // Ensure that the character set and font size is correctly configured, otherwise the layout will be invalid.
    FT_Select_Charmap(m_base_font->face(), FT_ENCODING_UNICODE);
    FT_Set_Char_Size(m_base_font->face(), 0, ((m_base_font_size - 2) << 6U), 100, 100);

    for (const auto& ch : m_layout) {
        FT_UInt glyph_index = FT_Get_Char_Index(m_base_font->face(), ch.value);
        if (FT_Load_Glyph(m_base_font->face(), glyph_index, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT)) {
            continue;
        }

        if (FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL)) {
            continue;
        }

        auto y_offset = static_cast<int>(line_height - slot->bitmap_top);
        auto x_offset = static_cast<int>(slot->bitmap_left);

        for (auto yy = 0; yy < slot->bitmap.rows; ++yy) {
            for (auto xx = 0; xx < slot->bitmap.width; ++xx) {
                auto alpha = slot->bitmap.buffer[(yy * slot->bitmap.width) + xx];
                alpha = alpha > 0 ? std::min(255U, static_cast<unsigned int>(alpha) + 64) : alpha;
                auto hex_color = static_cast<unsigned int>(m_font_color.value() & 0x00FFFFFFU);
                auto color = hex_color | (alpha << 24U); // Color of the glyph becomes the alpha for the text.
                auto offset = ((static_cast<int>(std::round(ch.y)) + y_offset + yy) * static_cast<int>(std::round(m_min_size.width))) + static_cast<int>(std::round(ch.x)) + x_offset + xx;
                if (offset < buffer.size()) {
                    buffer[offset] = graphics::color::color_value(color);
                }
            }
        }
    }

    return buffer;
}

// MARK: - Working Buffer

auto graphics::typesetter::commit_buffer() -> void
{
    m_layout.insert(m_layout.end(), m_buffer.begin(), m_buffer.end());
    advance(m_buffer_width, false);
    drop_buffer();
}

auto graphics::typesetter::drop_buffer() -> void
{
    m_buffer.clear();
    m_buffer_width = 0;
}

auto graphics::typesetter::newline() -> void
{
    m_pos.y += static_cast<int>(std::round(m_base_font->line_height()));
    m_pos.x = 0;
}

auto graphics::typesetter::advance(const double& d, const bool& buffer) -> void
{
    if (buffer) {
        m_buffer_width += std::round(d);
    }
    else {
        m_pos.x += std::round(d);
    }
}

auto graphics::typesetter::place(const wchar_t& c, const bool& in_buffer) -> void
{
    typesetter::character ch { 0 };
    ch.value = c;
    ch.x = std::round(m_pos.x + m_buffer_width);
    ch.y = std::round(m_pos.y);
    if (in_buffer) {
        m_buffer.emplace_back(ch);
    }
    else {
        m_layout.emplace_back(ch);
    }
}

// MARK: - Support

auto graphics::typesetter::is_non_breaking(const wchar_t &c) -> bool
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}