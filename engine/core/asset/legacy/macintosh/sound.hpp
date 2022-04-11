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

#pragma once

#include "core/audio/player/player_item.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "scripting/state.hpp"

namespace asset::legacy::macintosh::quicktime
{

    struct sound: public scripting::lua::object
    {
    public:
        constexpr static const char *type { "snd " };
        typedef luabridge::RefCountedPtr<sound> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit sound(const resource_descriptor::lua_reference& ref);

        lua_api static auto load(const resource_descriptor::lua_reference& ref) -> lua_reference;
        lua_api auto play() -> void;
        lua_api auto playWithCallback(const luabridge::LuaRef& ref) -> void;
        lua_api auto stop() -> void;

    private:
        std::shared_ptr<audio::player_item> m_item {};
        uint64_t m_item_reference { 0 };
        auto parse(const std::shared_ptr<graphite::data::data>& data) -> bool;
    };

};
