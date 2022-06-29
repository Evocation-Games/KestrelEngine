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

#include <stdexcept>
#include "core/ui/game_scene.hpp"
#include "core/ui/entity/scene_entity.hpp"
#include "core/environment.hpp"
#include "core/ui/widgets/text_widget.hpp"
#include "core/ui/widgets/label_widget.hpp"
#include "core/ui/widgets/custom_widget.hpp"
#include "core/ui/widgets/image_widget.hpp"
#include "core/ui/widgets/button_widget.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Lua

auto ui::game_scene::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<game_scene>("Scene")
            .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
            .addStaticProperty("current", &game_scene::current)
            .addStaticFunction("back", &game_scene::back)
            .addProperty("canPush", &game_scene::can_push)
            .addProperty("size", &game_scene::size)
            .addProperty("centerPoint", &game_scene::center_point)
            .addProperty("name", &game_scene::name)
            .addProperty("currentTime", &game_scene::current_time)
            .addProperty("passthroughRender", &game_scene::passthrough_render, &game_scene::set_passthrough_render)
            .addProperty("positioningFrame", &game_scene::positioning_frame, &game_scene::set_positioning_frame)
            .addProperty("isCurrent", &game_scene::is_current)
            .addProperty("entities", &game_scene::entities)
            .addFunction("push", &game_scene::push)
            .addFunction("render", &game_scene::on_render)
            .addFunction("update", &game_scene::on_update)
            .addFunction("onKeyEvent", &game_scene::on_key_event)
            .addFunction("onMouseEvent", &game_scene::on_mouse_event)
            .addFunction("after", &game_scene::after)
            .addFunction("repeatEvery", &game_scene::repeat)
            .addFunction("addEntity", &game_scene::add_entity)
            .addFunction("replaceEntity", &game_scene::replace_entity)
            .addFunction("key", &game_scene::key)
            .addFunction("keyDown", &game_scene::is_key_down)
            .addFunction("keyReleased", &game_scene::is_key_released)
            .addFunction("addWidget", &game_scene::add_widget)
            .addFunction("importSupportingScripts", &game_scene::import_supporting_scripts)
        .endClass();
}

// MARK: - Construction

ui::game_scene::game_scene(const asset::resource_descriptor::lua_reference &script_ref)
    : m_script_descriptor(script_ref->with_type("LuaS"))
{
    auto env = environment::active_environment().lock();
    if (!env) {
        throw std::runtime_error("Attempting to push a scene outside of a valid environment.");
    }

    auto session = env->session();
    if (!session) {
        throw std::runtime_error("Attempting to push a scene with no active session.");
    }

    if (!can_push()) {
        env->lua_out("Failed to push scene as script was invalid.", true);
        return;
    }

    scripting::lua::script scene_main_script { env->lua_runtime(), m_script_descriptor };
    m_name = scene_main_script.name().empty() ? " untitled-scene" : scene_main_script.name();
    m_backing_scene = session->new_scene(m_name, scene_main_script);

    // Setup some defaults...
    auto size = session->size();
    m_positioning_frame = {
        new ui::layout::positioning_frame(
            { 0, 0, size.width, size.height },
            { 0.5, 0.5 },
            { 0, 0 }
        )
    };

    m_backing_scene->add_mouse_event_block([&, this] (const event& e) {
        if (!m_user_input || !e.is_mouse_event()) {
            return;
        }

        auto point = e.location();
        auto local_point = m_positioning_frame->convert_point(point);

        for (const auto& entity : m_entities) {
            entity->send_event(event::mouse(e.type(), local_point - entity->position()));
        }

        m_responder_chain.send_event(e.relocated(local_point));

        if (m_mouse_event_block.state() && m_mouse_event_block.isFunction()) {
            m_mouse_event_block(event::lua_reference( new event(e) ));
        }
    });

    m_backing_scene->add_key_event_block([&, this] (const event& e) {
        if (!m_user_input || !e.is_key_event()) {
            return;
        }

        m_key_states[static_cast<int>(e.key())] = { new event(e) };

        if (m_key_event_block.state() && m_key_event_block.isFunction()) {
            m_key_event_block(event::lua_reference( new event(e) ));
        }

        m_responder_chain.send_event(e);
    });

    m_backing_scene->add_render_block([&, this] {
        const auto& entities = this->m_entities;
        for (auto i = 0; i < entities.size(); ++i) {
            const auto& entity = entities[i];
            entity->set_draw_position(m_positioning_frame->position_for_entity(*entity.get()));
            entity->layout();
            entity->draw();
        }

        draw_widgets();

        if (m_render_block.state() && m_render_block.isFunction()) {
            m_render_block();
        }
    });

    m_backing_scene->add_update_block([&, this] {
        if (m_update_block.state() && m_update_block.isFunction()) {
            m_update_block();
        }

        m_backing_scene->check_timed_events();

        // Remove any of the "up" states.
        for (auto it = m_key_states.begin(); it != m_key_states.end();) {
            if (it->second->is_key_event() && it->second->is_released()) {
                it = m_key_states.erase(it);
            }
            else {
                it++;
            }
        }
    });

}

