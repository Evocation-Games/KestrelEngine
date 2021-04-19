// Copyright (c) 2020 Tom Hancocks
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

#if !defined(KESTREL_SCENE_HPP)
#define KESTREL_SCENE_HPP

#include <memory>
#include "core/graphics/common/session_window.hpp"
#include "scripting/state.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/clock/timed_event.hpp"

namespace graphics
{

    class scene: public std::enable_shared_from_this<scene>
    {
    protected:
        std::string m_name;
        std::weak_ptr<graphics::session_window> m_owner;
        std::vector<luabridge::LuaRef> m_render_blocks;
        std::vector<luabridge::LuaRef> m_update_blocks;
        std::vector<luabridge::LuaRef> m_key_event_blocks;
        std::vector<luabridge::LuaRef> m_mouse_event_blocks;
        std::vector<std::shared_ptr<rtc::timed_event>> m_timed_events;
        scripting::lua::script m_script;
        rtc::clock::time m_starting_time { rtc::clock::global().current() };
        bool m_passthrough_render { false };

    public:
        explicit scene(const std::shared_ptr<graphics::session_window>& window, const scripting::lua::script &script, const std::string& name);

        auto get_name() const -> std::string;
        auto get_owner() const -> std::weak_ptr<graphics::session_window>;

        auto get_passthrough_render() const -> bool;
        auto set_passthrough_render(const bool f) -> void;

        auto add_update_block(const luabridge::LuaRef& block) -> void;
        auto invoke_update_blocks() -> void;

        auto add_render_block(const luabridge::LuaRef& block) -> void;
        auto invoke_render_blocks() -> void;

        auto add_key_event_block(const luabridge::LuaRef& block) -> void;
        auto key_event(const event::key& event) -> void;

        auto add_mouse_event_block(const luabridge::LuaRef& block) -> void;
        auto mouse_event(const event::mouse& event) -> void;

        auto add_timed_event(std::shared_ptr<rtc::timed_event> event) -> void;
        auto check_timed_events() -> void;

        auto current_time() const -> double;

        virtual auto start() -> void;
        virtual auto update() -> void;
        virtual auto render() -> void;

        virtual auto draw_entity(const graphics::entity::lua_reference& entity) const -> void;
    };

}


#endif //KESTREL_SCENE_HPP
