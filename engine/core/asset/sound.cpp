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
#include "core/asset/sound.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::sound::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<asset::sound>("Sound")
            .addConstructor<auto(*)(const asset::resource_reference::lua_reference&) -> void, asset::sound::lua_reference>()
            .addStaticFunction("load", &asset::sound::load)
            .addFunction("play", &asset::sound::play)
        .endClass();
}

// MARK: - Construction

asset::sound::sound(const asset::resource_reference::lua_reference& ref)
{
    if (ref->id().has_value()) {
        if (auto res = graphite::rsrc::manager::shared_manager().find(sound::type, ref->id().value()).lock()) {
            graphite::resources::sound sound(res->data(), res->id(), res->name());
            return;
        }
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::sound::load(const asset::resource_reference::lua_reference& ref) -> sound::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(sound::type, ref);
        if (asset.has_value()) {
            return std::any_cast<asset::sound::lua_reference>(asset.value());
        }
    }

    auto sound = asset::sound::lua_reference(new asset::sound(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(sound::type, ref, sound);
    }
    return sound;
}

auto asset::sound::play() -> void
{
    // do something sound-related
}