auto ui::game_scene::current() -> lua_reference
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return nullptr;
    }

    auto session = env->session();
    if (!session) {
        return nullptr;
    }

    return env->session()->current_scene();
}

// MARK: - Management

auto ui::game_scene::push() -> void
{
    auto env = environment::active_environment().lock();
    if (!env || !can_push()) {
        return;
    }

    auto session = env->session();
    if (!session) {
        return;
    }

    session->console().write("Presenting Scene: " + name());
    session->present_scene({ this });
}

auto ui::game_scene::can_push() const -> bool
{
    return m_script_descriptor->valid();
}

auto ui::game_scene::start() const -> void
{
    m_backing_scene->start();
    renderer::resync_clock();
}

auto ui::game_scene::pop() const -> void
{
    auto env = environment::active_environment().lock();
    if (env && is_current()) {
        auto scene = env->session();
        if (scene) {
            scene->pop_scene();
        }
    }
}

auto ui::game_scene::back() -> void
{
    auto env = environment::active_environment().lock();
    if (env) {
        auto scene = env->session();
        if (scene) {
            scene->pop_scene();
        }
    }
}

auto ui::game_scene::internal_scene() -> std::shared_ptr<ui::scene>
{
    return m_backing_scene;
}

auto ui::game_scene::is_current() const -> bool
{
    auto env = environment::active_environment().lock();
    if (env) {
        auto session = env->session();
        if (session) {
            return session->current_scene().get() == this;
        }
    }
    return false;
}

// MARK: - Accessors

auto ui::game_scene::name() const -> std::string
{
    return m_name;
}

auto ui::game_scene::center_point() const -> math::point
{
    return { size().width / 2, size().height / 2 };
}

auto ui::game_scene::size() const -> math::size
{
    auto env = environment::active_environment().lock();
    if (env) {
        auto session = env->session();
        if (session) {
            return session->size();
        }
    }
    return { 0, 0 };
}

auto ui::game_scene::current_time() const -> double
{
    return m_backing_scene->current_time();
}

auto ui::game_scene::passthrough_render() const -> bool
{
    return m_backing_scene->is_passthrough_render();
}

auto ui::game_scene::entities() const -> util::lua_vector<scene_entity::lua_reference>
{
    return util::lua_vector(m_entities);
}

auto ui::game_scene::positioning_frame() const -> ui::layout::positioning_frame::lua_reference
{
    return m_positioning_frame;
}

// MARK: - Setters

auto ui::game_scene::set_passthrough_render(bool f) -> void
{
    m_backing_scene->set_passthrough_render(f);
}

auto ui::game_scene::set_positioning_frame(const ui::layout::positioning_frame::lua_reference &positioning) -> void
{
    m_positioning_frame = positioning;
}

// MARK: - Callbacks

auto ui::game_scene::on_render(const luabridge::LuaRef& block) -> void
{
    m_render_block = block;
}

auto ui::game_scene::on_update(const luabridge::LuaRef& block) -> void
{
    m_update_block = block;
}

auto ui::game_scene::on_key_event(const luabridge::LuaRef& block) -> void
{
    m_key_event_block = block;
}

auto ui::game_scene::on_mouse_event(const luabridge::LuaRef& block) -> void
{
    m_mouse_event_block = block;
}

auto ui::game_scene::after(double period, const luabridge::LuaRef& block) -> void
{
    m_backing_scene->add_timed_event(std::make_shared<rtc::timed_event>(period, block));
}

