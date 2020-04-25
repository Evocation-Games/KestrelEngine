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

#if !defined(KESTREL_LUA_STATE_HPP)
#define KESTREL_LUA_STATE_HPP

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lauxlib.h"
}

#include <string>

namespace kestrel { namespace lua {

    /**
     * Singleton to encapsulate the persistant state of the Lua VM.
     * This class is responsible for managing access to the Lua VM state and performing various base
     * actions such as loading and injecting scripts.
     */
    class state
    {
    private:
        lua_State *m_state { nullptr };

        state();

    public:
        state(const state&) = delete;
        state& operator=(const state&) = delete;
        state(state&&) = delete;
        state& operator=(state&&) = delete;

        static auto& global()
        {
            static lua::state instance;
            return instance;
        }

        auto prepare() -> void;

        auto internal_state() const -> lua_State*;
        auto error_string() const -> std::string_view;

        auto load_script(std::string_view s) const -> void;
    };


}};

#endif //KESTREL_LUA_STATE_HPP
