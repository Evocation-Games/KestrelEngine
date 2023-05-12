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
#include <unordered_map>
#include <clang-c/Index.h>
#include "lua/annotations/tag.hpp"

namespace luatool::lua_api
{
    struct annotation
    {
        annotation() = default;
        explicit annotation(const std::string& string);
        explicit annotation(CXCursor cursor);

        [[nodiscard]] auto tag(enum annotation_tag::name name) const -> std::string;
        [[nodiscard]] auto has(enum annotation_tag::name tag_name) const -> bool;

    private:
        auto parse() -> void;

    private:
        struct constants {
            static constexpr const char *master_tag = "luatool";
        };

    private:
        std::string m_string;
        std::unordered_map<enum annotation_tag::name, annotation_tag> m_tags;
        bool m_verified { false };
    };
}