auto ui::game_scene::repeat(double period, const luabridge::LuaRef& block) -> void
{
    m_backing_scene->add_timed_event(std::make_shared<rtc::timed_event>(period, block, true));
}

// MARK: - Entity Management

auto ui::game_scene::add_entity(const scene_entity::lua_reference& entity) -> int32_t
{
    auto index = m_entities.size();
    m_entities.emplace_back(entity);
    return index;
}

auto ui::game_scene::replace_entity(int32_t index, const scene_entity::lua_reference& entity) -> void
{
    if (index >= m_entities.size()) {
        // TODO: Warning?
        return;
    }
    m_entities[index] = entity;
}

// MARK: - Add Widget

auto ui::game_scene::add_widget(const luabridge::LuaRef &widget) -> void
{
    auto scene = internal_scene();
    m_widgets.emplace_back(widget);

    if (scripting::lua::ref_isa<ui::widgets::text_widget>(widget)) {
        auto text = widget.cast<ui::widgets::text_widget::lua_reference>();
        text->attach_responder_chain(&m_responder_chain);
        text->become_first_responder();
    }
    else if (scripting::lua::ref_isa<ui::widgets::button_widget>(widget)) {
        auto button = widget.cast<ui::widgets::button_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(button.get());
    }
}

auto ui::game_scene::draw_widgets() const -> void
{
    std::shared_ptr<scene_entity> entity;

    for (const auto& widget : m_widgets) {
        if (!widget.isUserdata()) {
            continue;
        }

        if (scripting::lua::ref_isa<ui::widgets::label_widget>(widget)) {
            auto label = widget.cast<ui::widgets::label_widget::lua_reference>();
            entity = label->entity();
            label->draw();
        }
        else if (scripting::lua::ref_isa<ui::widgets::text_widget>(widget)) {
            auto text = widget.cast<ui::widgets::text_widget::lua_reference>();
            entity = text->entity();
            text->draw();
        }
        else if (scripting::lua::ref_isa<ui::widgets::custom_widget>(widget)) {
            auto custom = widget.cast<ui::widgets::custom_widget::lua_reference>();
            entity = custom->entity();
            custom->draw();
        }
        else if (scripting::lua::ref_isa<ui::widgets::image_widget>(widget)) {
            auto image = widget.cast<ui::widgets::image_widget::lua_reference>();
            entity = image->entity();
            image->draw();
        }
        else if (scripting::lua::ref_isa<ui::widgets::button_widget>(widget)) {
            auto button = widget.cast<ui::widgets::button_widget::lua_reference>();
            entity = button->entity();
            button->draw();
        }
        else {
            // TODO: Unrecognised widget type... skip.
            continue;
        }

        entity->set_draw_position(m_positioning_frame->position_for_entity(*entity));
        entity->layout();
        entity->draw();
    }
}

// MARK: - Key States

auto ui::game_scene::key(int k) const -> event::lua_reference
{
    if (m_key_states.find(k) == m_key_states.end()) {
        return { new event(event::key(event::type::none, static_cast<hid::key>(k))) };
    }
    return m_key_states.at(k);
}

auto ui::game_scene::is_key_down(int k) const -> bool
{
    const auto& key = this->key(k);
    return (key->is_pressed() || key->is_held());
}

auto ui::game_scene::is_key_released(int k) const -> bool
{
    const auto& key = this->key(k);
    return (key->is_released());
}

// MARK: - Script Importing

static std::vector<std::string> s_imported_namespaces;

auto ui::game_scene::import_supporting_scripts(const luabridge::LuaRef& ref) -> void
{
    if (!scripting::lua::ref_isa<asset::resource_namespace>(ref)) {
        return;
    }
    auto ns = ref.cast<asset::resource_namespace::lua_reference>();

    if (std::find(s_imported_namespaces.begin(), s_imported_namespaces.end(), ns->primary_name()) != s_imported_namespaces.end()) {
        return;
    }

    if (auto env = environment::active_environment().lock()) {
        auto scripts = ns->typed_resource("LuaS")->matching_resources();
        for (auto i = 0; i < scripts.size(); ++i) {
            auto script = scripts.at(i);
            if (script->id == 1000) {
                continue;
            }
            env->import_script(script);
        }
    }

    s_imported_namespaces.emplace_back(ns->primary_name());
}