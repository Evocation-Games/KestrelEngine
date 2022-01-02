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

#if !defined(KESTREL_MOD_REFERENCE_HPP)
#define KESTREL_MOD_REFERENCE_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace host::sandbox
{
    struct mod_reference: public scripting::lua::object
    {
    public:
        enum class bundle_type { simple, package };
        enum class bundle_origin { game, user };

        typedef luabridge::RefCountedPtr<host::sandbox::mod_reference> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        std::string m_name;
        std::string m_version;
        std::string m_author;
        std::string m_primary_namespace;
        int64_t m_lua_entry_script;
        std::string m_path;
        bundle_type m_bundle;
        bundle_origin m_origin;
        bool m_loaded { false };
        bool m_executed { false };
        bool m_is_active { true };

    public:
        explicit mod_reference(const std::string& path, bundle_origin origin = bundle_origin::game, bundle_type type = bundle_type::simple);

        [[nodiscard]] auto validate_as_modpackage() const -> bool;
        auto load_modpackage() -> void;

        [[nodiscard]] auto validate_as_simplemod() const -> bool;
        auto load_simplemod() -> void;
        auto construct_as_simplemod() -> void;

        auto configure_lua_api(const lua_reference& self = nullptr) -> void;

        [[nodiscard]] auto has_initial_script() const -> bool;
        [[nodiscard]] auto user_provided() const -> bool;
        [[nodiscard]] auto has_executed() const -> bool;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto author() const -> std::string;
        [[nodiscard]] auto version() const -> std::string;
        [[nodiscard]] auto path() const -> std::string;

    };
}

#endif //ENGINE_CORE_FILE_MOD_REFERENCE_HPP
