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

#include <string>
#include <vector>
#include <functional>
#include <clang-c/Index.h>

namespace kdtool::lua_api::ast
{
    struct namespace_path
    {
        static constexpr const char *cxx_delimiter = "::";
        static constexpr const char *lua_delimiter = ".";

        namespace_path() = default;
        namespace_path(const namespace_path& path);
        namespace_path(namespace_path&& path) noexcept;

        auto operator=(const namespace_path& path) -> namespace_path&;
        auto operator=(namespace_path&& path) noexcept -> namespace_path&;

        explicit namespace_path(const std::string& ns_name);
        explicit namespace_path(const std::vector<std::string>& ns_names);
        explicit namespace_path(CXCursor ns_name);

        [[nodiscard]] auto nested(const std::string& ns_name) const -> namespace_path;
        [[nodiscard]] auto nested(CXCursor ns_name) const -> namespace_path;

        [[nodiscard]] auto parent() const -> namespace_path;

        auto push(const std::string& ns_name) -> void;
        auto push(CXCursor ns_name) -> void;
        auto pop() -> void;

        [[nodiscard]] auto has_namespace(const std::string& ns_name) const -> bool;
        [[nodiscard]] auto is_namespace(const std::string& ns_name) const -> bool;
        [[nodiscard]] auto is_namespace(CXCursor ns_name) const -> bool;

        [[nodiscard]] auto path_string(const char *delimiter = cxx_delimiter) const -> std::string;
        [[nodiscard]] auto path_string(const std::string& name, const char *delimiter = cxx_delimiter) const -> std::string;

        [[nodiscard]] auto path_vector() const -> const std::vector<std::string>&;

    private:
        std::vector<std::string> m_ns_names;
    };
}