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

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "util/lua_vector.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include <libGraphite/rsrc/file.hpp>

namespace host::sandbox
{
    struct resource_file_reference: public scripting::lua::object
    {
    public:
        enum class type : std::uint8_t { none = 0, classic = 1, extended = 2, rez = 3 };

        typedef luabridge::RefCountedPtr<resource_file_reference> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit resource_file_reference(const std::string& path);

        [[nodiscard]] lua_api auto exists() const -> bool;
        [[nodiscard]] lua_api auto name() const -> std::string;
        [[nodiscard]] lua_api auto path() const -> std::string;
        [[nodiscard]] lua_api auto extension() const -> std::string;
        [[nodiscard]] lua_api auto basename() const -> std::string;
        [[nodiscard]] lua_api auto type() const -> enum type;

        [[nodiscard]] lua_api auto all_types() const -> util::lua_vector<asset::resource_descriptor::lua_reference>;

    private:
        std::string m_path;
        graphite::rsrc::file m_file;
    };
}
