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
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
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

        std::wstring m_rendered_text { L"" };
        int m_rendered_font_size { 12 };
        std::vector<std::tuple<wchar_t, math::point>> m_layout;
        math::size m_rendered_text_size { 0, 0 };

    public:
        explicit font(const std::string& name = "");
        ~font();

        auto requires_layout() const -> bool;

        auto layout_text(const std::string& text, const int& font_size = 12) -> math::size;
        auto layout_text_with_bounds(const std::string &text, const int& font_size, const math::size& max) -> math::size;
        auto render_text(const graphics::color& color = graphics::color::white_color()) const -> std::vector<uint32_t>;

        auto clear() -> void;
    };

}


#endif //KESTREL_FONT_HPP
