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

#if !defined(KESTREL_LUA_SCENE_WRAPPER_HPP)
#define KESTREL_LUA_SCENE_WRAPPER_HPP

#include <memory>
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace graphics
{

    class scene;

    struct lua_scene_wrapper: public scripting::lua::object
    {
        typedef luabridge::RefCountedPtr<graphics::lua_scene_wrapper> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        std::shared_ptr<graphics::scene> scene;

        explicit lua_scene_wrapper(std::shared_ptr<graphics::scene> scene);

        lua_api static auto current() -> graphics::lua_scene_wrapper::lua_reference;

        lua_api auto present() const -> void;
        lua_api auto render(const luabridge::LuaRef& block) const -> void;
    };

}


#endif //KESTREL_LUA_SCENE_WRAPPER_HPP
