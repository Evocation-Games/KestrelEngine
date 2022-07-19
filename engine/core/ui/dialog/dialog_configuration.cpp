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
#include <libGraphite/rsrc/manager.hpp>
#include "core/ui/dialog/dialog_configuration.hpp"
#include "core/asset/scene_interface.hpp"
#include "core/asset/legacy/macintosh/item_list.hpp"
#include "core/ui/dialog/dialog.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto ui::dialog_configuration::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("UI")
            .beginClass<dialog_configuration>("DialogConfiguration")
                .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
                .addProperty("passthrough", &dialog_configuration::passthrough, &dialog_configuration::set_passthrough)
                .addProperty("size", &dialog_configuration::size, &dialog_configuration::set_size)
                .addFunction("setBackground", &dialog_configuration::set_background)
                .addFunction("setStretchedBackground", &dialog_configuration::set_stretched_background)
                .addFunction("defineElement", &dialog_configuration::define_element)
                .addFunction("element", &dialog_configuration::element)
                .addFunction("build", &dialog_configuration::build)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::dialog_configuration::dialog_configuration(const luabridge::LuaRef &layout)
    : m_layout(layout, {})
{
}

// MARK: - Configuration

auto ui::dialog_configuration::load_image_asset(const luabridge::LuaRef &ref) -> asset::static_image::lua_reference
{
    if (scripting::lua::ref_isa<asset::resource_descriptor>(ref)) {
        auto descriptor = ref.cast<asset::resource_descriptor::lua_reference>();
        return { new asset::static_image(descriptor) };
    }
    else if (scripting::lua::ref_isa<asset::static_image>(ref)) {
        return ref.cast<asset::static_image::lua_reference>();
    }
    else {
        throw std::runtime_error("Invalid object type provided for DialogConfiguration image asset.");
    }
}

auto ui::dialog_configuration::set_background(const luabridge::LuaRef &background) -> void
{
    m_background_image = nullptr;

    if (!background.state()) {
        return;
    }

    m_background_image = load_image_asset(background);
}

auto ui::dialog_configuration::set_stretched_background(const luabridge::LuaRef &top, const luabridge::LuaRef &fill, const luabridge::LuaRef &bottom) -> void
{
    m_background_image = nullptr;
    m_background_bottom_image = nullptr;
    m_background_stretch_image = nullptr;

    if (top.state()) {
        m_background_image = load_image_asset(top);
    }

    if (fill.state()) {
        m_background_stretch_image = load_image_asset(fill);
    }

    if (bottom.state()) {
        m_background_bottom_image = load_image_asset(bottom);
    }
}

auto ui::dialog_configuration::define_element(const luabridge::LuaRef& index, const std::string &name, uint8_t type) -> control_definition::lua_reference
{
    control_definition::lua_reference def(new control_definition(&m_layout, name, static_cast<enum control_definition::type>(type)));

    if (index.state() && index.isNumber()) {
        def->add_element_index_vector(index.cast<uint32_t>());
    }
    else if (index.state() && index.isTable()) {
        auto length = index.length();
        for (auto i = 1; i <= length; ++i) {
            def->add_element_index_vector(index[i]);
        }
    }

    m_element_definitions.emplace(std::pair(name, def));
    return def;
}

auto ui::dialog_configuration::element(const std::string &name) -> control_definition::lua_reference
{
    auto it = m_element_definitions.find(name);
    if (it == m_element_definitions.end()) {
        return nullptr;
    }
    return it->second;
}

auto ui::dialog_configuration::all_elements() const -> std::vector<std::string>
{
    std::vector<std::string> elements;

    for (auto& element : m_element_definitions) {
        elements.emplace_back(element.first);
    }

    return elements;
}

// MARK: - Building

auto ui::dialog_configuration::build(const luabridge::LuaRef &configure_callback) -> ui::dialog::lua_reference
{
    if (m_dialog.get()) {
        throw std::runtime_error("Dialog has already been built.");
    }

    m_dialog = dialog::lua_reference(new dialog(this));

    if (configure_callback.state() && configure_callback.isFunction()) {
        configure_callback(m_dialog);
        m_dialog->present();
    }

    return m_dialog;
}