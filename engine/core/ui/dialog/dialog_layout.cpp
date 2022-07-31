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
#include "core/environment.hpp"
#include "core/ui/dialog/dialog_layout.hpp"
#include "scripting/state.hpp"

// MARK: - Construction

ui::dialog_layout::dialog_layout(const luabridge::LuaRef &ref, const math::size& size)
    : m_frame({}, size)
{
    if (scripting::lua::ref_isa<asset::resource_descriptor>(ref)) {
        auto descriptor = ref.cast<asset::resource_descriptor::lua_reference>();
        if (!descriptor->has_id()) {
            throw std::runtime_error("Invalid resource descriptor passed to dialog configuration: " + descriptor->description());
        }

        // If there is no type, then determine the best type in the following order: scïn, DITL
        if (!descriptor->has_type()) {
            // TODO: Update scene_interface method to the newer version
            if (graphite::rsrc::manager::shared_manager().find(asset::scene_interface::type, descriptor->id)) {
                descriptor = descriptor->with_type(asset::scene_interface::type);
            }
            else if (graphite::rsrc::manager::shared_manager().find<graphite::toolbox::dialog>(descriptor->id)) {
                descriptor = descriptor->with_type(graphite::toolbox::dialog::type_code());
            }
            else {
                throw std::runtime_error("No matching resuorce for resource descriptor passed to dialog configuration: " + descriptor->description());
            }
        }

        // Determine the type of the descriptor and then load the appropriate asset
        if (descriptor->type == asset::scene_interface::type) {
            asset::scene_interface interface(descriptor);
            build_scene_interface_layout(&interface);
        }
        else if (descriptor->type == graphite::toolbox::dialog::type_code()) {
            m_mode = mode::scene;

            auto dlog = graphite::rsrc::manager::shared_manager().load<graphite::toolbox::dialog>(descriptor->id);
            build_dialog_layout(&dlog);
        }
        else {
            throw std::runtime_error("Unknown resource descriptor type passed to dialog configuration: " + descriptor->description());
        }
    }
    else {
        throw std::runtime_error("Unknown layout reference provided.");
    }
}

// MARK: - Building

auto ui::dialog_layout::build_scene_interface_layout(const asset::scene_interface *scin) -> void
{
    auto env = environment::active_environment().lock();

    m_mode = static_cast<std::uint16_t>(scin->flags()) & static_cast<std::uint16_t>(asset::scene_interface::flags::use_imgui)
        ? mode::imgui : mode::scene;

    m_flags = scin->flags();
    m_frame.size = scin->scene_size();

    for (auto i = 0; i < scin->child_count(); ++i) {
        const auto& item = scin->child_at(i);

        struct element element;
        element.type = static_cast<std::uint8_t>(item.type());
        element.frame = item.frame();
        element.value = item.value();

        // TODO: Handle being able to load linked scripts...
        if (env) {
            element.script = scripting::lua::script(env->lua_runtime(), item.lua_script());
        }

        m_elements.emplace_back(std::move(element));
    }
}

auto ui::dialog_layout::build_dialog_layout(const graphite::toolbox::dialog *dlog) -> void
{
    if (!graphite::rsrc::manager::shared_manager().find<graphite::toolbox::dialog_item_list>(dlog->interface_list())) {
        throw std::runtime_error("Failed to find DITL resource referenced by DLOG.");
    }

    m_frame = dlog->bounds();

    auto ditl = graphite::rsrc::manager::shared_manager().load<graphite::toolbox::dialog_item_list>(dlog->interface_list());
    for (const auto& item : ditl) {
        struct element element;
        element.type = static_cast<std::uint8_t>(item.type);
        element.frame = math::rect(item.frame);
        element.value = item.info;
        m_elements.emplace_back(std::move(element));
    }
}

// MARK: - Accessors

auto ui::dialog_layout::frame() const -> math::rect
{
    return m_frame;
}

auto ui::dialog_layout::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
}

auto ui::dialog_layout::size() const -> math::size
{
    return m_frame.size;
}

auto ui::dialog_layout::set_size(const math::size &size) -> void
{
    m_frame.size = size;
}

auto ui::dialog_layout::mode() const -> enum mode
{
    return m_mode;
}

auto ui::dialog_layout::flags() const -> enum asset::scene_interface::flags
{
    return m_flags;
}

auto ui::dialog_layout::element_count() const -> std::size_t
{
    return m_elements.size();
}

auto ui::dialog_layout::element_at(std::int16_t idx) const -> const struct element *
{
    return &m_elements[idx - 1];
}

