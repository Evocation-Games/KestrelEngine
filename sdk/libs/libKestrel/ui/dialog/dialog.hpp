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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/imgui/imgui.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/ui/legacy/macintosh/item_list.hpp>
#include <libKestrel/ui/dialog/dialog_render_mode.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>

namespace kestrel::ui
{
    struct game_scene;
    class dialog_configuration;

    class lua_api(UI.Dialog, Available_0_8) dialog
    {
    public:
        has_constructable_lua_api(dialog);

        explicit dialog(dialog_configuration* config, ui::game_scene *scene = nullptr);

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_function(present, Available_0_8) auto present() -> void;
        auto present_into_scene(ui::game_scene *scene) -> void;

        lua_function(setBackground, Available_0_8) auto set_background(const luabridge::LuaRef& background) -> void;
        lua_function(setStretchableBackground, Available_0_8) auto set_stretchable_background(const math::size& size, const luabridge::LuaRef& top, const luabridge::LuaRef& fill, const luabridge::LuaRef& bottom) -> void;

        lua_function(configureElementsOnOpen, Available_0_9) auto configure_elements_on_open(const luabridge::LuaRef& configure) -> void;
        lua_function(configureElement, Available_0_8) auto configure_element(const std::string& name, const luabridge::LuaRef& configure) -> void;
        lua_function(elementNamed, Available_0_8) auto named_element(const std::string& name) -> luabridge::LuaRef;

        lua_function(close, Available_0_8) auto close() -> void;

    private:
        enum dialog_render_mode m_mode { dialog_render_mode::scene };
        dialog_configuration *m_configuration { nullptr };
        std::unordered_map<std::string, luabridge::LuaRef> m_elements;
        luabridge::LuaRef m_configure_elements { nullptr };

        math::size m_positioning_offset;
        math::rect m_frame;
        std::string m_name;
        ui::game_scene *m_owner_scene { nullptr };
        bool m_open { false };

        struct {
            image::static_image::lua_reference top { nullptr };
            image::static_image::lua_reference fill { nullptr };
            image::static_image::lua_reference bottom { nullptr };
            ui::scene_entity::lua_reference top_entity { nullptr };
            ui::scene_entity::lua_reference fill_entity { nullptr };
            ui::scene_entity::lua_reference bottom_entity { nullptr };
        } m_background;

        struct {
            ui::imgui::window::lua_reference window { nullptr };
        } m_imgui;

        struct {
            math::size frame_size;
        } m_scene_ui;

        auto load_contents(dialog_configuration *config, ui::game_scene *scene) -> void;
        auto load_imgui_contents(dialog_configuration *config, const ui::game_scene *scene) -> void;
        auto load_scene_contents(dialog_configuration *config, const ui::game_scene *scene) -> void;
        auto present_imgui(ui::game_scene *scene) -> void;
        auto present_scene(ui::game_scene *scene) -> void;

        auto add_to_scene(ui::game_scene *scene) -> void;

        auto configure_elements() -> void;
    };
}
