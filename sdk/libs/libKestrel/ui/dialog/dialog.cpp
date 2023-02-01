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
#include <libKestrel/ui/dialog/dialog.hpp>
#include <libKestrel/ui/scene/interface.hpp>
#include <libKestrel/ui/dialog/dialog_configuration.hpp>
#include <libKestrel/ui/legacy/macintosh/dialog.hpp>
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>

// Widgets
#include <libKestrel/ui/widgets/button_widget.hpp>
#include <libKestrel/ui/widgets/label_widget.hpp>
#include <libKestrel/ui/widgets/image_widget.hpp>
#include <libKestrel/ui/widgets/textarea_widget.hpp>
#include <libKestrel/ui/widgets/list_widget.hpp>
#include <libKestrel/ui/widgets/grid_widget.hpp>
#include <libKestrel/ui/widgets/custom_widget.hpp>
#include <libKestrel/ui/widgets/sprite_widget.hpp>
#include <libKestrel/ui/widgets/scrollview_widget.hpp>
#include <libKestrel/ui/widgets/text_widget.hpp>
#include <libKestrel/ui/widgets/checkbox_widget.hpp>
#include <libKestrel/ui/widgets/popup_button_widget.hpp>

// Controls
#include <libKestrel/ui/imgui/imgui.hpp>

// MARK: - Construction

kestrel::ui::dialog::dialog(dialog_configuration *config)
{
    load_contents(config);
}

auto kestrel::ui::dialog::load_contents(dialog_configuration *config) -> void
{
    m_configuration = config;

    switch (config->layout()->mode()) {
        case dialog_render_mode::scene: {
            load_scene_contents(config);
            break;
        }
        case dialog_render_mode::imgui: {
            load_imgui_contents(config);
            break;
        }
    }
}

auto kestrel::ui::dialog::load_imgui_contents(dialog_configuration *config) -> void
{
    auto L = kestrel::lua_runtime()->internal_state();

    // We're working with ImGUI, so use the ImGUI Controls
    for (const auto& element_name : config->all_elements()) {
        const auto& element = config->element(element_name);

        switch (static_cast<enum control_type>(element->type())) {
            case control_type::button: {
                auto button = imgui::button::lua_reference(new imgui::button(element->suggested_value()));
                button->set_position(element->frame().origin());
                button->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, button)));
                break;
            }
            case control_type::checkbox: {
                auto checkbox = imgui::checkbox::lua_reference(new imgui::checkbox(element->suggested_value(), false));
                checkbox->set_position(element->frame().origin());
                checkbox->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, checkbox)));
                break;
            }
            case control_type::popup_button: {
                // TODO: Take the suggested value, and extract items.
                auto combo = imgui::combo::lua_reference(new imgui::combo({ nullptr }));
                combo->set_position(element->frame().origin());
                combo->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, combo)));
                break;
            }
            case control_type::image: {
                auto image = imgui::image::lua_reference(new imgui::image({ nullptr }));
                image->set_position(element->frame().origin());
                image->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, image)));
                break;
            }
            case control_type::label: {
                auto label = imgui::label::lua_reference(new imgui::label(element->suggested_value()));
                label->set_position(element->frame().origin());
                label->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, label)));
                break;
            }
            case control_type::text_field: {
                auto value = element->suggested_value();
                auto text_field = imgui::textfield::lua_reference(new imgui::textfield(value.size(), value));
                text_field->set_position(element->frame().origin());
                text_field->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, text_field)));
                break;
            }
            case control_type::slider: {
                auto slider = imgui::slider::lua_reference(new imgui::slider(50, 0, 100));
                slider->set_position(element->frame().origin());
                slider->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, slider)));
                break;
            }
            case control_type::tabbar: {
                auto tabbar = imgui::tabbar::lua_reference(new imgui::tabbar({ nullptr }));
                tabbar->set_position(element->frame().origin());
                tabbar->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, tabbar)));
                break;
            }
            case control_type::table:
            case control_type::list: {
                auto table = imgui::table::lua_reference(new imgui::table(0));
                table->set_position(element->frame().origin());
                table->set_size(element->frame().size());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, table)));
                break;
            }

            default: break;
        }
    }
}

