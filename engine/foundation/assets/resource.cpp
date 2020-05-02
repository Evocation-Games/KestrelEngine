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

#include "foundation/assets/resource.hpp"
#include "libGraphite/rsrc/manager.hpp"

// MARK: - Lua Integration

auto kestrel::assets::resource::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<assets::resource>("ResourceData")
            .addConstructor<auto(*)(resource_reference::lua_reference)->void, resource::lua_reference>()
            .addFunction("isValid", &resource::valid)
            .addProperty("id", &resource::id)
            .addProperty("name", &resource::name)
        .endClass();
}

// MARK: - Construction

kestrel::assets::resource::resource(kestrel::assets::resource_reference::lua_reference ref)
{
    if (ref->id().has_value() && ref->type().has_value()) {
        if (auto res = graphite::rsrc::manager::shared_manager().find(ref->type().value(), ref->id().value()).lock()) {
            m_type = res->type_code();
            m_id = res->id();
            m_name = res->name();
            m_resource_reader = graphite::data::reader(res->data());
        }
    }
}

auto kestrel::assets::resource::valid() const -> bool
{
    return m_resource_reader.has_value();
}

auto kestrel::assets::resource::id() const -> int64_t
{
    return m_id;
}

auto kestrel::assets::resource::name() const -> std::string
{
    return m_name;
}


