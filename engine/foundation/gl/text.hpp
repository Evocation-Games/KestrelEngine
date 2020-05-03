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

#if !defined(KESTREL_TEXT_HPP)
#define KESTREL_TEXT_HPP


#include <string>
#include <memory>
#include "foundation/math/size.hpp"
#include "foundation/gl/spritesheet.hpp"
#include "libDiamane/graphics/color.hpp"

namespace kestrel { namespace gl {

    struct text : public lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<gl::text> lua_reference;

    private:
        std::shared_ptr<gl::spritesheet> m_spritesheet { nullptr };
        std::string m_text { "" };
        std::string m_font { "" };
        int m_font_size;
        diamane::gl::color m_color { diamane::gl::color::whiteColor() };

    public:
        static auto register_object() -> void;

        text(const std::string& s, const std::string& font, int size);

        auto lua_draw(math::point::lua_reference position) const -> void;
        auto draw(const math::vector& v) const -> void;
    };

}};

#endif //KESTREL_TEXT_HPP
