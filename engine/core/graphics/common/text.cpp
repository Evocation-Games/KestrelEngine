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

#include "core/environment.hpp"
#include "renderer/common/renderer.hpp"
#include "core/graphics/common/text.hpp"
#include "core/graphics/common/text/typesetter.hpp"
#include <libGraphite/data/writer.hpp>

// MARK: - Lua

auto graphics::text::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<graphics::text>("Text")
            .addConstructor<auto(*)(std::string, const ui::font::reference::lua_reference&, graphics::color::lua_reference)->void, graphics::text::lua_reference>()
            .addProperty("font", &text::get_font)
            .addProperty("color", &text::get_color)
            .addFunction("spawnEntity", &text::spawn_entity)
        .endClass();
}

// MARK: - Construction

graphics::text::text(std::string text, const ui::font::reference::lua_reference& font, const graphics::color::lua_reference& color)
    : m_text(std::move(text)), m_font(font), m_color(*color.get())
{
    m_font->load_for_graphics();
}

// MARK: - Accessors

auto graphics::text::get_value() const -> std::string
{
    return m_text;
}

auto graphics::text::get_font() const -> ui::font::reference::lua_reference
{
    return m_font;
}

auto graphics::text::get_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}


// MARK: - Entity

auto graphics::text::spawn_entity(const math::point &position) -> std::shared_ptr<graphics::entity>
{
    // Create a typesetter to layout the text and generate a bitmap.
    graphics::typesetter ts(m_text);
    ts.set_font(*m_font.get());
    ts.set_font_color(m_color);
    ts.layout();

    auto size = ts.get_bounding_size();

    graphite::data::block bmp_data(size.width * size.height * 4);
    graphite::data::writer bmp(&bmp_data);
    for (const auto& c : ts.render()) {
        bmp.write_long(c.value());
    }

    auto tex = renderer::create_texture(size, *bmp.data());
    auto entity = std::make_shared<graphics::entity>(size);
    entity->set_sprite_sheet(std::make_shared<graphics::spritesheet>(tex, size));
    entity->set_position(position);

    return entity;
}