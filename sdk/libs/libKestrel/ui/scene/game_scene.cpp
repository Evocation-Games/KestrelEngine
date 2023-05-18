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
#include <libKestrel/kestrel.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/ui/entity/text_entity.hpp>
#include <libKestrel/ui/entity/line_entity.hpp>
#include <libKestrel/ui/widgets/text_widget.hpp>
#include <libKestrel/ui/widgets/label_widget.hpp>
#include <libKestrel/ui/widgets/custom_widget.hpp>
#include <libKestrel/ui/widgets/image_widget.hpp>
#include <libKestrel/ui/widgets/button_widget.hpp>
#include <libKestrel/ui/widgets/grid_widget.hpp>
#include <libKestrel/ui/widgets/textarea_widget.hpp>
#include <libKestrel/ui/widgets/list_widget.hpp>
#include <libKestrel/ui/widgets/scrollview_widget.hpp>
#include <libKestrel/ui/widgets/sprite_widget.hpp>
#include <libKestrel/ui/widgets/checkbox_widget.hpp>
#include <libKestrel/ui/widgets/popup_button_widget.hpp>
#include <libKestrel/resource/container.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/lua/script.hpp>
#include <libKestrel/exceptions/invalid_scene_exception.hpp>

static std::uint32_t scene_counter = 0;

// MARK: - Construction

static inline auto default_scene_name(const std::string& name) -> std::string
{
    return (name.empty() ? "untitled-scene" : name) + " " + std::to_string(scene_counter++);
}

