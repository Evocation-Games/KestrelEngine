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
            .addConstructor<auto(*)(std::string, std::string, int, graphics::color::lua_reference)->void, graphics::text::lua_reference>()
            .addProperty("fontSize", &text::get_font_size)
            .addProperty("font", &text::get_font)
            .addProperty("color", &text::get_color)
            .addFunction("spawnEntity", &text::spawn_entity)
        .endClass();
}

// MARK: - Construction

graphics::text::text(std::string text, std::string font, int size, const graphics::color::lua_reference& color)
    : m_text(std::move(text)), m_font_face(std::move(font)), m_font_size(size), m_color(*color.get())
{
    // TODO: Correctly load fonts for platforms
#if __APPLE__
    m_font = std::make_shared<graphics::font>(m_font_face);
#elif __linux__
    m_font = std::make_shared<graphics::font>("Ubuntu-R");
#else
#   error Fix for Windows
#endif
}

// MARK: - Accessors

auto graphics::text::get_value() const -> std::string
{
    return m_text;
}

auto graphics::text::get_font_size() const -> int
{
    return m_font_size;
}

auto graphics::text::get_font() const -> std::string
{
    return m_font_face;
}

auto graphics::text::get_color() const -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(m_color));
}


// MARK: - Entity

auto graphics::text::spawn_entity(const math::vector &position) -> graphics::entity::lua_reference
{
    // Create a new bitmap of the text.
    auto size = m_font->layout_text(m_text, m_font_size);
    auto bmp = m_font->render_text(m_color);

    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, std::move(bmp));

        auto entity = graphics::entity::lua_reference(new graphics::entity(size));
        entity->set_spritesheet(std::make_shared<graphics::spritesheet>(tex, size));
        entity->set_position(position);

        return entity;
    }

    return nullptr;
}