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

namespace graphics
{

    class scene: public std::enable_shared_from_this<scene>
    {
    protected:
        std::weak_ptr<graphics::session_window> m_owner;
        std::vector<luabridge::LuaRef> m_render_blocks;
        std::vector<luabridge::LuaRef> m_key_event_blocks;
        scripting::lua::script m_script;

    public:
        explicit scene(const std::shared_ptr<graphics::session_window>& window, const scripting::lua::script &script);

        auto add_render_block(const luabridge::LuaRef& block) -> void;
        auto invoke_render_blocks() -> void;

        auto add_key_event_block(const luabridge::LuaRef& block) -> void;
        auto key_event(const event::key& event) -> void;

        virtual auto start() -> void;
        virtual auto update() -> void;
        virtual auto render() -> void;

        virtual auto draw_entity(const graphics::entity::lua_reference& entity) const -> void;
    };

}


#endif //KESTREL_SCENE_HPP
