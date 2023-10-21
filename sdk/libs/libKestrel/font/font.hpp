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

#include <functional>
#include <imgui/imgui.h>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/graphics/common/font.hpp>

namespace kestrel::font
{
    struct lua_api(Font, Available_0_8) reference
    {
    public:
        has_constructable_lua_api(reference);

        reference() = default;
        lua_constructor(Available_0_8) explicit reference(const std::string& name, std::uint32_t size);

        lua_function(namedFont, Available_0_8) static auto named_font(const std::string& name, std::uint32_t size) -> lua_reference;

        lua_getter(lineHeight, Available_0_8) [[nodiscard]] auto line_height() const -> std::uint32_t;

        lua_getter(fontSize, Available_0_8) [[nodiscard]] inline auto font_size() const -> std::uint32_t { return m_font_size; }
        lua_getter(fontFace, Available_0_8) [[nodiscard]] inline auto font_face() const -> std::string { return m_font_face; }
        lua_getter(path, Available_0_8) [[nodiscard]] inline auto path() const -> std::string { return m_path; }
        [[nodiscard]] inline auto imgui_font() const -> ImFont * { return m_instances.imgui; }
        [[nodiscard]] inline auto graphics_font() const -> std::shared_ptr<graphics::font> { return m_instances.graphics; }

        lua_function(loadFont, Available_0_8) auto load() -> void;
        lua_function(unloadFont, Available_0_8) auto unload() -> void;

        lua_function(loadForImGui, Available_0_8) auto load_for_imgui() -> void;
        lua_function(unloadForImGui, Available_0_8) auto unload_for_imgui() -> void;

        lua_function(loadForGraphics, Available_0_8) auto load_for_graphics() -> void;
        lua_function(unloadForGraphics, Available_0_8) auto unload_for_graphics() -> void;

        lua_function(withSize, Available_0_8) auto with_size(std::uint32_t size) -> lua_reference;

        auto push(const std::function<auto()->void>& scope) const -> void;

    private:
        static constexpr const char * imgui_default_font { "ImGui.DefaultFont" };

    private:
        struct {
            ImFontConfig config;
            ImFont *imgui { nullptr };
            std::shared_ptr<graphics::font> graphics;
        } m_instances;
        std::uint32_t m_font_size { 13 };
        std::string m_font_face { imgui_default_font };
        std::string m_path;
    };
}