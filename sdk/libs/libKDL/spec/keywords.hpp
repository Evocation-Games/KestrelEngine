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

namespace kdl::spec::keywords
{
    static constexpr const char *type_keyword = "type";
    static constexpr const char *declare_keyword = "declare";
    static constexpr const char *new_keyword = "new";
    static constexpr const char *override_keyword = "override";
    static constexpr const char *duplicate_keyword = "duplicate";
    static constexpr const char *field_keyword = "field";
    static constexpr const char *template_keyword = "template";
    static constexpr const char *assert_keyword = "assert";
    static constexpr const char *component_keyword = "component";
    static constexpr const char *files_keyword = "files";
    static constexpr const char *types_keyword = "types";
    static constexpr const char *repeatable_keyword = "repeatable";
    static constexpr const char *as_keyword = "as";
    static constexpr const char *constructor_keyword = "constructor";

    static auto vector() -> std::vector<std::string>
    {
        return {
            type_keyword, declare_keyword, new_keyword, override_keyword,
            duplicate_keyword, field_keyword, template_keyword, assert_keyword,
            component_keyword, files_keyword, types_keyword, repeatable_keyword,
            as_keyword, constructor_keyword,
        };
    }
}