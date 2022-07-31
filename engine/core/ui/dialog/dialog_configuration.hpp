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
#include <unordered_map>
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"
#include "core/ui/control_definition.hpp"
#include "core/asset/static_image.hpp"
#include "core/ui/game_scene.hpp"
#include "core/ui/dialog/dialog.hpp"
#include "core/ui/dialog/dialog_layout.hpp"

namespace ui
{
    class dialog_configuration : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<dialog_configuration> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit dialog_configuration(const luabridge::LuaRef& layout);

        lua_api auto build(const luabridge::LuaRef& configure_callback) -> ui::dialog::lua_reference;

        [[nodiscard]] lua_api inline auto size() const -> math::size { return m_layout.size(); }
        lua_api inline auto set_size(const math::size& size) -> void { m_layout.set_size(size); }

        lua_api auto set_background(const luabridge::LuaRef& background) -> void;
        lua_api auto set_stretched_background(const luabridge::LuaRef& top, const luabridge::LuaRef& fill, const luabridge::LuaRef& bottom) -> void;

        lua_api auto set_passthrough(bool f) -> void { m_passthrough = f; }
        [[nodiscard]] lua_api auto passthrough() const -> bool { return m_passthrough; }

        lua_api auto define_element(const luabridge::LuaRef& index, const std::string& name, uint8_t type) -> control_definition::lua_reference;
        lua_api auto element(const std::string& name) -> control_definition::lua_reference;
        [[nodiscard]] auto all_elements() const -> std::vector<std::string>;

        [[nodiscard]] inline auto background() const -> asset::static_image::lua_reference { return m_background_image; }
        [[nodiscard]] inline auto background_stretch() const -> asset::static_image::lua_reference { return m_background_stretch_image; }
        [[nodiscard]] inline auto background_bottom() const -> asset::static_image::lua_reference { return m_background_bottom_image; }

        [[nodiscard]] inline auto layout() const -> const dialog_layout * { return &m_layout; }

    private:
        std::unordered_map<std::string, control_definition::lua_reference> m_element_definitions;
        dialog_layout m_layout;
        ui::dialog::lua_reference m_dialog { nullptr };
        bool m_passthrough { true };

        asset::static_image::lua_reference m_background_image { nullptr };
        asset::static_image::lua_reference m_background_stretch_image { nullptr };
        asset::static_image::lua_reference m_background_bottom_image { nullptr };

        static auto load_image_asset(const luabridge::LuaRef& ref) -> asset::static_image::lua_reference;
    };
}