auto kestrel::ui::dialog::load_scene_contents(dialog_configuration *config) -> void
{
    auto L = kestrel::lua_runtime()->internal_state();

    // Adopt any additional aspects of layout and configuration information provided.
    m_scene_ui.frame_size = config->size();

    // We're working with the native kestrel scene, so use the widgets
    for (auto element_name : config->all_elements()) {
        const auto& element = config->element(element_name);

        switch (static_cast<enum control_type>(element->type())) {
            case control_type::button: {
                auto button = widgets::button_widget::lua_reference(new widgets::button_widget(element->suggested_value()));
                button->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, button)));
                break;
            }
            case control_type::sprite: {
                auto image = widgets::sprite_widget::lua_reference(new widgets::sprite_widget({ nullptr }));
                image->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, image)));
                break;
            }
            case control_type::image: {
                auto sprite = widgets::image_widget::lua_reference(new widgets::image_widget({ nullptr }));
                sprite->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, sprite)));
                break;
            }
            case control_type::checkbox: {
                auto checkbox = widgets::checkbox_widget::lua_reference(new widgets::checkbox_widget());
                checkbox->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, checkbox)));
                break;
            }
            case control_type::label: {
                auto label = widgets::label_widget::lua_reference(new widgets::label_widget(element->suggested_value()));
                label->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, label)));
                break;
            }
            case control_type::text_field: {
                auto text = widgets::text_widget::lua_reference(new widgets::text_widget(element->frame().width()));
                text->set_text(element->suggested_value());
                text->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, text)));
                break;
            }
            case control_type::text_area: {
                auto text = widgets::textarea_widget::lua_reference(new widgets::textarea_widget(element->suggested_value()));
                text->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, text)));
                break;
            }
            case control_type::popup_button: {
                auto popup = widgets::popup_button_widget::lua_reference(new widgets::popup_button_widget(element->frame().width()));
                popup->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, popup)));
                break;
            }
            case control_type::table:
            case control_type::list: {
                auto list = widgets::list_widget::lua_reference(new widgets::list_widget());
                list->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, list)));
                break;
            }
            case control_type::grid: {
                auto grid = widgets::grid_widget::lua_reference(new widgets::grid_widget());
                grid->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, grid)));
                break;
            }
            case control_type::canvas: {
                auto custom = widgets::custom_widget::lua_reference(new widgets::custom_widget({ nullptr }));
                custom->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, custom)));
                break;
            }
            case control_type::scroll_area: {
                auto scroll = widgets::scrollview_widget::lua_reference(new widgets::scrollview_widget());
                scroll->set_frame(element->frame());
                m_elements.emplace(std::pair(element_name, luabridge::LuaRef(L, scroll)));
                break;
            }

            default: break;
        }
    }
}


// MARK: - Accessors

auto kestrel::ui::dialog::frame() const -> math::rect
{
    return m_frame;
}

// MARK: - Presentation

auto kestrel::ui::dialog::present() -> void
{
    auto scene = kestrel::session().current_scene();
    switch (m_configuration->layout()->mode()) {
        case dialog_render_mode::scene: {
            present_scene(scene);
            break;
        }
        case dialog_render_mode::imgui: {
            present_imgui(scene);
            break;
        }
    }
}

