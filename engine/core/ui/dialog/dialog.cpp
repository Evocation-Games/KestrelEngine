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
#include <algorithm>
#include "core/ui/dialog/dialog.hpp"
#include "core/asset/scene_interface.hpp"
#include "core/ui/dialog/dialog_configuration.hpp"
#include "core/asset/legacy/macintosh/dialog.hpp"
#include "core/ui/control_definition.hpp"
#include "core/environment.hpp"
#include "core/graphics/common/canvas.hpp"

// MARK: - Lua

auto ui::dialog::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginNamespace("UI")
            .beginClass<dialog>("Dialog")
                .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
                .addProperty("passthrough", &dialog::passthrough, &dialog::set_passthrough)
                .addFunction("setBackground", &dialog::set_background)
                .addFunction("setStretchableBackground", &dialog::set_stretchable_background)
                .addFunction("configureElement", &dialog::configure_element)
                .addFunction("elementNamed", &dialog::named_element)
                .addFunction("present", &dialog::present)
                .addFunction("presentInScene", &dialog::present_in_scene)
                .addFunction("close", &dialog::close)
                .addFunction("update", &dialog::update)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::dialog::dialog(dialog_configuration *config)
{
    load_contents(config);
}

ui::dialog::dialog(const luabridge::LuaRef &ref)
{
    if (ref.state() && scripting::lua::ref_isa<dialog_configuration>(ref)) {
        load_contents(ref.cast<dialog_configuration::lua_reference>().get());
    }
    else {
        throw std::runtime_error("Invalid parameter passed to Dialog constructor.");
    }
}

auto ui::dialog::load_contents(dialog_configuration *config) -> void
{
    m_configuration = config;

    if (!m_configuration) {
        throw std::runtime_error("Dialog construction requires DialogConfiguration");
    }

    auto target_size = m_configuration->size();
    auto layout = m_configuration->layout();

    if (scripting::lua::ref_isa<asset::scene_interface>(layout)) {
        auto scene_interface = layout.cast<asset::scene_interface::lua_reference>();
        auto flags = scene_interface->flags();

        m_name = scene_interface->name();
        m_frame = { math::point(), scene_interface->scene_size() };

        for (auto i = 0; i < scene_interface->child_count(); ++i) {
            auto child = scene_interface->child_at(i);

            // Define a new ControlDefinition from the item.
            ui::control_definition::lua_reference definition {
                new ui::control_definition(child.frame(), static_cast<uint32_t>(child.type()))
            };

            m_control_definitions.emplace(std::pair(child.identifier(), definition));
        }
    }
    else if (scripting::lua::ref_isa<asset::legacy::macintosh::toolbox::dialog>(layout)) {
        auto dialog = layout.cast<asset::legacy::macintosh::toolbox::dialog::lua_reference>();

        if (target_size.width == 0 && target_size.height == 0) {
            target_size = dialog->bounds().size;
        }

        asset::resource_descriptor::lua_reference ditl_ref(new asset::resource_descriptor());
        ditl_ref = ditl_ref->with_id(dialog->interface_list());

        m_item_list = { new asset::legacy::macintosh::toolbox::item_list(ditl_ref) };
        m_name = dialog->title();
        m_frame = dialog->bounds();

        for (auto name : m_configuration->defined_elements()) {
            auto type = static_cast<enum control_definition::type>(m_configuration->type_of_element_named(name));
            auto anchor = static_cast<enum control_definition::anchor>(m_configuration->anchor_of_element_named(name));
            auto index_vector = m_configuration->index_vector_for_element_named(name);

            double left = 10'000;
            double top = 10'000;
            double right = 0;
            double bottom = 0;

            for (auto index : index_vector) {
                auto item = m_item_list->at(index);
                left = std::min(left, item.frame.get_x());
                top = std::min(top, item.frame.get_y());
                right = std::max(right, item.frame.get_max_x());
                bottom = std::max(bottom, item.frame.get_max_y());
            }

            // Vertical Anchors
            if ((anchor & control_definition::anchor::vertical) == control_definition::anchor::vertical) {
                auto diff = m_frame.get_height() - bottom;
                bottom = target_size.get_height() - diff;
            }
            else if ((anchor & control_definition::anchor::bottom) == control_definition::anchor::bottom) {
                auto top_diff = m_frame.get_height() - top;
                auto bottom_diff = m_frame.get_height() - bottom;
                top = target_size.height - top_diff;
                bottom = target_size.height - bottom_diff;
            }

            // Horizontal Anchors
            if ((anchor & control_definition::anchor::horizontal) == control_definition::anchor::horizontal) {
                auto diff = m_frame.get_width() - right;
                right = target_size.get_width() - diff;
            }
            else if ((anchor & control_definition::anchor::right) == control_definition::anchor::right) {
                auto right_diff = m_frame.get_width() - right;
                auto left_diff = m_frame.get_width() - left;
                right = target_size.width - right_diff;
                left = target_size.width - left_diff;
            }

            ui::control_definition::lua_reference definition {
                new ui::control_definition({ left, top, right - left, bottom - top }, static_cast<uint8_t>(type))
            };
            m_control_definitions.emplace(std::pair(name, definition));
        }
    }

    // Finally identify the scene that we're being created within.
    if (auto env = environment::active_environment().lock()) {
        m_owner_scene = env->session()->current_scene();
    }
}

