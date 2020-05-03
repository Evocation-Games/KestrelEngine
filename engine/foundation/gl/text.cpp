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

#include "text.hpp"

// MARK: - Lua Bridge

auto kestrel::gl::text::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<gl::text>("Text")
            .addConstructor<auto(*)(const std::string&, const std::string&, int)->void, text::lua_reference>()
            .addFunction("draw", &text::lua_draw)
        .endClass();
}

// MARK: - Construction

kestrel::gl::text::text(const std::string &s, const std::string &font, int size)
    : m_text(s), m_font(font), m_font_size(size)
{
    auto texture = diamane::gl::texture::text(s, font, size, m_color);
    m_spritesheet = gl::spritesheet::create(texture, texture->size(), 1);
}

// MARK: - Drawing

auto kestrel::gl::text::lua_draw(math::point::lua_reference position) const -> void
{
    draw(position->vector());
}

auto kestrel::gl::text::draw(const kestrel::math::vector &v) const -> void
{
    if (m_spritesheet) {
        if (auto sprite = m_spritesheet->sprite(0).lock()) {
            sprite->draw(v, *m_spritesheet);
        }
    }
}

