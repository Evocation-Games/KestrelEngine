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

#include "foundation/math/size.hpp"
#include "scripting/lua/lua.hpp"
#include "foundation/clock/timed_event.hpp"
#include "foundation/assets/resource_reference.hpp"

namespace kestrel
{

    class scene : public lua::object
    {
    private:
        std::string m_name{""};
        lua::script m_script{};
        luabridge::LuaRef m_renderer { lua::stub_function() };
        std::vector<timed_event> m_timed_callbacks;

    public:
        typedef luabridge::RefCountedPtr<kestrel::scene> lua_reference;

        static auto register_object() -> void;
        static auto create(std::string name) -> scene::lua_reference;
        static auto current() -> scene::lua_reference;

        scene(std::string name);

        auto set_name(std::string name) -> void;
        auto get_name() const -> std::string;

        auto size() const -> math::size;

        auto attach_script(assets::resource_reference::lua_reference ref) -> void;
        auto set_renderer(luabridge::LuaRef callback) -> void;

        auto add_timed_callback(double delay, luabridge::LuaRef callback) -> void;

        auto present() -> void;
        auto begin() -> void;
        auto render() -> void;

        auto set_game_coordinates() const -> void;
        auto set_interface_coordinates() const -> void;

    };

};

#endif //KESTREL_SCENE_HPP
