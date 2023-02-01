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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/writer.hpp>
#include <libGraphite/rsrc/file.hpp>

namespace kestrel::sandbox
{
    enum class lua_api(Kestrel.ResourceFileFormat, Available_0_8) resource_file_type : std::uint8_t
    {
        none lua_case(None, Available_0_8) = 0,
        classic lua_case(Classic, Available_0_8) = 1,
        extended lua_case(Extended, Available_0_8) = 2,
        rez lua_case(Rez, Available_0_8) = 3
    };

    struct lua_api(Kestrel.Sandbox.ResourceFileReference, Available_0_8) resource_file_reference
    {
    public:
        has_constructable_lua_api(resource_file_reference);

        explicit resource_file_reference(const std::string& path);

        lua_getter(exists, Available_0_8) [[nodiscard]] auto exists() const -> bool;
        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_getter(path, Available_0_8) [[nodiscard]] auto path() const -> std::string;
        lua_getter(extension, Available_0_8) [[nodiscard]] auto extension() const -> std::string;
        lua_getter(basename, Available_0_8) [[nodiscard]] auto basename() const -> std::string;
        lua_getter(type, Available_0_8) [[nodiscard]] auto type() const -> enum resource_file_type;

        lua_getter(resourceTypes, Available_0_8) [[nodiscard]] auto all_types() const -> lua::vector<resource::descriptor::lua_reference>;

        lua_function(save, Available_0_8) auto save() -> void;

        lua_function(writeResource, Available_0_8) auto add_resource(const resource::writer::lua_reference& writer) -> void;

    private:
        std::string m_path;
        graphite::rsrc::file m_file;
    };
}
