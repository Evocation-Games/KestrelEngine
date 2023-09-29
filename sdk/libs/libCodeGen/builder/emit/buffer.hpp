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

#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/builder/emit/segment.hpp>
#include <libFoundation/system/filesystem/path.hpp>

namespace codegen::emit
{
    class buffer
    {
    public:
        buffer() = default;

        auto write(const std::shared_ptr<ast::node>& node) -> void;
        auto write(const std::vector<segment>& segments) -> void;
        auto write(const segment& segment) -> void;

        auto save(const foundation::filesystem::path& path) -> void;

    private:
        auto write(const std::string& str) -> void;

        auto indent() -> void;
        auto outdent() -> void;
        auto newline() -> void;

        [[nodiscard]] auto indentation_string() const -> std::string;

    private:
        std::string m_output;
        std::int32_t m_indentation { 0 };
    };
}