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

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "scripting/state.hpp"
#include "scripting/script.hpp"
#include "util/hint.hpp"
#include "util/lua_vector.hpp"
#include "core/ui/scene.hpp"
#include "core/event/event.hpp"
#include "core/event/responder/responder_chain.hpp"
#include "core/ui/entity/scene_entity.hpp"
#include "core/ui/layout/positioning_frame.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"

namespace ui
{

    struct game_scene: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<game_scene> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit game_scene(const asset::resource_descriptor::lua_reference& script_ref);
        lua_api static auto current() -> lua_reference;

        [[nodiscard]] lua_api auto can_push() const -> bool;
        lua_api auto start() const -> void;
        lua_api auto pop() const -> void;
        lua_api static auto back() -> void;
        lua_api auto push() -> void;

        auto internal_scene() -> std::shared_ptr<ui::scene>;

        [[nodiscard]] lua_api auto is_current() const -> bool;
        [[nodiscard]] lua_api auto name() const -> std::string;
        [[nodiscard]] lua_api auto center_point() const -> math::point;
        [[nodiscard]] lua_api auto size() const -> math::size;
        [[nodiscard]] lua_api auto current_time() const -> double;
        [[nodiscard]] lua_api auto passthrough_render() const -> bool;
        [[nodiscard]] lua_api auto entities() const -> util::lua_vector<luabridge::LuaRef>;
        [[nodiscard]] lua_api auto positioning_frame() const -> ui::layout::positioning_frame::lua_reference;
        [[nodiscard]] lua_api auto disable_user_input() const -> bool;

        lua_api auto set_passthrough_render(bool f) -> void;
        lua_api auto set_positioning_frame(const ui::layout::positioning_frame::lua_reference& positioning) -> void;

        lua_api auto on_render(const luabridge::LuaRef& block) -> void;
        lua_api auto on_update(const luabridge::LuaRef& block) -> void;
        lua_api auto on_key_event(const luabridge::LuaRef& block) -> void;
        lua_api auto on_mouse_event(const luabridge::LuaRef& block) -> void;
        lua_api auto after(double period, const luabridge::LuaRef& block) -> void;
        lua_api auto repeat(double period, const luabridge::LuaRef& block) -> void;

        lua_api auto add_scene_entity(const scene_entity::lua_reference & entity) -> int32_t;
        lua_api auto add_entity(const luabridge::LuaRef& entity) -> int32_t;
        lua_api auto replace_entity(int32_t index, const luabridge::LuaRef& entity) -> void;
        lua_api auto add_widget(const luabridge::LuaRef& widget) -> void;

        [[nodiscard]] lua_api auto key(int k) const -> event::lua_reference;
        [[nodiscard]] lua_api auto is_key_down(int k) const -> bool;
        [[nodiscard]] lua_api auto is_key_released(int k) const -> bool;

        lua_api auto set_user_input_disabled(bool disabled) -> void;

        lua_api auto import_supporting_scripts(const luabridge::LuaRef& ns) -> void;

        lua_api auto draw_line(const math::point& p, const math::point& q, const graphics::color::lua_reference& color, float weight) -> void;

    private:
        std::string m_name;
        asset::resource_descriptor::lua_reference m_script_descriptor;
        std::shared_ptr<scene> m_backing_scene;
        layout::positioning_frame::lua_reference m_positioning_frame { nullptr };
        bool m_user_input { true };
        std::vector<luabridge::LuaRef> m_entities;
        std::vector<luabridge::LuaRef> m_widgets;
        std::unordered_map<int, event::lua_reference> m_key_states;
        luabridge::LuaRef m_render_block { nullptr };
        luabridge::LuaRef m_update_block { nullptr };
        luabridge::LuaRef m_key_event_block { nullptr };
        luabridge::LuaRef m_mouse_event_block { nullptr };
        responder_chain m_responder_chain;
        bool m_imgui { false };

        auto draw_widgets() const -> void;

        auto forward_event_to_widgets(const event& e) -> void;
    };

}