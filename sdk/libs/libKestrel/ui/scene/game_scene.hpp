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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/ui/scene/scene.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/event/responder/responder_chain.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/physics/world.hpp>
#include <libKestrel/ui/widgets/menu_widget.hpp>
#include <libKestrel/ui/dialog/dialog.hpp>

namespace kestrel::ui
{
    struct lua_api(Scene, Available_0_8) game_scene
    {
    public:
        has_constructable_lua_api(game_scene);

        static constexpr std::int64_t initial_script_id = 1000;

        lua_constructor(Available_0_8) explicit game_scene(const resource::descriptor::lua_reference& script_ref);
        lua_getter(current, Available_0_8) static auto current() -> lua_reference;

        auto will_close() -> void;

        lua_getter(canPush, Available_0_8) [[nodiscard]] auto can_push() const -> bool;
        lua_function(start, Available_0_8) auto start() const -> void;
        lua_function(pop, Available_0_8) auto pop() const -> void;
        lua_function(back, Available_0_8) static auto back() -> void;
        lua_function(push, Available_0_8) auto push() -> void;

        lua_function(onClose, Available_0_9) auto on_close(const luabridge::LuaRef& ref) -> void;

        auto internal_scene() -> std::shared_ptr<scene>;
        auto physics_world() -> std::shared_ptr<physics::world>;
        auto responder_chain() -> responder_chain&;

        lua_function(adoptPhysicsBody, Available_0_8) auto adopt_physics_body(physics::body::lua_reference body) -> void;

        lua_getter(isCurrent, Available_0_8) [[nodiscard]] auto is_current() const -> bool;
        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_getter(centerPoint, Available_0_8) [[nodiscard]] auto center_point() const -> math::point;
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size;
        lua_getter(currentTime, Available_0_8) [[nodiscard]] auto current_time() const -> double;
        lua_getter(passthroughRender, Available_0_8) [[nodiscard]] auto passthrough_render() const -> bool;
        lua_getter(entities, Available_0_8) [[nodiscard]] auto entities() const -> lua::vector<luabridge::LuaRef>;
        lua_getter(disableUserInput, Available_0_8) [[nodiscard]] auto disable_user_input() const -> bool;
        lua_getter(sceneBoundingFrame, Available_0_9) [[nodiscard]] auto scene_bounding_frame() const -> math::rect;
        lua_getter(sceneScalingFactor, Available_0_9) [[nodiscard]] auto scene_scaling_factor() const -> double;

        lua_setter(passthroughRender, Available_0_8) auto set_passthrough_render(bool f) -> void;
        lua_setter(sceneBoundingFrame, Available_0_9) auto set_scene_bounding_frame(const math::rect& frame) -> void;
        lua_setter(sceneScalingFactor, Available_0_9) auto set_scene_scaling_factor(double factor) -> void;

        lua_function(render, Available_0_8) auto on_render(const luabridge::LuaRef& block) -> void;
        lua_function(update, Available_0_8) auto on_update(const luabridge::LuaRef& block) -> void;
        lua_function(onKeyEvent, Available_0_8) auto on_key_event(const luabridge::LuaRef& block) -> void;
        lua_function(onMouseEvent, Available_0_8) auto on_mouse_event(const luabridge::LuaRef& block) -> void;
        lua_function(after, Available_0_8) auto after(double period, const luabridge::LuaRef& block) -> void;
        lua_function(repeatEvery, Available_0_8) auto repeat(double period, const luabridge::LuaRef& block) -> void;

        auto add_scene_entity(const scene_entity::lua_reference & entity) -> std::int32_t;
        lua_function(addEntity, Available_0_8) auto add_entity(const luabridge::LuaRef& entity) -> std::int32_t;
        lua_function(replaceEntity, Available_0_8) auto replace_entity(std::int32_t index, const luabridge::LuaRef& entity) -> void;
        lua_function(addWidget, Available_0_8) auto add_widget(const luabridge::LuaRef& widget) -> void;

        lua_setter(menuWidget, Available_0_9) auto set_menu_widget(const widgets::menu_widget::lua_reference& menu) -> void;
        lua_getter(menuWidget, Available_0_9) [[nodiscard]] auto menu_widget() const -> widgets::menu_widget::lua_reference;

        lua_function(key, Available_0_8) [[nodiscard]] auto key(std::int32_t k) const -> event::lua_reference;
        lua_function(keyDown, Available_0_8) [[nodiscard]] auto is_key_down(std::int32_t k) const -> bool;
        lua_function(keyReleased, Available_0_8) [[nodiscard]] auto is_key_released(std::int32_t k) const -> bool;

        lua_setter(disableUserInput, Available_0_8) auto set_user_input_disabled(bool disabled) -> void;

        lua_function(importSupportingScripts, Available_0_8) auto import_supporting_scripts(const luabridge::LuaRef& container) -> void;

        lua_function(drawLine, Available_0_8) auto draw_line(const math::point& p, const math::point& q, const graphics::color::lua_reference& color, float weight) -> void;

        lua_function(bind, Available_0_9) auto bind(const luabridge::LuaRef& bindings) -> void;
        lua_getter(Bindings, Available_0_9) [[nodiscard]] auto bindings() const -> luabridge::LuaRef;

        lua_getter(Dialog, Available_0_9) [[nodiscard]] auto dialog() const -> ui::dialog::lua_reference;
        auto set_dialog(const ui::dialog::lua_reference& dialog) -> void;

        [[nodiscard]] auto find_function(const std::string& name) const -> luabridge::LuaRef;

    private:
        std::string m_name;
        std::shared_ptr<physics::world> m_world { std::make_shared<physics::world>() };
        resource::descriptor::lua_reference m_script_descriptor { nullptr };
        std::shared_ptr<scene> m_backing_scene;
        math::rect m_bounding_frame { 0, 0, 0, 0 };
        bool m_user_input { true };
        lua::vector<luabridge::LuaRef> m_entities;
        std::vector<luabridge::LuaRef> m_widgets;
        widgets::menu_widget::lua_reference m_menu_widget { nullptr };
        std::unordered_map<int, event::lua_reference> m_key_states;
        luabridge::LuaRef m_render_block { nullptr };
        luabridge::LuaRef m_update_block { nullptr };
        luabridge::LuaRef m_key_event_block { nullptr };
        luabridge::LuaRef m_mouse_event_block { nullptr };
        luabridge::LuaRef m_bindings { nullptr };
        luabridge::LuaRef m_on_close { nullptr };
        ui::dialog::lua_reference m_dialog { nullptr };
        struct responder_chain m_responder_chain;

        auto draw_widgets() const -> void;
    };

}