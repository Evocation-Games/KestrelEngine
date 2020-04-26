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

#if !defined(KESTREL_SPRITE_HPP)
#define KESTREL_SPRITE_HPP

#include <memory>
#include "foundation/math/rect.hpp"
#include "foundation/gl/spritesheet.hpp"

namespace kestrel { namespace gl {

    class spritesheet;

    struct sprite
    {
    private:
        math::rect m_frame;
        math::rect m_uv;
        int m_uv_index { 0 };

    public:
        sprite(const gl::spritesheet& sheet, const math::point& point, const math::size& size);
        static auto create(const gl::spritesheet& sheet, const math::point& point, const math::size& size) -> std::shared_ptr<sprite>;

        auto frame() const -> math::rect;
        auto uv() const -> math::rect;
        auto set_uv_index(const int index) -> void;

        auto draw(const math::point& p, const gl::spritesheet& sheet) const -> void;
        auto draw(const math::vector& v, const gl::spritesheet& sheet) const -> void;
    };

}};

#endif //KESTREL_SPRITE_HPP
