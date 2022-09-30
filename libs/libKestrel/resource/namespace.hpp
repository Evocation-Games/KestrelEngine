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

#include <vector>
#include <string>
#include <functional>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libGraphite/rsrc/resource.hpp>
#include <libGraphite/rsrc/file.hpp>

namespace kestrel::resource
{
    lua_api(0.8) class resource_namespace
    {
    public:
        has_contructable_lua_api(resource_namespace);

        resource_namespace() = default;
        lua_api(0.8) explicit resource_namespace(const std::string& name);
        explicit resource_namespace(const std::vector<std::string>& names);

        lua_api(0.8) static auto global() -> lua_reference;
        lua_api(0.8) static auto universal() -> lua_reference;

        lua_api(0.8) [[nodiscard]] auto primary_name() const -> std::string;
        lua_api(0.8) [[nodiscard]] auto text() const -> std::string;
        lua_api(0.8) [[nodiscard]] auto count() const -> std::size_t;
        lua_api(0.8) [[nodiscard]] auto contains_resources() const -> bool;
        lua_api(0.8) [[nodiscard]] auto has_name(const std::string& name) const -> bool;
        lua_api(0.8) [[nodiscard]] auto is_global() const -> bool;
        lua_api(0.8) [[nodiscard]] auto is_universal() const -> bool;

        lua_api(0.8) auto lua_add_namespace(const lua_reference& ns) -> void;
        auto add_namespace(const resource_namespace& ns) -> void;

        lua_api(0.8) auto lua_each_name(luabridge::LuaRef body) const -> void;
        auto each_name(const std::function<auto(const resource_namespace&)->void>& body) const -> void;

        lua_api(0.8) [[nodiscard]] auto file_constraint(const graphite::rsrc::file *file) -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto identified_resource(graphite::rsrc::resource::identifier id) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto typed_resource(const std::string& type) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto named_resource(const std::string& name) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto typed_identified_resource(const std::string& type, graphite::rsrc::resource::identifier id) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto identified_named_resource(graphite::rsrc::resource::identifier id, const std::string& name) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto typed_named_resource(const std::string& type, const std::string& name) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto typed_identified_named_resource(const std::string& type, graphite::rsrc::resource::identifier id, const std::string& name) const -> descriptor::lua_reference;

        lua_api(0.8) [[nodiscard]] auto first_resource_of(const std::string& type) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto resource_for_id(const std::string& type, graphite::rsrc::resource::identifier id) const -> descriptor::lua_reference;
        lua_api(0.8) [[nodiscard]] auto resources_of_type(const std::string& type) const -> lua::vector<descriptor::lua_reference>;

//        lua_api(0.8) [[nodiscard]]  auto scene() const -> ui::game_scene::lua_reference;
//        lua_api(0.8) auto push_scene() const -> void;

    public:
        constexpr static const char *attribute_name = "namespace";

    private:
        constexpr static const char *universal_name = "*";
        constexpr static const char *global_name = "";
        std::vector<std::string> m_names;

    };
}