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

#pragma once

#include <unordered_map>
#include <libKestrel/font/font.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::font
{
    class lua_api(FontManager, Available_0_8) lua_use_namespace manager
    {
    public:
        has_constructable_lua_api(manager);

        manager(const manager&) = delete;
        manager& operator=(const manager&) = delete;
        manager(manager&&) = delete;
        manager& operator=(manager&&) = delete;

        static auto shared_manager() -> manager&;

        auto load_all_fonts_for_imgui() -> void;
        auto unload_all_imgui_fonts() -> void;

        [[nodiscard]] auto default_font() const -> reference::lua_reference { return m_default_font; }
        auto set_default_font(const reference::lua_reference& font) -> void;

        auto add_font(const reference::lua_reference& font) -> reference::lua_reference;
        [[nodiscard]] auto get_font(const std::string& name, std::uint32_t size) -> reference::lua_reference;

    private:
        std::unordered_map<std::string, reference::lua_reference> m_fonts;
        reference::lua_reference m_default_font { nullptr };

        manager() = default;

        static inline auto font_hash(const reference::lua_reference& font) -> std::string;
        static inline auto font_hash(const std::string& face, std::uint32_t size) -> std::string;

        lua_function(loadAllFontsForImGui, Available_0_8) static auto lua_load_all_fonts_for_imgui() -> void
        {
            shared_manager().load_all_fonts_for_imgui();
        }

        lua_function(unloadAllFontsForImGui, Available_0_8) static auto lua_unload_all_imgui_fonts() -> void
        {
            shared_manager().unload_all_imgui_fonts();
        }

        lua_getter(defaultFont, Available_0_8) [[nodiscard]] static auto lua_default_font() -> reference::lua_reference
        {
            return shared_manager().default_font();
        }

        lua_setter(defaultFont, Available_0_8) static auto lua_set_default_font(reference::lua_reference font) -> void
        {
            shared_manager().set_default_font(font);
        }

        lua_function(addFont, Available_0_8) static auto lua_add_font(const reference::lua_reference& font) -> reference::lua_reference
        {
            return shared_manager().add_font(font);
        }

        lua_function(getFont, Available_0_8) [[nodiscard]] static auto lua_get_font(const std::string& name, std::uint32_t size) -> reference::lua_reference
        {
            return shared_manager().get_font(name, size);
        }

    };
}

