// Copyright (c) 2022 Tom Hancocks
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

#include <unordered_map>
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/asset/rsrc/resource_key.hpp"

namespace asset
{
    struct resource_collection: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<resource_collection> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        resource_collection() = default;

        lua_api auto add_resource(const resource_descriptor::lua_reference& descriptor, const luabridge::LuaRef& resource) -> void;

        [[nodiscard]] lua_api auto size() const -> std::size_t;
        [[nodiscard]] lua_api auto has(const resource_descriptor::lua_reference& descriptor) const -> bool;
        [[nodiscard]] lua_api auto get(const resource_descriptor::lua_reference& descriptor) const -> luabridge::LuaRef;

    private:
        std::vector<std::pair<resource_key, luabridge::LuaRef>> m_resources;
    };
}
