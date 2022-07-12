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
#include "util/hint.hpp"
#include "math/rect.hpp"
#include "scripting/state.hpp"
#include "core/ui/game_scene.hpp"
#include "core/ui/imgui/imgui.hpp"
#include "core/asset/static_image.hpp"
#include "core/ui/control_definition.hpp"
#include "core/asset/legacy/macintosh/item_list.hpp"

namespace ui
{
    class dialog_configuration;

    class dialog : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<dialog> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit dialog(dialog_configuration* config);
        explicit lua_api dialog(const luabridge::LuaRef& ref);

        [[nodiscard]] auto frame() const -> math::rect;

        lua_api auto present() -> void;
        lua_api auto present_in_scene(const ui::game_scene::lua_reference& scene) -> void;

        lua_api auto set_background(const luabridge::LuaRef& background) -> void;
        lua_api auto set_stretchable_background(const math::size& size, const luabridge::LuaRef& top, const luabridge::LuaRef& fill, const luabridge::LuaRef& bottom) -> void;

        lua_api auto configure_element(const std::string& name, const luabridge::LuaRef& configure) -> void;
        lua_api auto named_element(const std::string& name) -> ui::control_definition::lua_reference;

        lua_api auto close() -> void;

        lua_api auto update() -> void;

        [[nodiscard]] lua_api auto passthrough() const -> bool { return false; }
        lua_api auto set_passthrough(bool f) -> void { }

    private:
        dialog_configuration *m_configuration { nullptr };
        math::size m_positioning_offset;
        math::rect m_frame;
        std::string m_name;
        std::unordered_map<std::string, ui::control_definition::lua_reference> m_control_definitions;
        ui::game_scene::lua_reference m_owner_scene { nullptr };
        asset::legacy::macintosh::toolbox::item_list::lua_reference m_item_list { nullptr };

        struct {
            asset::static_image::lua_reference top { nullptr };
            asset::static_image::lua_reference fill { nullptr };
            asset::static_image::lua_reference bottom { nullptr };
            ui::scene_entity::lua_reference top_entity { nullptr };
            ui::scene_entity::lua_reference fill_entity { nullptr };
            ui::scene_entity::lua_reference bottom_entity { nullptr };
        } m_background;

        struct {
            ui::imgui::window::lua_reference window { nullptr };
        } m_imgui;

        struct {

        } m_scene_ui;

        auto load_contents(dialog_configuration *config) -> void;
        auto present_imgui(const ui::game_scene::lua_reference& scene) -> void;
        auto present_scene(const ui::game_scene::lua_reference& scene) -> void;

        auto add_to_scene(const ui::game_scene::lua_reference& scene) -> void;
    };
}