// MARK: - Accessors

auto ui::dialog::frame() const -> math::rect
{
    return m_frame;
}

// MARK: - Presentation

auto ui::dialog::present() -> void
{
    auto layout = m_configuration->layout();
    if (scripting::lua::ref_isa<asset::scene_interface>(layout)) {
        auto scene_interface = layout.cast<asset::scene_interface::lua_reference>();
        if ((scene_interface->flags() & asset::scene_interface::flags::use_imgui) == asset::scene_interface::flags::use_imgui) {
            present_imgui(m_owner_scene);
        }
        else {
            present_scene(m_owner_scene);
        }
    }
    else if (scripting::lua::ref_isa<asset::legacy::macintosh::toolbox::dialog>(layout)) {
        present_scene(m_owner_scene);
    }
}

auto ui::dialog::present_in_scene(const ui::game_scene::lua_reference &scene) -> void
{
    add_to_scene(scene);
    present();
}

auto ui::dialog::present_imgui(const ui::game_scene::lua_reference& scene) -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    auto presentation_scene = scene;
    if (m_configuration) {

    }
    auto layout = m_configuration->layout().cast<asset::scene_interface::lua_reference>();

    add_to_scene(presentation_scene);

    env->start_imgui_environment();

    if ((layout->flags() & asset::scene_interface::imgui_show_title) == asset::scene_interface::imgui_show_title) {
        m_imgui.window = ui::imgui::window::create(m_name, m_frame.size);
    }
    else {
        m_imgui.window = ui::imgui::window::create("", m_frame.size);
    }
    m_imgui.window->set_has_close_button(false);
    m_imgui.window->set_resizable(false);

    for (auto& definition : m_control_definitions) {
        definition.second->construct(static_cast<uint32_t>(control_definition::mode::imgui));
        auto control = definition.second->control();
        if (control.state()) {
            m_imgui.window->add_widget(control);
        }
    }

    m_imgui.window->center();
    m_imgui.window->show();
}

auto ui::dialog::present_scene(const ui::game_scene::lua_reference& scene) -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    auto presentation_scene = scene;
    if (m_configuration) {
    }

    add_to_scene(presentation_scene);

    if (m_configuration && m_configuration->background().get()) {
        // Setup the background of the dialog in the correct position.
        if (m_configuration->background_stretch().get() && m_configuration->background_bottom().get()) {
            set_stretchable_background(
                m_configuration->size(),
                { env->lua_runtime()->internal_state(), m_configuration->background() },
                { env->lua_runtime()->internal_state(), m_configuration->background_stretch() },
                { env->lua_runtime()->internal_state(), m_configuration->background_bottom() }
            );
        }
        else {
            set_background({ env->lua_runtime()->internal_state(), m_configuration->background() });
        }
    }

    for (const auto& definition : m_control_definitions) {
        if (definition.second->access_flag() && definition.second->frame().intersects(m_frame)) {
            definition.second->construct(static_cast<uint32_t>(control_definition::mode::scene));
            auto entity = definition.second->entity();
            if (entity.get()) {
                definition.second->set_entity_index(m_owner_scene->add_entity(entity));
            }
        }
    }
}

// MARK: - Functions

auto ui::dialog::add_to_scene(const ui::game_scene::lua_reference &scene) -> void
{
    m_owner_scene = scene;
    scene->set_passthrough_render(true);
}

