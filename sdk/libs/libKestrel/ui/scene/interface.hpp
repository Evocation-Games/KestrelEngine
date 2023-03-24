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

#include <vector>
#include <libGraphite/data/reader.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/resource/macro.hpp>

namespace kestrel::ui
{
    enum lua_api(UI.SceneInterfaceFlags, Available_0_8) scene_interface_flags : std::uint16_t
    {
        // ImGui Flags
        use_imgui = 0x0001,
        imgui_show_title = 0x0002,
        imgui_close_button = 0x0004,

        // Scene
        scene_passthrough = 0x0010,
        vertical_flow_layout = 0x0020,
        is_dialog = 0x0040,

        // Misc
        force_macintosh_dialog = 0xF00F,
    };

    struct lua_api(UI.SceneInterfaceItem, Available_0_8) scene_interface_item
    {
    public:
        has_constructable_lua_api(scene_interface_item);

        scene_interface_item() = default;

        lua_getter(type, Available_0_8) [[nodiscard]] auto type() const -> enum ui::control_type { return m_type; }
        auto set_type(enum ui::control_type type) -> void { m_type = type; }

        lua_getter(identifier, Available_0_8) [[nodiscard]] auto identifier() const -> std::string { return m_identifier; }
        auto set_identifier(const std::string& id) -> void { m_identifier = id; }

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect { return m_frame; }
        auto set_frame(const math::rect& frame) -> void { m_frame = frame; }

        lua_getter(childCount, Available_0_8) [[nodiscard]] auto child_count() const -> std::size_t { return m_items.size(); }
        lua_function(childAt, Available_0_8) [[nodiscard]] auto child_at(std::int32_t idx) const -> lua_reference { return m_items[idx]; }
        auto add_child(const lua_reference& item) -> void { m_items.emplace_back(item); }
        auto set_children(std::vector<lua_reference> children) -> void { m_items = std::move(children); }

        lua_getter(value, Available_0_8) [[nodiscard]] auto value() const -> std::string { return m_value; }
        auto set_value(const std::string& value) -> void { m_value = value; }

        lua_getter(script, Available_0_8) [[nodiscard]] auto lua_script() const -> std::string { return m_lua_script; }
        auto set_lua_script(const std::string& script) -> void { m_lua_script = script; }

    private:
        enum ui::control_type m_type { ui::control_type::none };
        std::string m_identifier;
        math::rect m_frame;
        std::vector<lua_reference> m_items;
        std::string m_value;
        std::string m_lua_script;
    };

    struct lua_api(UI.SceneInterface, Available_0_8) scene_interface
    {
    public:
        has_constructable_lua_api(scene_interface);
        is_resource_type("scïn");

        lua_constructor(Available_0_8) explicit scene_interface(const resource::descriptor::lua_reference& ref);

        lua_getter(itemCount, Available_0_8) [[nodiscard]] auto child_count() const -> std::uint32_t { return m_items.size(); }
        lua_function(itemAt, Available_0_8) [[nodiscard]] auto child_at(std::uint32_t idx) const -> scene_interface_item::lua_reference { return m_items[idx]; }

        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string { return m_name; }
        lua_getter(flags, Available_0_8) [[nodiscard]] auto flags() const -> enum scene_interface_flags { return m_flags; }
        lua_getter(size, Available_0_8) [[nodiscard]] auto scene_size() const -> math::size { return m_scene_size; }

        lua_getter(usesImGui, Available_0_8) [[nodiscard]] auto uses_imgui() const -> bool { return m_flags & scene_interface_flags::use_imgui; }
        lua_getter(showsTitle, Available_0_8) [[nodiscard]] auto shows_title_in_imgui() const -> bool { return m_flags & scene_interface_flags::imgui_show_title; }
        lua_getter(showsCloseButton, Available_0_8) [[nodiscard]] auto shows_close_button_in_imgui() const -> bool { return m_flags & scene_interface_flags::imgui_close_button; }
        lua_getter(hasPassthrough, Available_0_8) [[nodiscard]] auto has_scene_passthrough() const -> bool { return m_flags & scene_interface_flags::scene_passthrough; }

    private:
        std::string m_name;
        enum scene_interface_flags m_flags;
        math::size m_scene_size;
        std::vector<scene_interface_item::lua_reference> m_items;

        auto parse_item_list(graphite::data::reader& reader, std::uint32_t count = UINT32_MAX) -> std::vector<scene_interface_item::lua_reference>;
    };

}