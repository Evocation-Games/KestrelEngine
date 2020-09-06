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

#include "core/graphics/common/text.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto graphics::text::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::text>("Text")
            .addConstructor<auto(*)(const std::string&)->void, graphics::text::lua_reference>()
            .addProperty("fontSize", &text::get_font_size, &text::set_font_size)
            .addProperty("font", &text::get_font, &text::set_font)
            .addProperty("color", &text::get_color, &text::set_color)
            .addProperty("position", &text::get_position, &text::set_position)
            .addFunction("draw", &text::draw)
        .endClass();
}

// MARK: - Construction

graphics::text::text(const std::string &text)
    : m_text(text), m_font_face("Geneva"), m_font_size(12)
{
    // TODO: Correctly load fonts for platforms
#if __APPLE__
    m_font = std::make_shared<graphics::font>("/System/Library/Fonts/SFCompactDisplay.ttf");
#elif __linux__
    m_font = std::make_shared<graphics::font>("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf");
#else
#   error Fix for Windows
#endif
}

// MARK: - Accessors

auto graphics::text::get_value() const -> std::string
{
    return m_text;
}

auto graphics::text::set_value(const std::string& value) -> void
{
    m_text = value;
    update_entity();
}

auto graphics::text::get_font_size() const -> int
{
    return m_font_size;
}

auto graphics::text::set_font_size(const int &size) -> void
{
    m_font_size = size;
}

auto graphics::text::get_font() const -> std::string
{
    return m_font_face;
}

auto graphics::text::set_font(const std::string &font) -> void
{
    m_font_face = font;
    // TODO: Update the font
    update_entity();
}

auto graphics::text::get_color() const -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(m_color));
}

auto graphics::text::set_color(graphics::color::lua_reference color) -> void
{
    m_color = *color.get();
    update_entity();
}

auto graphics::text::get_position() const -> math::point::lua_reference
{
    return math::point::lua_reference(new math::point(m_position.x, m_position.y));
}

auto graphics::text::set_position(math::point::lua_reference position) -> void
{
    m_position.x = position->x;
    m_position.y = position->y;
    update_entity();
}

// MARK: - Drawing

auto graphics::text::draw() -> void
{
    m_entity->draw();
}

auto graphics::text::update_entity() -> void
{
    if (auto env = environment::active_environment().lock()) {
        auto size = m_font->text_size(m_text, m_font_size);
        auto bmp = m_font->render_text(m_text, size, m_font_size, m_color);
        auto tex = env->create_texture(size, std::move(bmp));

        m_entity = graphics::entity::lua_reference (new graphics::entity(size));
        m_entity->position = math::vector(m_position.x + (size.width / 2.0), m_position.y + (size.height / 2.0));
        m_entity->set_spritesheet(std::make_shared<graphics::spritesheet>(tex, size));
    }
}
