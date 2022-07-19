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

// Widgets
#include "core/ui/widgets/button_widget.hpp"
#include "core/ui/widgets/label_widget.hpp"
#include "core/ui/widgets/image_widget.hpp"
#include "core/ui/widgets/textarea_widget.hpp"
#include "core/ui/widgets/list_widget.hpp"
#include "core/ui/widgets/grid_widget.hpp"
#include "core/ui/widgets/custom_widget.hpp"
#include "core/ui/widgets/text_widget.hpp"

// Controls
#include "core/ui/imgui/imgui.hpp"


// MARK: - Lua

auto ui::dialog::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginNamespace("UI")
            .beginClass<dialog>("Dialog")
                .addFunction("setBackground", &dialog::set_background)
                .addFunction("setStretchableBackground", &dialog::set_stretchable_background)
                .addFunction("configureElement", &dialog::configure_element)
                .addFunction("elementNamed", &dialog::named_element)
                .addFunction("present", &dialog::present)
                .addFunction("close", &dialog::close)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::dialog::dialog(dialog_configuration *config)
{
    load_contents(config);
}

auto ui::dialog::load_contents(dialog_configuration *config) -> void
{
    m_configuration = config;

    switch (config->layout()->mode()) {
        case dialog_layout::mode::scene: {
            load_scene_contents(config);
            break;
        }
        case dialog_layout::mode::imgui: {
            load_imgui_contents(config);
            break;
        }
    }
}

auto ui::dialog::load_imgui_contents(dialog_configuration *config) -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        // TODO: Handle this scenario.
        return;
    }
    auto L = env->lua_runtime()->internal_state();

    // We're working with ImGUI, so use the ImGUI Controls
    for (const auto& element_name : config->all_elements()) {
        const auto& element = config->element(element_name);

        switch (static_cast<enum control_definition::type>(element->type())) {
            case control_definition::type::button: {
                auto button = imgui::button::lua_reference(new imgui::button(element->suggested_value(), { nullptr }));
                button->set_position(element->frame().origin);
                button->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, button)));
                break;
            }
            case control_definition::type::checkbox: {
                auto checkbox = imgui::checkbox::lua_reference(new imgui::checkbox(element->suggested_value(), false));
                checkbox->set_position(element->frame().origin);
                checkbox->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, checkbox)));
                break;
            }
            case control_definition::type::popup_button: {
                // TODO: Take the suggested value, and extract items.
                auto combo = imgui::combo::lua_reference(new imgui::combo({ nullptr }));
                combo->set_position(element->frame().origin);
                combo->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, combo)));
                break;
            }
            case control_definition::type::image: {
                auto image = imgui::image::lua_reference(new imgui::image({ nullptr }));
                image->set_position(element->frame().origin);
                image->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, image)));
                break;
            }
            case control_definition::type::label: {
                auto label = imgui::label::lua_reference(new imgui::label(element->suggested_value()));
                label->set_position(element->frame().origin);
                label->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, label)));
                break;
            }
            case control_definition::type::text_field: {
                auto value = element->suggested_value();
                auto text_field = imgui::textfield::lua_reference(new imgui::textfield(value.size(), value));
                text_field->set_position(element->frame().origin);
                text_field->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, text_field)));
                break;
            }
            case control_definition::type::slider: {
                auto slider = imgui::slider::lua_reference(new imgui::slider(50, 0, 100));
                slider->set_position(element->frame().origin);
                slider->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, slider)));
                break;
            }
            case control_definition::type::tabbar: {
                auto tabbar = imgui::tabbar::lua_reference(new imgui::tabbar({ nullptr }));
                tabbar->set_position(element->frame().origin);
                tabbar->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, tabbar)));
                break;
            }
            case control_definition::type::table:
            case control_definition::type::list: {
                auto table = imgui::table::lua_reference(new imgui::table(0));
                table->set_position(element->frame().origin);
                table->set_size(element->frame().size);
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, table)));
                break;
            }

            default: break;
        }
    }
}

auto ui::dialog::load_scene_contents(dialog_configuration *config) -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        // TODO: Handle this scenario.
        return;
    }
    auto L = env->lua_runtime()->internal_state();

    // We're working with the native kestrel scene, so use the widgets
    for (auto element_name : config->all_elements()) {
        const auto& element = config->element(element_name);

        switch (static_cast<enum control_definition::type>(element->type())) {
            case control_definition::type::button: {
                auto button = widgets::button_widget::lua_reference(new widgets::button_widget(element->suggested_value()));
                button->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, button)));
                break;
            }
            case control_definition::type::image: {
                auto image = widgets::image_widget::lua_reference(new widgets::image_widget({ nullptr }));
                image->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, image)));
                break;
            }
            case control_definition::type::label: {
                auto label = widgets::label_widget::lua_reference(new widgets::label_widget(element->suggested_value()));
                label->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, label)));
                break;
            }
            case control_definition::type::text_area: {
                auto text = widgets::textarea_widget::lua_reference(new widgets::textarea_widget(element->suggested_value()));
                text->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, text)));
                break;
            }
            case control_definition::type::table:
            case control_definition::type::list: {
                auto list = widgets::list_widget::lua_reference(new widgets::list_widget());
                list->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, list)));
                break;
            }
            case control_definition::type::grid: {
                auto grid = widgets::grid_widget::lua_reference(new widgets::grid_widget());
                grid->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, grid)));
                break;
            }
            case control_definition::type::canvas: {
                auto custom = widgets::custom_widget::lua_reference(new widgets::custom_widget({ nullptr }));
                custom->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, custom)));
                break;
            }

            default: break;
        }
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
    auto env = environment::active_environment().lock();
    if (!env) {
        // TODO: Handle this...
        return;
    }
    auto scene = env->session()->current_scene();

    switch (m_configuration->layout()->mode()) {
        case dialog_layout::mode::scene: {
            present_scene(scene);
            break;
        }
        case dialog_layout::mode::imgui: {
            present_imgui(scene);
            break;
        }
    }
}

auto ui::dialog::present_imgui(const ui::game_scene::lua_reference& scene) -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    auto& presentation_scene = scene;
    auto layout = m_configuration->layout();
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

    for (auto& element : m_elements) {
        auto control = element.second;
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

    auto& presentation_scene = scene;
    add_to_scene(presentation_scene);

    if (m_configuration->background().get()) {
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

    for (auto& element : m_elements) {
        auto widget = element.second;
        if (widget.state()) {
            m_owner_scene->add_widget(widget);
        }
    }
}

// MARK: - Functions

auto ui::dialog::add_to_scene(const ui::game_scene::lua_reference &scene) -> void
{
    m_owner_scene = scene;
    scene->set_passthrough_render(m_configuration->passthrough());
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
    if (!element.state()) {
        return;
    }

    if (configure.state() && configure.isFunction()) {
        configure(element);
    }
}

auto ui::dialog::named_element(const std::string &name) -> luabridge::LuaRef
{
    auto it = m_elements.find(name);
    if (it == m_elements.end()) {
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
