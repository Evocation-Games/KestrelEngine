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
#include "core/asset/rsrc/resource.hpp"
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
        std::optional<std::string> m_name {};

    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        resource_namespace() = default;
        explicit resource_namespace(const std::string& name);

        lua_api static auto global() -> resource_namespace::lua_reference;

        lua_api auto name() const -> std::string;
        lua_api auto contains_resources() const -> bool;
        lua_api auto has_name(const std::string& name) const -> bool;

        lua_api auto first_resource_of(const std::string& type) const -> asset::resource::lua_reference;
        lua_api auto resource_for_id(const std::string& type, int64_t id) const -> asset::resource::lua_reference;

        lua_api auto resources_of_type(const std::string& type) const -> std::vector<asset::resource::lua_reference>;
    };

}

#endif //KESTREL_NAMESPACE_HPP
