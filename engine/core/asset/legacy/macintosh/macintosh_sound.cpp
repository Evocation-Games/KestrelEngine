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
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/resources/sound.hpp>
#include "macintosh_sound.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::macintosh_sound::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<asset::macintosh_sound>("Sound")
                    .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&) -> void, asset::macintosh_sound::lua_reference>()
                    .addStaticFunction("load", &asset::macintosh_sound::load)
                    .addFunction("play", &asset::macintosh_sound::play)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::macintosh_sound::macintosh_sound(const asset::resource_descriptor::lua_reference& ref)
{
    if (auto res = ref->with_type(type)->load().lock()) {
        graphite::resources::sound sound(res->data(), res->id(), res->name());
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::macintosh_sound::load(const asset::resource_descriptor::lua_reference& ref) -> macintosh_sound::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(macintosh_sound::type, ref);
        if (asset.has_value()) {
            return std::any_cast<asset::macintosh_sound::lua_reference>(asset.value());
        }
    }

    auto sound = asset::macintosh_sound::lua_reference(new asset::macintosh_sound(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(macintosh_sound::type, ref, sound);
    }
    return sound;
}

auto asset::macintosh_sound::play() -> void
{
    // do something sound-related
}