kestrel::ui::game_scene::game_scene(const resource::descriptor::lua_reference &script_ref)
    : m_script_descriptor(script_ref->with_type(lua::script::resource_type::code))
{
    if (!can_push()) {
        throw invalid_scene_exception("Failed to push scene as script was invalid.");
    }

    lua::script scene_main_script(kestrel::lua_runtime(), m_script_descriptor);
    m_name = default_scene_name(scene_main_script.name());
    m_backing_scene = kestrel::create_backing_scene(scene_main_script, m_name);

    // Setup some defaults...
    m_positioning_frame = {
        new layout::positioning_frame(renderer::window_size(), layout::axis_origin::center, layout::scaling_mode::normal)
    };

    m_backing_scene->add_mouse_event_block([&, this] (const event& e) {
        if (!m_user_input || !e.is_mouse_event()) {
            return;
        }

        auto point = e.location();
        auto local_point = m_positioning_frame->translate_point_from(point);

        if (m_menu_widget.get()) {
            if (m_menu_widget->receive_event(e.relocated(local_point))) {
                m_menu_widget->will_close();
                m_menu_widget = { nullptr };
                return;
            }
            else if (e.has(::ui::event::any_mouse_down)) {
                m_menu_widget->will_close();
                m_menu_widget = { nullptr };
            }
        }

        for (const auto& entity_ref : m_entities) {
            if (lua::ref_isa<scene_entity>(entity_ref)) {
                auto entity = entity_ref.cast<scene_entity::lua_reference>();
                entity->send_event(event::mouse(e.type(), local_point - entity->position()));
            }
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

        m_key_states[static_cast<std::int32_t>(e.key())] = { new event(e) };

        if (m_key_event_block.state() && m_key_event_block.isFunction()) {
            m_key_event_block(event::lua_reference( new event(e) ));
        }

        m_responder_chain.send_event(e);
    });

    m_backing_scene->add_render_block([&, this] {
        const auto& entities = this->m_entities;
        for (auto i = 0; i < entities.size(); ++i) {
            if (lua::ref_isa<scene_entity>(entities[i])) {
                const auto& entity = entities[i].cast<scene_entity::lua_reference>();
                m_positioning_frame->position_scene_entity(entity);
                entity->layout();
                entity->draw();
            }
            else if (lua::ref_isa<text_entity>(entities[i])) {
                const auto& entity = entities[i].cast<text_entity::lua_reference>();
                m_positioning_frame->position_text_entity(entity);
                entity->layout();
                entity->draw();
            }
            else if (lua::ref_isa<line_entity>(entities[i])) {
                const auto& entity = entities[i].cast<line_entity::lua_reference>();
                m_positioning_frame->position_line_entity(entity);
                entity->layout();
                entity->draw();
            }
        }

        draw_widgets();

        if (m_render_block.state() && m_render_block.isFunction()) {
            m_render_block();
        }

    });

    m_backing_scene->add_update_block([&, this] {
        m_world->update();

        if (m_update_block.state() && m_update_block.isFunction()) {
            m_update_block();
        }

        m_backing_scene->check_timed_events();

        // Remove any of the "up" states.
        for (auto it = m_key_states.begin(); it != m_key_states.end();) {
            if (it->second->is_key_event() && it->second->has(::ui::event::key_up)) {
                it = m_key_states.erase(it);
            }
            else {
                it++;
            }
        }
    });
}

auto kestrel::ui::game_scene::current() -> lua_reference
{
    return kestrel::session().current_scene();
}

// MARK: - Destruction


auto kestrel::ui::game_scene::will_close() -> void
{
    if (m_on_close.state() && m_on_close.isFunction()) {
        m_on_close();
    }

    m_world->purge_all_bodies();

    m_entities.clear();
    m_widgets.clear();
    m_render_block = kestrel::lua_runtime()->null();
    m_update_block = kestrel::lua_runtime()->null();
    m_key_event_block = kestrel::lua_runtime()->null();
    m_mouse_event_block = kestrel::lua_runtime()->null();
    m_bindings = kestrel::lua_runtime()->null();
    m_on_close = kestrel::lua_runtime()->null();
    m_dialog = nullptr;
    m_positioning_frame = nullptr;
    m_backing_scene = nullptr;
}

auto kestrel::ui::game_scene::on_close(const luabridge::LuaRef &ref) -> void
{
    m_on_close = ref;
}

// MARK: - Management

auto kestrel::ui::game_scene::push() -> void
{
    // TODO: Verify the memory ownership here...
    kestrel::session().present_scene({ this });
}

auto kestrel::ui::game_scene::can_push() const -> bool
{
    return m_script_descriptor->valid();
}

auto kestrel::ui::game_scene::start() const -> void
{
    m_backing_scene->start();
    renderer::resync_clock();
}

auto kestrel::ui::game_scene::pop() const -> void
{
    kestrel::session().pop_scene();
}

auto kestrel::ui::game_scene::back() -> void
{
    kestrel::session().pop_scene();
}

auto kestrel::ui::game_scene::internal_scene() -> std::shared_ptr<ui::scene>
{
    return m_backing_scene;
}

auto kestrel::ui::game_scene::physics_world() -> std::shared_ptr<physics::world>
{
    return m_world;
}

auto kestrel::ui::game_scene::responder_chain() -> struct responder_chain&
{
    return m_responder_chain;
}

auto kestrel::ui::game_scene::is_current() const -> bool
{
    return (*kestrel::session().current_scene() == this);
}

// MARK: - Accessors

auto kestrel::ui::game_scene::name() const -> std::string
{
    return m_name;
}

auto kestrel::ui::game_scene::center_point() const -> math::point
{
    return math::vec2(size() / 2.f).to_point();
}

auto kestrel::ui::game_scene::size() const -> math::size
{
    return kestrel::session().size();
}

auto kestrel::ui::game_scene::disable_user_input() const -> bool
{
    return !m_user_input;
}

auto kestrel::ui::game_scene::set_user_input_disabled(bool disabled) -> void
{
    m_user_input = !disabled;
}

auto kestrel::ui::game_scene::current_time() const -> double
{
    return m_backing_scene->current_time();
}

auto kestrel::ui::game_scene::passthrough_render() const -> bool
{
    return m_backing_scene->is_passthrough_render();
}

auto kestrel::ui::game_scene::entities() const -> lua::vector<luabridge::LuaRef>
{
    return m_entities;
}

auto kestrel::ui::game_scene::positioning_frame() const -> layout::positioning_frame::lua_reference
{
    return m_positioning_frame;
}

// MARK: - Setters

auto kestrel::ui::game_scene::set_passthrough_render(bool f) -> void
{
    m_backing_scene->set_passthrough_render(f);
}

auto kestrel::ui::game_scene::set_positioning_frame(const layout::positioning_frame::lua_reference &positioning) -> void
{
    m_positioning_frame = positioning;
}

// MARK: - Callbacks

auto kestrel::ui::game_scene::on_render(const luabridge::LuaRef& block) -> void
{
    m_render_block = block;
}

auto kestrel::ui::game_scene::on_update(const luabridge::LuaRef& block) -> void
{
    m_update_block = block;
}

auto kestrel::ui::game_scene::on_key_event(const luabridge::LuaRef& block) -> void
{
    m_key_event_block = block;
}

auto kestrel::ui::game_scene::on_mouse_event(const luabridge::LuaRef& block) -> void
{
    m_mouse_event_block = block;
}

auto kestrel::ui::game_scene::after(double period, const luabridge::LuaRef& block) -> void
{
    m_backing_scene->add_timed_event(std::make_shared<rtc::timed_event>(period, block));
}

auto kestrel::ui::game_scene::repeat(double period, const luabridge::LuaRef& block) -> void
{
    m_backing_scene->add_timed_event(std::make_shared<rtc::timed_event>(period, block, true));
}

// MARK: - Entity Management

auto kestrel::ui::game_scene::add_scene_entity(const scene_entity::lua_reference &entity) -> std::int32_t
{
    return add_entity({ kestrel::lua_runtime()->internal_state(), entity });
}

auto kestrel::ui::game_scene::add_entity(const luabridge::LuaRef& entity) -> std::int32_t
{
    if (!entity.state()) {
        return -1;
    }

    auto index = m_entities.size();
    m_entities.emplace_back(entity);
    return static_cast<std::int32_t>(index);
}

auto kestrel::ui::game_scene::replace_entity(std::int32_t index, const luabridge::LuaRef& entity) -> void
{
    if (index >= m_entities.size() || !entity.state()) {
        // TODO: Warning?
        return;
    }
    m_entities.replace(index, entity);
}

// MARK: - Add Widget

auto kestrel::ui::game_scene::add_widget(const luabridge::LuaRef &widget) -> void
{
    auto scene = internal_scene();

    if (lua::ref_isa<widgets::text_widget>(widget)) {
        auto text = widget.cast<widgets::text_widget::lua_reference>();
        text->attach_responder_chain(&m_responder_chain);
        text->become_first_responder();
        m_responder_chain.add_mouse_responder(text.get());
    }
    else if (lua::ref_isa<ui::widgets::button_widget>(widget)) {
        auto button = widget.cast<ui::widgets::button_widget::lua_reference>();
        if (button->label().empty()) {
            return;
        }

        m_responder_chain.add_mouse_responder(button.get());
    }
    else if (lua::ref_isa<ui::widgets::list_widget>(widget)) {
        auto list = widget.cast<ui::widgets::list_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(list.get());
    }
    else if (lua::ref_isa<ui::widgets::grid_widget>(widget)) {
        auto grid = widget.cast<ui::widgets::grid_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(grid.get());
    }
    else if (lua::ref_isa<ui::widgets::scrollview_widget>(widget)) {
        auto scroll = widget.cast<ui::widgets::scrollview_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(scroll.get());
    }
    else if (lua::ref_isa<ui::widgets::image_widget>(widget)) {
        auto image = widget.cast<ui::widgets::image_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(image.get());
    }
    else if (lua::ref_isa<ui::widgets::checkbox_widget>(widget)) {
        auto checkbox = widget.cast<ui::widgets::checkbox_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(checkbox.get());
    }
    else if (lua::ref_isa<ui::widgets::popup_button_widget>(widget)) {
        auto popup = widget.cast<ui::widgets::popup_button_widget::lua_reference>();
        m_responder_chain.add_mouse_responder(popup.get());
    }

    m_widgets.emplace_back(widget);
}

auto kestrel::ui::game_scene::draw_widgets() const -> void
{
    scene_entity::lua_reference entity { nullptr };

    for (const auto& widget : m_widgets) {
        if (!widget.isUserdata()) {
            continue;
        }

        if (lua::ref_isa<ui::widgets::label_widget>(widget)) {
            auto label = widget.cast<ui::widgets::label_widget::lua_reference>();
            entity = label->entity();
            label->draw();
        }
        else if (lua::ref_isa<ui::widgets::text_widget>(widget)) {
            auto text = widget.cast<ui::widgets::text_widget::lua_reference>();
            entity = text->entity();
            text->draw();
        }
        else if (lua::ref_isa<ui::widgets::custom_widget>(widget)) {
            auto custom = widget.cast<ui::widgets::custom_widget::lua_reference>();
            entity = custom->entity();
            custom->draw();
        }
        else if (lua::ref_isa<ui::widgets::image_widget>(widget)) {
            auto image = widget.cast<ui::widgets::image_widget::lua_reference>();
            entity = image->entity();
            image->draw();
        }
        else if (lua::ref_isa<ui::widgets::button_widget>(widget)) {
            auto button = widget.cast<ui::widgets::button_widget::lua_reference>();
            entity = button->entity();
            button->draw();
        }
        else if (lua::ref_isa<ui::widgets::grid_widget>(widget)) {
            auto grid = widget.cast<ui::widgets::grid_widget::lua_reference>();
            entity = grid->entity();
            grid->draw();
        }
        else if (lua::ref_isa<ui::widgets::list_widget>(widget)) {
            auto list = widget.cast<ui::widgets::list_widget::lua_reference>();
            entity = list->entity();
            list->draw();
        }
        else if (lua::ref_isa<ui::widgets::textarea_widget>(widget)) {
            auto text = widget.cast<ui::widgets::textarea_widget::lua_reference>();
            entity = text->entity();
            text->draw();
        }
        else if (lua::ref_isa<ui::widgets::scrollview_widget>(widget)) {
            auto scroll = widget.cast<ui::widgets::scrollview_widget::lua_reference>();
            entity = scroll->entity();
            scroll->draw();
        }
        else if (lua::ref_isa<ui::widgets::sprite_widget>(widget)) {
            auto sprite = widget.cast<ui::widgets::sprite_widget::lua_reference>();
            entity = sprite->entity();
            sprite->draw();
        }
        else if (lua::ref_isa<ui::widgets::checkbox_widget>(widget)) {
            auto checkbox = widget.cast<ui::widgets::checkbox_widget::lua_reference>();
            entity = checkbox->entity();
            checkbox->draw();
        }
        else if (lua::ref_isa<ui::widgets::popup_button_widget>(widget)) {
            auto popup = widget.cast<ui::widgets::popup_button_widget::lua_reference>();
            entity = popup->entity();
            popup->draw();
        }
        else {
            // TODO: Unrecognised widget type... skip.
            continue;
        }

        entity->set_anchor_point(layout::axis_origin::top_left);
        m_positioning_frame->position_scene_entity(entity);
        entity->layout();
        entity->draw();
    }

    if (m_menu_widget.get()) {
        m_menu_widget->entity()->set_anchor_point(layout::axis_origin::top_left);
        m_positioning_frame->position_scene_entity(m_menu_widget->entity());
        m_menu_widget->entity()->layout();
        m_menu_widget->entity()->draw();
    }
}

// MARK: - Key States

auto kestrel::ui::game_scene::key(std::int32_t k) const -> event::lua_reference
{
    if (m_key_states.find(k) == m_key_states.end()) {
        return { new event(event::key(::ui::event::none, static_cast<::ui::hid::key>(k))) };
    }
    return m_key_states.at(k);
}

auto kestrel::ui::game_scene::is_key_down(std::int32_t k) const -> bool
{
    const auto& key = this->key(k);
    return key->has(::ui::event::key_down) || key->has(::ui::event::key_repeat);
}

auto kestrel::ui::game_scene::is_key_released(std::int32_t k) const -> bool
{
    const auto& key = this->key(k);
    return key->has(::ui::event::key_up);
}

// MARK: - Script Importing

static std::vector<std::string> s_imported_containers;

auto kestrel::ui::game_scene::import_supporting_scripts(const luabridge::LuaRef& ref) -> void
{
    if (!lua::ref_isa<resource::container>(ref)) {
        return;
    }
    auto container = ref.cast<resource::container::lua_reference>();

    if (std::find(s_imported_containers.begin(), s_imported_containers.end(), container->primary_name()) != s_imported_containers.end()) {
        return;
    }

    auto scripts = container->typed_resource(lua::script::resource_type::code)->matching_resources();
    for (const auto& script_ref : scripts) {
        if (script_ref->id == initial_script_id) {
            continue;
        }

        lua::script script(kestrel::lua_runtime(), script_ref);
        script.execute();
    }

    s_imported_containers.emplace_back(container->primary_name());
}

// MARK: - Direct Drawing

auto kestrel::ui::game_scene::draw_line(const math::point &p, const math::point &q, const graphics::color::lua_reference &color, float weight) -> void
{
    renderer::draw_line(p, q, renderer::blending::normal, *color.get(), weight);
}

// MARK: - Physics

auto kestrel::ui::game_scene::adopt_physics_body(physics::body::lua_reference body) -> void
{
    body->migrate_to_world(m_world);
}

// MARK: - Menu Widgets

auto kestrel::ui::game_scene::menu_widget() const -> widgets::menu_widget::lua_reference
{
    return m_menu_widget;
}

auto kestrel::ui::game_scene::set_menu_widget(const widgets::menu_widget::lua_reference &menu) -> void
{
    m_menu_widget = menu;
}

// MARK: - Bindings

auto kestrel::ui::game_scene::bind(const luabridge::LuaRef &bindings) -> void
{
    if (bindings.state() && bindings.isTable() || bindings.isUserdata()) {
        m_bindings = bindings;
    }
}

auto kestrel::ui::game_scene::bindings() const -> luabridge::LuaRef
{
    if (m_bindings.state()) {
        return m_bindings;
    }
    return { nullptr };
}

auto kestrel::ui::game_scene::find_function(const std::string &name) const -> luabridge::LuaRef
{
    if (m_bindings.isUserdata()) {

    }
    else if (m_bindings.isTable()) {
        auto item = m_bindings[name];
        if (item.isFunction()) {
            return item;
        }
    }
    return { nullptr };
}

// MARK: - Dialog

auto kestrel::ui::game_scene::dialog() const -> ui::dialog::lua_reference
{
    return m_dialog;
}

auto kestrel::ui::game_scene::set_dialog(const ui::dialog::lua_reference& dialog) -> void
{
    m_dialog = dialog;
}