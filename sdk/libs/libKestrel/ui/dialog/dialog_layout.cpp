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
#include <libKestrel/ui/dialog/dialog_layout.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Construction

kestrel::ui::dialog_layout::dialog_layout(const luabridge::LuaRef &ref, const math::size& size)
    : m_frame({}, size)
{
    if (lua::ref_isa<resource::descriptor>(ref)) {
        auto descriptor = ref.cast<resource::descriptor::lua_reference>();
        if (!descriptor->has_id()) {
            throw std::runtime_error("Invalid resource descriptor passed to dialog configuration: " + descriptor->description());
        }

        // If there is no type, then determine the best type in the following order: scïn, DITL
        if (!descriptor->has_type()) {
            // TODO: Update scene_interface method to the newer version
            if (graphite::rsrc::manager::shared_manager().find(scene_interface::resource_type::code, descriptor->id)) {
                descriptor = descriptor->with_type(scene_interface::resource_type::code);
            }
            else if (graphite::rsrc::manager::shared_manager().find<graphite::toolbox::dialog>(descriptor->id)) {
                descriptor = descriptor->with_type(graphite::toolbox::dialog::type_code());
            }
            else {
                throw std::runtime_error("No matching resource for resource descriptor passed to dialog configuration: " + descriptor->description());
            }
        }

        // Determine the type of the descriptor and then load the appropriate asset
        if (descriptor->type == scene_interface::resource_type::code) {
            scene_interface interface(descriptor);
            build_scene_interface_layout(&interface);
        }
        else if (descriptor->type == graphite::toolbox::dialog::type_code()) {
            m_mode = dialog_render_mode::scene;
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

auto kestrel::ui::dialog_layout::build_scene_interface_layout(const scene_interface *scin) -> void
{
    m_mode = scin->flags() & scene_interface_flags::use_imgui ? dialog_render_mode::imgui : dialog_render_mode::scene;

    m_flags = scin->flags();
    m_frame.set_size(scin->scene_size());

    for (auto i = 0; i < scin->child_count(); ++i) {
        const auto& item = scin->child_at(i);

        struct element element;
        element.type = static_cast<std::uint8_t>(item->type());
        element.frame = item->frame();
        element.value = item->value();

        // TODO: Handle being able to load linked scripts...
        element.script = lua::script(kestrel::lua_runtime(), item->lua_script());

        m_elements.emplace_back(std::move(element));
    }
}

auto kestrel::ui::dialog_layout::build_dialog_layout(const graphite::toolbox::dialog *dlog) -> void
{
    if (!graphite::rsrc::manager::shared_manager().find<graphite::toolbox::dialog_item_list>(dlog->interface_list())) {
        throw std::runtime_error("Failed to find DITL resource referenced by DLOG.");
    }

    m_frame = math::rect(dlog->bounds());

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

auto kestrel::ui::dialog_layout::frame() const -> math::rect
{
    return m_frame;
}

auto kestrel::ui::dialog_layout::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
}

auto kestrel::ui::dialog_layout::size() const -> math::size
{
    return m_frame.size();
}

auto kestrel::ui::dialog_layout::set_size(const math::size &size) -> void
{
    m_frame.set_size(size);
}

auto kestrel::ui::dialog_layout::mode() const -> enum dialog_render_mode
{
    return m_mode;
}

auto kestrel::ui::dialog_layout::flags() const -> enum scene_interface_flags
{
    return m_flags;
}

auto kestrel::ui::dialog_layout::element_count() const -> std::size_t
{
    return m_elements.size();
}

auto kestrel::ui::dialog_layout::element_at(std::int16_t idx) const -> const struct element *
{
    return &m_elements[idx - 1];
}

