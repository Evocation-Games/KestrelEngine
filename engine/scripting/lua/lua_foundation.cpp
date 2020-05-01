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

#include "scripting/lua/lua_foundation.hpp"

auto kestrel::lua::foundation::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<lua::foundation>("Kestrel")
            .addStaticFunction("importScript", &foundation::import_script)
            .addStaticFunction("start", &foundation::start)
        .endClass();
}

// MARK: - Functions

auto kestrel::lua::foundation::import_script(luabridge::RefCountedPtr<assets::resource_reference> ref) -> void
{
    if (ref->id().has_value()) {
        lua::script scpt(ref->id().value());
        scpt.execute();
    }
    else if (ref->name().has_value()) {
//        lua::script scpt(ref->name().value());
//        scpt.execute();
    }
    else {
        // Error
    }
}

auto kestrel::lua::foundation::start(luabridge::LuaRef callback) -> void
{
    // TODO: Delay this until we are certain all setup is completed, or until instructed to start.
    callback();
}
