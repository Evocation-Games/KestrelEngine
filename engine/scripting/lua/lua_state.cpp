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

#include <stdexcept>
#include "scripting/lua/lua_state.hpp"
#include "scripting/lua/lua_stack.hpp"
#include "foundation/scene/scene.hpp"
#include "foundation/assets/macintosh_picture.hpp"
#include "scripting/lua/lua_foundation.hpp"
#include "foundation/assets/resource_reference.hpp"
#include "foundation/assets/resource.hpp"
#include "foundation/math/size.hpp"
#include "foundation/math/point.hpp"
#include "foundation/math/vector.hpp"
#include "foundation/gl/text.hpp"

// MARK: - Construction

kestrel::lua::state::state()
{
    m_state = luaL_newstate();
    luaL_openlibs(m_state);
}


// MARK: - Lua Configuration

auto kestrel::lua::state::prepare() -> void
{
    scene::register_object();
    foundation::register_object();
    assets::macintosh_picture::register_object();
    assets::resource_reference::register_object();
    assets::resource::register_object();
    math::size::register_object();
    math::point::register_object();
    math::vector::register_object();
    gl::text::register_object();
}


// MARK: - Accessor

auto kestrel::lua::state::internal_state() const -> lua_State *
{
    return m_state;
}

auto kestrel::lua::state::error_string() const -> std::string_view
{
    return lua::stack::pop_string();
}


// MARK: - Scripts

auto kestrel::lua::state::load_script(std::string_view s) const -> void
{
    if (luaL_loadstring(m_state, std::string(s).c_str()) != LUA_OK) {
        throw std::runtime_error(std::string(error_string()));
    }
    if (lua_pcall(m_state, 0, LUA_MULTRET, 0) != LUA_OK) {
        throw std::runtime_error(std::string(error_string()));
    }
}