auto kestrel::ui::dialog::present_imgui(const ui::game_scene::lua_reference& scene) -> void
{
    auto& presentation_scene = scene;
    auto layout = m_configuration->layout();
    add_to_scene(presentation_scene);

    kestrel::load_imgui_environment({ kestrel::lua_runtime()->internal_state() });

    if ((layout->flags() & scene_interface_flags::imgui_show_title) == scene_interface_flags::imgui_show_title) {
        m_imgui.window = ui::imgui::window::create(m_name, m_frame.size());
    }
    else {
        m_imgui.window = ui::imgui::window::create("", m_frame.size());
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

auto kestrel::ui::dialog::present_scene(const ui::game_scene::lua_reference& scene) -> void
{
    auto& presentation_scene = scene;
    add_to_scene(presentation_scene);

    if (m_configuration->background().get()) {
        // Setup the background of the dialog in the correct position.
        if (m_configuration->background_stretch().get() && m_configuration->background_bottom().get()) {
            set_stretchable_background(
                m_configuration->size(),
                { kestrel::lua_runtime()->internal_state(), m_configuration->background() },
                { kestrel::lua_runtime()->internal_state(), m_configuration->background_stretch() },
                { kestrel::lua_runtime()->internal_state(), m_configuration->background_bottom() }
            );
        }
        else {
            set_background({ kestrel::lua_runtime()->internal_state(), m_configuration->background() });
        }
    }

    for (auto& element_name : m_configuration->all_elements()) {
        const auto& it = m_elements.find(element_name);
        if (it != m_elements.end()) {
            auto widget = it->second;
            if (widget.state()) {
                m_owner_scene->add_widget(widget);
            }
        }
    }
}

// MARK: - Functions

auto kestrel::ui::dialog::add_to_scene(const ui::game_scene::lua_reference &scene) -> void
{
    m_owner_scene = scene;
    scene->set_passthrough_render(m_configuration->passthrough());
}

auto kestrel::ui::dialog::set_background(const luabridge::LuaRef &background) -> void
{
    m_background.top = { nullptr };
    m_background.fill = { nullptr };
    m_background.bottom = { nullptr };
    m_background.top_entity = { nullptr };
    m_background.fill_entity = { nullptr };
    m_background.bottom_entity = { nullptr };

    if (lua::ref_isa<image::static_image>(background)) {
        m_background.fill = background.cast<image::static_image::lua_reference>();
        auto size = m_scene_ui.frame_size;
        m_owner_scene->set_positioning_frame({
            new layout::positioning_frame(size, layout::axis_origin::center, layout::scaling_mode::normal)
        });
        m_owner_scene->positioning_frame()->set_axis_displacement({ -size.width() / 2.f, -size.height() / 2.f });

        m_background.fill_entity = { new ui::scene_entity(m_background.fill) };
        m_background.fill_entity->set_anchor_point(layout::axis_origin::top_left);
        m_background.fill_entity->set_render_size(size);
        m_background.fill_entity->set_draw_size(size);
        m_owner_scene->add_scene_entity(m_background.fill_entity);
    }
}

auto kestrel::ui::dialog::set_stretchable_background(const math::size& size, const luabridge::LuaRef& top, const luabridge::LuaRef& fill, const luabridge::LuaRef& bottom) -> void
{
    m_owner_scene->set_positioning_frame({
        new layout::positioning_frame(size, layout::axis_origin::center, layout::scaling_mode::normal)
    });
    m_owner_scene->positioning_frame()->set_axis_displacement({ -size.width() / 2.f, -size.height() / 2.f });

    if (lua::ref_isa<image::static_image>(top)) {
        m_background.top = top.cast<image::static_image::lua_reference>();
        m_background.top_entity = { new ui::scene_entity(m_background.top) };
        m_background.top_entity->set_position({ 0, 0 });
        m_background.top_entity->set_anchor_point(layout::axis_origin::top_left);
    }

    if (lua::ref_isa<image::static_image>(bottom)) {
        m_background.bottom = bottom.cast<image::static_image::lua_reference>();
        m_background.bottom_entity = { new ui::scene_entity(m_background.bottom) };
        m_background.bottom_entity->set_position({ 0 , size.height() - m_background.bottom_entity->size().height() });
        m_background.bottom_entity->set_anchor_point(layout::axis_origin::top_left);
    }

    if (lua::ref_isa<image::static_image>(fill)) {
        m_background.fill = fill.cast<image::static_image::lua_reference>();

        auto height = size.height();
        auto y = 0.f;
        if (m_background.top_entity.get()) {
            height -= m_background.top_entity->size().height();
            y = m_background.top_entity->size().height();
        }
        if (m_background.bottom_entity.get()) {
            height -= m_background.bottom_entity->size().height();
        }

        graphics::canvas::lua_reference canvas(new graphics::canvas({ size.width(), height }) );
        math::rect fill_rect { 0, 0, m_background.fill->size().width(), height };
        canvas->draw_static_image(m_background.fill, fill_rect);
        m_background.fill_entity = { new ui::scene_entity(canvas) };
        m_background.fill_entity->set_position({ 0, y });
        m_background.fill_entity->set_anchor_point(layout::axis_origin::top_left);
    }

    m_owner_scene->add_scene_entity(m_background.fill_entity);
    m_owner_scene->add_scene_entity(m_background.top_entity);
    m_owner_scene->add_scene_entity(m_background.bottom_entity);
}

auto kestrel::ui::dialog::configure_element(const std::string &name, const luabridge::LuaRef &configure) -> void
{
    auto element = named_element(name);
    if (!element.state()) {
        return;
    }

    if (configure.state() && configure.isFunction()) {
        configure(element);
    }
}

auto kestrel::ui::dialog::named_element(const std::string &name) -> luabridge::LuaRef
{
    auto it = m_elements.find(name);
    if (it == m_elements.end()) {
        return { nullptr };
    }
    return it->second;
}

auto kestrel::ui::dialog::close() -> void
{
    if (m_imgui.window.get()) {
        m_imgui.window->close();
        kestrel::unload_imgui_environment({ kestrel::lua_runtime()->internal_state() });
    }
    ui::game_scene::back();
}
