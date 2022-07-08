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
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/ui/control_definition.hpp"
#include "scripting/state.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "util/hint.hpp"

namespace asset
{
    struct scene_interface : public scripting::lua::object
    {
    public:
        struct item;

        enum flags : uint16_t
        {
            // ImGui Flags
            use_imgui = 0x0001,
            imgui_show_title = 0x0002,
            imgui_close_button = 0x0004,

            // Scene
            scene_passthrough = 0x0010,
        };

        struct item
        {
        public:
            item() = default;

            [[nodiscard]] lua_api auto type() const -> enum ui::control_definition::type { return m_type; }
            lua_api auto set_type(enum ui::control_definition::type type) -> void { m_type = type; }

            [[nodiscard]] lua_api auto identifier() const -> std::string { return m_identifier; }
            lua_api auto set_identifier(const std::string& id) -> void { m_identifier = id; }

            [[nodiscard]] lua_api auto frame() const -> math::rect { return m_frame; }
            lua_api auto set_frame(const math::rect& frame) -> void { m_frame = frame; }

            [[nodiscard]] lua_api auto child_count() const -> uint32_t { return m_items.size(); }
            [[nodiscard]] lua_api auto child_at(uint32_t idx) const -> const struct item& { return m_items[idx]; }
            lua_api auto add_child(struct item& item) -> void { m_items.emplace_back(item); }
            lua_api auto set_children(std::vector<struct item> children) -> void { m_items = std::move(children); }

            [[nodiscard]] lua_api auto value() const -> std::string { return m_value; }
            lua_api auto set_value(const std::string& value) -> void { m_value = value; }

            [[nodiscard]] lua_api auto lua_script() const -> std::string { return m_lua_script; }
            lua_api auto set_lua_script(const std::string& script) -> void { m_lua_script = script; }

        private:
            enum ui::control_definition::type m_type { ui::control_definition::type::none };
            std::string m_identifier;
            math::rect m_frame;
            std::vector<struct item> m_items;
            std::string m_value;
            std::string m_lua_script;
        };

    public:
        constexpr static const char *type { "scïn" };
        typedef luabridge::RefCountedPtr<scene_interface> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        lua_api explicit scene_interface(const asset::resource_descriptor::lua_reference& ref);

        [[nodiscard]] lua_api auto child_count() const -> uint32_t { return m_items.size(); }
        [[nodiscard]] lua_api auto child_at(uint32_t idx) const -> const struct item& { return m_items[idx]; }

        [[nodiscard]] lua_api auto name() const -> std::string { return m_name; }
        [[nodiscard]] lua_api auto flags() const -> enum flags { return m_flags; }
        [[nodiscard]] lua_api auto scene_size() const -> math::size { return m_scene_size; }

        [[nodiscard]] lua_api auto uses_imgui() const -> bool { return m_flags & flags::use_imgui; }
        [[nodiscard]] lua_api auto shows_title_in_imgui() const -> bool { return m_flags & flags::imgui_show_title; }
        [[nodiscard]] lua_api auto shows_close_button_in_imgui() const -> bool { return m_flags & flags::imgui_close_button; }
        [[nodiscard]] lua_api auto has_scene_passthrough() const -> bool { return m_flags & flags::scene_passthrough; }

    private:
        std::string m_name;
        enum flags m_flags;
        math::size m_scene_size;
        std::vector<struct item> m_items;

        auto parse_item_list(graphite::data::reader& reader, uint32_t count = UINT32_MAX) -> std::vector<struct item>;
    };

    inline auto operator& (enum scene_interface::flags lhs, enum scene_interface::flags rhs) -> enum scene_interface::flags
    {
        return static_cast<enum scene_interface::flags>(static_cast<int>(lhs) & static_cast<int>(rhs));
    };
}