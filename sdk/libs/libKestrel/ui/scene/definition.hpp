// Copyright (c) 2023 Tom Hancocks
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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/macro.hpp>
#include <libKestrel/ui/scene/interface.hpp>
#include <libKestrel/ui/legacy/macintosh/dialog.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>

namespace kestrel::ui
{
    struct lua_api(UI.SceneDefinition, Available_0_9) scene_definition
    {
    public:
        has_constructable_lua_api(scene_definition);
        is_resource_type("scën");

        lua_constructor(Available_0_9) explicit scene_definition(const std::string& name);

        lua_getter(name, Available_0_9) [[nodiscard]] auto name() const -> std::string;
        lua_getter(valid, Available_0_9) [[nodiscard]] auto valid() const -> bool;

        lua_getter(shouldUseMacintoshDialog, Available_0_9) [[nodiscard]] auto should_use_macintosh_dialog() const -> bool;
        lua_getter(interface, Available_0_9) [[nodiscard]] auto scene_interface() const -> scene_interface::lua_reference;
        lua_getter(macintoshDialog, Available_0_9) [[nodiscard]] auto macintosh_dialog() const -> ui::legacy::macintosh::toolbox::dialog::lua_reference;

        [[nodiscard]] auto main_script() const -> lua::script;

        lua_function(constructScene, Available_0_9) auto construct_scene() -> ui::game_scene::lua_reference;
        lua_function(push, Available_0_9) auto push() -> void;

    private:
        struct constants {
            static constexpr std::int64_t definition_id = 0;
        };

        struct ditl_element_mapping {
            std::uint16_t index;
            std::string name;
            std::uint8_t type;
        };

        std::string m_name;
        std::vector<ditl_element_mapping> m_ditl_map;
        resource::descriptor::lua_reference m_descriptor { nullptr };
        resource::descriptor::lua_reference m_main_script { nullptr };
        resource::descriptor::lua_reference m_interface { nullptr };
        resource::descriptor::lua_reference m_dialog { nullptr };
        resource::descriptor::lua_reference m_background { nullptr };
        resource::descriptor::lua_reference m_background_top { nullptr };
        resource::descriptor::lua_reference m_background_bottom { nullptr };
        bool m_valid { false };

        auto load() -> void;
        auto construct_interface() -> ui::game_scene::lua_reference;
        auto construct_macintosh_dialog() -> ui::game_scene::lua_reference;
    };
}