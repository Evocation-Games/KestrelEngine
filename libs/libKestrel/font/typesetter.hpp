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

#include <type_traits>
#include <memory>
#include <string>
#include <libKestrel/font/font.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/graphics/common/font.hpp>

namespace kestrel::font
{
    class typesetter: public std::enable_shared_from_this<typesetter>
    {
    public:
        struct character
        {
            wchar_t value;
            bool bold;
            bool italic;
            double x;
            double y;
            double w;
        };

        explicit typesetter(const std::string& text, double scale = 1.0);

        auto set_margins(const math::size& margins) -> void;
        auto set_font(const kestrel::font::reference& font) -> void;
        auto set_font_color(const graphics::color& color) -> void;

        [[nodiscard]] auto font() const -> std::shared_ptr<graphics::font>;

        auto set_text(const std::string& text) -> void;

        auto get_bounding_size() const -> math::size;
        auto get_point_at_location(std::uint32_t location) const -> math::point;

        auto requires_layout() const -> bool;

        auto reset() -> void;

        auto layout() -> void;
        auto render() -> std::vector<graphics::color>;

    private:
        double m_scale { 1.0 };
        std::uint16_t m_dpi { 72 };
        std::wstring m_text;
        std::shared_ptr<graphics::font> m_base_font;
        graphics::color m_font_color;
        std::vector<character> m_layout;
        std::vector<character> m_buffer;
        math::size m_max_size;
        math::size m_min_size;
        math::point m_pos;
        double m_buffer_width;
        double m_line_height;

        auto commit_buffer() -> void;
        auto drop_buffer() -> void;
        auto newline() -> void;
        auto advance(double d, bool buffer = false) -> void;
        auto place(wchar_t c, bool buffer = true, double cw = 0) -> void;

        static auto is_non_breaking(wchar_t c) -> bool;
    };
}

