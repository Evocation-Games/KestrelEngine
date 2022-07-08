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
#include "core/asset/legacy/macintosh/dialog.hpp"
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
                .addProperty("size", &dialog_configuration::size, &dialog_configuration::set_size)
                .addFunction("setBackground", &dialog_configuration::set_background)
                .addFunction("setStretchedBackground", &dialog_configuration::set_stretched_background)
                .addFunction("defineElement", &dialog_configuration::define_element)
                .addFunction("setElementName", &dialog_configuration::set_element_name)
                .addFunction("setTypeOfElementNamed", &dialog_configuration::set_type_of_element_named)
                .addFunction("setAnchorOfElementNamed", &dialog_configuration::set_anchor_of_element_named)
                .addFunction("typeOfElementNamed", &dialog_configuration::type_of_element_named)
                .addFunction("anchorOfElementNamed", &dialog_configuration::anchor_of_element_named)
                .addFunction("build", &dialog_configuration::build)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::dialog_configuration::dialog_configuration(const luabridge::LuaRef &layout)
{
    if (scripting::lua::ref_isa<asset::resource_descriptor>(layout)) {
        // We have received a resource descriptor. This could have a type and id, or just an id.
        auto descriptor = layout.cast<asset::resource_descriptor::lua_reference>();

        // Make sure there is an ID
        if (!descriptor->has_id()) {
            throw std::runtime_error("Invalid resource descriptor passed to dialog configuration: " + descriptor->description());
        }

        // If there is no type, then determine the best type in the following order: scïn, DLOG/DITL
        if (!descriptor->has_type()) {
            if (graphite::rsrc::manager::shared_manager().find(asset::scene_interface::type, descriptor->id)) {
                descriptor = descriptor->with_type(asset::scene_interface::type);
            }
            else if (graphite::rsrc::manager::shared_manager().find(asset::legacy::macintosh::toolbox::dialog::type, descriptor->id)) {
                descriptor = descriptor->with_type(asset::legacy::macintosh::toolbox::dialog::type);
            }
            else {
                throw std::runtime_error("No matching resource for resource descriptor passed to dialog configuration: " + descriptor->description());
            }
        }

        // Determine the type of the descriptor, and then load the appropriate asset.
        if (descriptor->type == asset::scene_interface::type) {
            asset::scene_interface::lua_reference interface { new asset::scene_interface(descriptor) };
            m_layout_ref = { layout.state(), interface };
        }
        else if (descriptor->type == asset::legacy::macintosh::toolbox::dialog::type) {
            asset::legacy::macintosh::toolbox::dialog::lua_reference dialog { new asset::legacy::macintosh::toolbox::dialog(descriptor) };
            m_layout_ref = { layout.state(), dialog };
        }
        else {
            throw std::runtime_error("Unknown resource descriptor type passed to dialog configuration: " + descriptor->description());
        }
    }
    else if (scripting::lua::ref_isa<asset::scene_interface>(layout)) {
        // The user has directly supplied a scene interface (scïn)
        m_layout_ref = layout;
    }
    else if (scripting::lua::ref_isa<asset::legacy::macintosh::toolbox::dialog>(layout)) {
        // The user has directly supplied a DLOG resource.
        m_layout_ref = layout;
    }
    else {
        throw std::runtime_error("Unknown DialogConfiguration layout reference.");
    }
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

auto ui::dialog_configuration::define_element(const luabridge::LuaRef& index, const std::string &name, uint8_t type) -> void
{
    element_definition def;

    if (index.state() && index.isNumber()) {
        def.index = { index.cast<uint32_t>() };
    }
    else if (index.state() && index.isTable()) {
        auto length = index.length();
        for (auto i = 1; i <= length; ++i) {
            def.index.emplace_back(index[i]);
        }
    }

    def.type = static_cast<enum control_definition::type>(type);
    m_elements.emplace(std::pair(name, def));
}

auto ui::dialog_configuration::set_element_name(uint32_t idx, const std::string &name) -> void
{
    // TODO: Find the element with the specified index and move it.
}

auto ui::dialog_configuration::set_type_of_element_named(const std::string &name, uint8_t type) -> void
{
    m_elements.at(name).type = static_cast<enum control_definition::type>(type);
}

auto ui::dialog_configuration::set_anchor_of_element_named(const std::string &name, uint8_t anchor) -> void
{
    m_elements.at(name).anchor = static_cast<enum control_definition::anchor>(anchor);
}

auto ui::dialog_configuration::index_for_element_named(const std::string &name) const -> luabridge::LuaRef
{
    if (auto env = environment::active_environment().lock()) {
        auto table = luabridge::LuaRef::newTable(env->lua_runtime()->internal_state());
        auto item = m_elements.at(name);

        for (auto index : item.index) {
            table[table.length() + 1] = index;
        }

        return table;
    }
    return { nullptr };
}

auto ui::dialog_configuration::index_vector_for_element_named(const std::string &name) const -> std::vector<uint32_t>
{
    return m_elements.at(name).index;
}

auto ui::dialog_configuration::type_of_element_named(const std::string &name) const -> uint8_t
{
    return static_cast<uint8_t>(m_elements.at(name).type);
}

auto ui::dialog_configuration::anchor_of_element_named(const std::string &name) const -> uint8_t
{
    return static_cast<uint32_t>(m_elements.at(name).anchor);
}

auto ui::dialog_configuration::name_of_element(uint32_t idx) const -> std::string
{
    for (auto it = m_elements.begin(); it != m_elements.end(); ++it) {
        const auto& index = (*it).second.index;
        if (std::find(index.begin(), index.end(), idx) != index.end()) {
            return (*it).first;
        }
    }
    return "";
}

auto ui::dialog_configuration::defined_elements() const -> std::vector<std::string>
{
    std::vector<std::string> elements;

    for (auto it = m_elements.begin(); it != m_elements.end(); ++it) {
        elements.emplace_back((*it).first);
    }

    return elements;
}

// MARK: - Building

auto ui::dialog_configuration::build(const luabridge::LuaRef &configure_callback) -> ui::dialog::lua_reference
{
    m_dialog = ui::dialog::lua_reference(new ui::dialog(this));
    if (configure_callback.state() && configure_callback.isFunction()) {
        configure_callback(m_dialog);
    }
    return m_dialog;
}