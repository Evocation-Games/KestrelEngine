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

#if !defined(KESTREL_TYPESETTER_HPP)
#define KESTREL_TYPESETTER_HPP

#include <type_traits>
#include <memory>
#include <string>
#include "scripting/state.hpp"
#include "core/graphics/common/font.hpp"
#include "core/graphics/common/color.hpp"
#include "math/size.hpp"

namespace graphics
{

    class typesetter: public std::enable_shared_from_this<graphics::typesetter>, public scripting::lua::object
    {
    public:
        struct character
        {
            wchar_t value;
            bool bold;
            bool italic;
            double x;
            double y;
        };

    private:
        double m_scale { 1.0 };
        int m_dpi { 100 };
        std::wstring m_text;
        std::shared_ptr<graphics::font> m_base_font;
        unsigned int m_base_font_size;
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
        auto advance(const double& d, const bool& buffer = false) -> void;
        auto place(const wchar_t& c, const bool& buffer = true) -> void;

        static auto is_non_breaking(const wchar_t& c) -> bool;

    public:
        lua_api explicit typesetter(const std::string& text, const double& scale = 1.0);

        lua_api auto set_margins(const math::size& margins) -> void;
        lua_api auto set_font(const std::string& font_name) -> void;
        lua_api auto set_font_size(const int& size) -> void;
        lua_api auto set_font_color(const graphics::color& color) -> void;

        [[nodiscard]] auto font() const -> std::shared_ptr<graphics::font>;

        lua_api auto set_text(const std::string& text) -> void;

        lua_api auto get_bounding_size() const -> math::size;

        lua_api auto requires_layout() const -> bool;

        lua_api auto reset() -> void;

        lua_api auto layout() -> void;
        auto render() -> std::vector<graphics::color>;
    };

}

#endif //KESTREL_TYPESETTER_HPP
