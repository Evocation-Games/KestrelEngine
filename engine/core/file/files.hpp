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

#if !defined(KESTREL_FILESYSTEM_HPP)
#define KESTREL_FILESYSTEM_HPP

#include <vector>
#include <string>
#include "core/file/file_reference.hpp"
#include "core/file/directory_reference.hpp"
#include "core/file/mod_reference.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace host::sandbox
{

    class files: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<host::sandbox::files> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        host::sandbox::file_reference::lua_reference m_current_save_file { nullptr };
        host::sandbox::file_reference::lua_reference m_game_core { nullptr };
        host::sandbox::directory_reference::lua_reference m_game_data { nullptr };
        host::sandbox::directory_reference::lua_reference m_game_fonts { nullptr };
        host::sandbox::directory_reference::lua_reference m_game_mods { nullptr };
        std::vector<host::sandbox::mod_reference::lua_reference> m_mods;

        files() = default;

        auto load_mods(const util::lua_vector<file_reference::lua_reference>& mods, mod_reference::bundle_origin) -> void;

    public:
        files(const files&) = delete;
        files& operator=(const files &) = delete;
        files(files &&) = delete;
        files & operator=(files &&) = delete;

        static auto shared_files() -> files&;

        auto set_game_core_path(const std::string& path) -> void;
        auto set_game_data_path(const std::string& path) -> void;
        auto set_game_fonts_path(const std::string& path) -> void;
        auto set_game_mods_path(const std::string& path) -> void;

        auto discover_mods() -> void;

        [[nodiscard]] auto get_current_save_file() const -> host::sandbox::file_reference::lua_reference;
        auto set_save_file_name(const std::string& name) -> void;

        lua_api static auto game() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto user() -> host::sandbox::directory_reference::lua_reference;

        lua_api static auto game_core() -> host::sandbox::file_reference::lua_reference;
        lua_api static auto game_data() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto game_fonts() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto game_mods() -> host::sandbox::directory_reference::lua_reference;

        lua_api static auto user_mods() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto user_saves() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto current_save_file() -> host::sandbox::file_reference::lua_reference;

        lua_api static auto set_save_file(const std::string& name) -> void;
        lua_api static auto save() -> void;
    };

}

#endif //KESTREL_FILESYSTEM_HPP
