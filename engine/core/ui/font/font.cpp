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

#include <imgui/imgui_internal.h>
#include "core/ui/font/font.hpp"
#include "core/graphics/common/font.hpp"
#include "core/ui/font/manager.hpp"
#include <libGraphite/font/manager.hpp>
#if TARGET_MACOS
#   include "core/support/macos/cocoa/font.h"
#elif TARGET_LINUX
#   include "core/support/linux/font_config.hpp"
#elif TARGET_WINDOWS
#   include "core/support/windows/fonts.hpp"
#endif

namespace ui::font::constants
{
    constexpr const char *rsrc_marker = "rsrc::font_manager::";
    constexpr std::size_t rsrc_marker_len = 20;
}

// MARK: - Lua

auto ui::font::reference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<reference>("Font")
            .addConstructor<auto(*)()->void, lua_reference>()
            .addStaticFunction("namedFont", &reference::named_font)
            .addProperty("lineHeight", &reference::line_height)
            .addProperty("fontSize", &reference::font_size)
            .addProperty("fontFace", &reference::font_face)
            .addProperty("path", &reference::path)
            .addFunction("loadForImGui", &reference::load_for_imgui)
            .addFunction("unloadForImGui", &reference::unload_for_imgui)
            .addFunction("loadForGraphics", &reference::load_for_graphics)
            .addFunction("unloadForGraphics", &reference::unload_for_graphics)
            .addFunction("withSize", &reference::with_size)
        .endClass();
}

// MARK: - Construction

ui::font::reference::reference()
    : m_font_face(imgui_default_font), m_font_size(13)
{
}

ui::font::reference::reference(const std::string &name, uint32_t size)
    : m_font_size(size), m_font_face(name)
{
    // Find the location of the font on the computer.
#if TARGET_MACOS
    m_path = cocoa::font::path_for(name);
#elif TARGET_LINUX
    m_path = linux_os::font_config::path_for_best_fit_font(name);
#elif TARGET_WINDOWS
    m_path = "C:/Windows/Fonts/Arial.ttf";
#endif
}

auto ui::font::reference::named_font(const std::string &name, uint32_t size) -> lua_reference
{
    return { new reference(name, size) };
}

// MARK: - General

auto ui::font::reference::load() -> void
{
    load_for_imgui();
    load_for_graphics();
}

auto ui::font::reference::unload() -> void
{
    unload_for_imgui();
    unload_for_graphics();
}

// MARK: - ImGui

auto ui::font::reference::load_for_imgui() -> void
{
    if (m_instances.imgui) {
        return;
    }

    auto& io = ImGui::GetIO();
    m_instances.config.SizePixels = static_cast<decltype(m_instances.config.SizePixels)>(m_font_size);
    m_instances.config.OversampleV = 1;
    m_instances.config.OversampleH = 1;

    if (m_font_face == imgui_default_font) {
        m_instances.imgui = io.Fonts->AddFontDefault(&m_instances.config);
    }
    else if (m_path.starts_with(constants::rsrc_marker)) {
        // The font data is contained in the font manager for graphite.
//        auto name = m_path.substr(constants::rsrc_marker_len);
//        if (auto ttf = graphite::font::manager::shared_manager().ttf_font_named(name)) {
//            m_instances.config.FontDataOwnedByAtlas = false;
//            if (m_instances.config.Name[0] == '\0')
//            {
//                ImFormatString(m_instances.config.Name, IM_ARRAYSIZE(m_instances.config.Name), "%s, %.0fpx", name.c_str(), m_font_size);
//            }
//            m_instances.imgui = io.Fonts->AddFontFromMemoryTTF(ttf->get<uint8_t *>(0), m_font_size, m_font_size, &m_instances.config);
//        }
// TODO: Fix the above code and prevent the crash that occurs.
        m_instances.imgui = io.Fonts->AddFontDefault(&m_instances.config);
    }
    else {
        m_instances.imgui = io.Fonts->AddFontFromFileTTF(m_path.c_str(), m_font_size, &m_instances.config);
    }

    m_instances.imgui->Scale = 1.f;
}

auto ui::font::reference::unload_for_imgui() -> void
{
    m_instances.imgui = nullptr;
}

// MARK: - Graphics

auto ui::font::reference::load_for_graphics() -> void
{
    if (m_instances.graphics || m_font_face == imgui_default_font) {
        return;
    }

    auto path = m_path;
    if (path.starts_with(constants::rsrc_marker)) {
        path = path.substr(constants::rsrc_marker_len);
    }

    m_instances.graphics = std::make_shared<graphics::font>(path, m_font_size);
}

auto ui::font::reference::unload_for_graphics() -> void
{
    m_instances.graphics = nullptr;
}

// MARK: - Font Usage

auto ui::font::reference::push(const std::function<auto()->void> &scope) const -> void
{
    if (!m_instances.imgui) {
        return;
    }

    ImGui::PushFont(m_instances.imgui);
    scope();
    ImGui::PopFont();
}

// MARK: - Alterations

auto ui::font::reference::with_size(std::uint32_t size) -> lua_reference
{
    auto existing = manager::shared_manager().get_font(m_path, size);
    if (existing.get()) {
        return existing;
    }

    lua_reference adjusted(new reference(m_path, size));
    return manager::shared_manager().add_font(adjusted);
}

// MARK: - Accessors

auto ui::font::reference::line_height() const -> std::uint32_t
{
    if (m_instances.graphics) {
        return m_instances.graphics->line_height();
    }
    else if (m_instances.imgui) {
        return 0;
    }
    return 0;
}