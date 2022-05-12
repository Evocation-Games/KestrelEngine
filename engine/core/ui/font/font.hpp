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
#include "core/graphics/common/font.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::font
{
    struct reference : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<reference> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit reference();
        lua_api explicit reference(const std::string& name, uint32_t size);

        lua_api static auto named_font(const std::string& name, uint32_t size) -> lua_reference;

        [[nodiscard]] lua_api inline auto font_size() const -> uint32_t { return m_font_size; }
        [[nodiscard]] lua_api inline auto font_face() const -> std::string { return m_font_face; }
        [[nodiscard]] lua_api inline auto path() const -> std::string { return m_path; }
        inline auto imgui_font() const -> ImFont * { return m_instances.imgui; }
        inline auto graphics_font() const -> std::shared_ptr<graphics::font> { return m_instances.graphics; }

        lua_api auto load() -> void;
        lua_api auto unload() -> void;

        lua_api auto load_for_imgui() -> void;
        lua_api auto unload_for_imgui() -> void;

        lua_api auto load_for_graphics() -> void;
        lua_api auto unload_for_graphics() -> void;

        auto push(const std::function<auto()->void>& scope) const -> void;

    private:
        static constexpr const char * imgui_default_font { "ImGui.DefaultFont" };

    private:
        struct {
            ImFontConfig config;
            ImFont *imgui { nullptr };
            std::shared_ptr<graphics::font> graphics;
        } m_instances;
        uint32_t m_font_size { 13 };
        std::string m_font_face;
        std::string m_path;
    };
}