auto ui::dialog::set_background(const luabridge::LuaRef &background) -> void
{
    m_background.top = { nullptr };
    m_background.fill = { nullptr };
    m_background.bottom = { nullptr };
    m_background.top_entity = { nullptr };
    m_background.fill_entity = { nullptr };
    m_background.bottom_entity = { nullptr };

    if (scripting::lua::ref_isa<asset::static_image>(background)) {
        m_background.fill = background.cast<asset::static_image::lua_reference>();
        m_positioning_offset = (m_owner_scene->size() * 0.5) - (m_background.fill->size() * 0.5);
        m_frame = { math::point(), m_background.fill->size() };
        m_owner_scene->set_positioning_frame({
            new layout::positioning_frame(m_frame, {}, { m_positioning_offset.width, m_positioning_offset.height })
        });

        m_background.fill_entity = { new ui::scene_entity(m_background.fill) };
        m_owner_scene->add_entity(m_background.fill_entity);
    }
}

auto ui::dialog::set_stretchable_background(const math::size& size, const luabridge::LuaRef& top, const luabridge::LuaRef& fill, const luabridge::LuaRef& bottom) -> void
{
    m_positioning_offset = (m_owner_scene->size() * 0.5) - (size * 0.5);
    m_frame = { math::point(), size };
    m_owner_scene->set_positioning_frame({
        new layout::positioning_frame(m_frame, {}, { m_positioning_offset.width, m_positioning_offset.height })
    });


    if (scripting::lua::ref_isa<asset::static_image>(top)) {
        m_background.top = top.cast<asset::static_image::lua_reference>();
        m_background.top_entity = { new ui::scene_entity(m_background.top) };
        m_background.top_entity->set_position({ 0, 0 });
    }

    if (scripting::lua::ref_isa<asset::static_image>(bottom)) {
        m_background.bottom = bottom.cast<asset::static_image::lua_reference>();
        m_background.bottom_entity = { new ui::scene_entity(m_background.bottom) };
        m_background.bottom_entity->set_position({ 0 , size.height - m_background.bottom_entity->size().height });
    }

    if (scripting::lua::ref_isa<asset::static_image>(fill)) {
        m_background.fill = fill.cast<asset::static_image::lua_reference>();

        auto height = size.height;
        auto y = 0;
        if (m_background.top_entity.get()) {
            height -= m_background.top_entity->size().height;
            y = m_background.top_entity->size().height;
        }
        if (m_background.bottom_entity.get()) {
            height -= m_background.bottom_entity->size().height;
        }

        graphics::canvas::lua_reference canvas(new graphics::canvas({ size.width, height }) );
        math::rect fill_rect { 0, 0, m_background.fill->size().width, height };
        canvas->draw_static_image(m_background.fill, fill_rect);
        m_background.fill_entity = { new ui::scene_entity(canvas) };

        m_background.fill_entity->set_position({ 0, static_cast<double>(y) });
    }

    m_owner_scene->add_entity(m_background.fill_entity);
    m_owner_scene->add_entity(m_background.top_entity);
    m_owner_scene->add_entity(m_background.bottom_entity);
}

auto ui::dialog::configure_element(const std::string &name, const luabridge::LuaRef &configure) -> void
{
    auto element = named_element(name);
    if (!element.get()) {
        return;
    }

    element->set_access_flag(true);

    if (configure.state() && configure.isFunction()) {
        configure(element);

        if (element->entity_index() == UINT32_MAX) {
            element->update();
            if (element->has_control()) {
                // TODO: Determine what to do for an ImGui Control
            }
            else if (element->has_entity()) {
                m_owner_scene->replace_entity(element->entity_index(), element->entity());
            }
        }
        else if (element->entity().get()) {
            element->update();
        }

    }
}

auto ui::dialog::named_element(const std::string &name) -> ui::control_definition::lua_reference
{
    auto it = m_control_definitions.find(name);
    if (it == m_control_definitions.end()) {
        return { nullptr };
    }

    return it->second;
}

auto ui::dialog::close() -> void
{
    if (m_imgui.window.get()) {
        m_imgui.window->close();

        if (auto env = environment::active_environment().lock()) {
            env->end_imgui_environment({ env->lua_runtime()->internal_state() });
        }
    }
    ui::game_scene::back();
}

auto ui::dialog::update() -> void
{
    for (auto def : m_control_definitions) {
        def.second->update();
    }
}