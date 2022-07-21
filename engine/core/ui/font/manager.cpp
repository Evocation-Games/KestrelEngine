// Copyright (c) 2022 Tom Hancocks
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

#include "core/ui/font/manager.hpp"

// MARK: - Lua

auto ui::font::manager::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("FontManager")
            .addProperty("defaultFont", &manager::lua_default_font, &manager::lua_set_default_font)
            .addFunction("loadAllFontsForImGui", &manager::lua_load_all_fonts_for_imgui)
            .addFunction("unloadAllImGuiFonts", &manager::lua_unload_all_imgui_fonts)
            .addFunction("addFont", &manager::lua_add_font)
            .addFunction("getFont", &manager::lua_get_font)
        .endNamespace();
}

// MARK: - Construction

auto ui::font::manager::shared_manager() -> manager&
{
    static manager manager;
    return manager;
}

// MARK: - Hashing

auto ui::font::manager::font_hash(const reference::lua_reference &font) -> std::string
{
    return font_hash(font->font_face(), font->font_size());
}

auto ui::font::manager::font_hash(const std::string &face, uint32_t size) -> std::string
{
    return { face + "-" + std::to_string(size) };
}

// MARK: - Font Management

auto ui::font::manager::load_all_fonts_for_imgui() -> void
{
    for (auto it : m_fonts) {
        it.second->load_for_imgui();
    }
    ImGui::GetIO().Fonts->Build();
}

auto ui::font::manager::unload_all_imgui_fonts() -> void
{
    for (auto it : m_fonts) {
        it.second->unload_for_imgui();
    }
}

auto ui::font::manager::add_font(const reference::lua_reference &font) -> reference::lua_reference
{
    auto hash = font_hash(font);
    auto it = m_fonts.find(hash);

    if (it != m_fonts.end()) {
        it->second->unload();
        it->second = font;
    }
    else {
        m_fonts[hash] = font;
    }

    return font;
}

auto ui::font::manager::get_font(const std::string &name, uint32_t size) -> reference::lua_reference
{
    auto it = m_fonts.find(font_hash(name, size));
    if (it != m_fonts.end()) {
        return it->second;
    }

    return add_font(reference::lua_reference(new reference(name, size)));
}

