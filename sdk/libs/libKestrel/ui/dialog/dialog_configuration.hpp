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

#include <vector>
#include <unordered_map>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>
#include <libKestrel/ui/dialog/dialog.hpp>
#include <libKestrel/ui/dialog/dialog_layout.hpp>

namespace kestrel::ui
{
    class lua_api(UI.DialogConfiguration, Available_0_8) dialog_configuration
    {
    public:
        has_constructable_lua_api(dialog_configuration);

        luatool_type_fix(const luabridge::LuaRef&, layout)
        lua_constructor(Available_0_8) explicit dialog_configuration(const luabridge::LuaRef& layout);

        auto build_into_scene(const game_scene::lua_reference& scene) -> dialog::lua_reference;
        lua_function(build, Available_0_8) auto build(const luabridge::LuaRef& configure_callback) -> dialog::lua_reference;

        lua_getter(size, Available_0_8) [[nodiscard]] inline auto size() const -> math::size { return m_layout.size(); }
        lua_setter(size, Available_0_8) inline auto set_size(const math::size& size) -> void { m_layout.set_size(size); }

        lua_function(setBackground, Available_0_8) auto set_background(const luabridge::LuaRef& background) -> void;
        lua_function(setStretchedBackground, Available_0_8) auto set_stretched_background(const luabridge::LuaRef& top, const luabridge::LuaRef& fill, const luabridge::LuaRef& bottom) -> void;

        lua_setter(passthrough, Available_0_8) auto set_passthrough(bool f) -> void { m_passthrough = f; }
        lua_getter(passthrough, Available_0_8) [[nodiscard]] auto passthrough() const -> bool { return m_passthrough; }

        lua_function(defineElement, Available_0_8) auto define_element(const luabridge::LuaRef& index, const std::string& name, std::uint8_t type) -> control_definition::lua_reference;
        lua_function(element, Available_0_8) auto element(const std::string& name) -> control_definition::lua_reference;
        [[nodiscard]] auto all_elements() const -> std::vector<std::string>;

        [[nodiscard]] inline auto background() const -> image::static_image::lua_reference { return m_background_image; }
        [[nodiscard]] inline auto background_stretch() const -> image::static_image::lua_reference { return m_background_stretch_image; }
        [[nodiscard]] inline auto background_bottom() const -> image::static_image::lua_reference { return m_background_bottom_image; }

        [[nodiscard]] inline auto layout() const -> const dialog_layout * { return &m_layout; }

    private:
        std::vector<std::pair<std::string, control_definition::lua_reference>> m_element_definitions;
        dialog_layout m_layout;
        ui::dialog::lua_reference m_dialog { nullptr };
        bool m_passthrough { true };

        image::static_image::lua_reference m_background_image { nullptr };
        image::static_image::lua_reference m_background_stretch_image { nullptr };
        image::static_image::lua_reference m_background_bottom_image { nullptr };

        static auto load_image_asset(const luabridge::LuaRef& ref) -> image::static_image::lua_reference;
    };
}
