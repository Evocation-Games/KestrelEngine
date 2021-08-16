// Copyright (c) 2021 Tom Hancocks
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

#if !defined(KESTREL_NAMESPACE_HPP)
#define KESTREL_NAMESPACE_HPP

#include <string>
#include <optional>
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "util/lua_vector.hpp"

namespace asset
{

    class resource_namespace: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<asset::resource_namespace> lua_reference;

    private:
        constexpr static const char *universal_name { "*" };
        constexpr static const char *global_name { "" };
        std::vector<std::string> m_names;

    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        resource_namespace() = default;
        explicit resource_namespace(const std::string& name);
        explicit resource_namespace(const std::vector<std::string>& names);

        lua_api static auto global() -> resource_namespace::lua_reference;
        lua_api static auto universal() -> resource_namespace::lua_reference;

        lua_api auto primary_name() const -> std::string;
        lua_api auto text() const -> std::string;
        lua_api auto count() const -> std::size_t;
        lua_api auto contains_resources() const -> bool;
        lua_api auto has_name(const std::string& name) const -> bool;
        lua_api auto is_global() const -> bool;
        lua_api auto is_universal() const -> bool;

        lua_api auto add_namespace_lua(const asset::resource_namespace::lua_reference& ns) -> void;
        auto add_namespace(const asset::resource_namespace& ns) -> void;

        lua_api auto lua_each_name(luabridge::LuaRef body) const -> void;
        auto each_name(const std::function<auto(const asset::resource_namespace&)->void>& body) const -> void;

        lua_api auto identified_resource(int64_t id) const -> resource_descriptor::lua_reference;
        lua_api auto typed_resource(const std::string& type) const -> resource_descriptor::lua_reference;
        lua_api auto named_resource(const std::string& name) const -> resource_descriptor::lua_reference;
        lua_api auto typed_identified_resource(const std::string& type, int64_t id) const -> resource_descriptor::lua_reference;
        lua_api auto identified_named_resource(int64_t id, const std::string& name) const -> resource_descriptor::lua_reference;
        lua_api auto typed_named_resource(const std::string& type, const std::string& name) const -> resource_descriptor::lua_reference;
        lua_api auto typed_identified_named_resource(const std::string& type, int64_t id, const std::string& name) const -> resource_descriptor::lua_reference;

        lua_api auto first_resource_of(const std::string& type) const -> asset::resource_descriptor::lua_reference;
        lua_api auto resource_for_id(const std::string& type, int64_t id) const -> asset::resource_descriptor::lua_reference;
        lua_api auto resources_of_type(const std::string& type) const -> util::lua_vector<asset::resource_descriptor::lua_reference>;
    };

}

#endif //KESTREL_NAMESPACE_HPP
