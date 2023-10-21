// Copyright (c) 2023 Tom Hancocks
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
#include <libResourceCore/file.hpp>
#include <libKDL/unit/file.hpp>

namespace kdtool::kdl
{
    struct tool
    {
        explicit tool(const std::string& path);

        auto add_file(const std::string& file) -> void;
        auto add_definition(const std::string& symbol) -> void;
        auto set_format(const std::string& format) -> void;

        auto build() -> void;

    private:
        [[nodiscard]] auto definitions() const -> std::vector<std::string>;

    private:
        std::string m_output_path;
        resource_core::file m_output;
        std::vector<std::string> m_definitions;
        enum resource_core::file::format m_format { resource_core::file::format::extended };
        ::kdl::sema::context m_context;
        ::kdl::unit::file m_session;
    };
}