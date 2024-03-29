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

#include <vector>
#include <string>

namespace kdl::spec::decorators
{
    static constexpr const char *builtin = "__builtin";
    static constexpr const char *explicit_type = "__explicit";
    static constexpr const char *no_declaration = "no_declaration";
    static constexpr const char *synthesize = "synthesize";
    static constexpr const char *constexpression = "constexpr";
    static constexpr const char *api = "api";
    static constexpr const char *example = "example";
    static constexpr const char *deprecated = "deprecated";
    static constexpr const char *condition = "if";
    static constexpr const char *not_condition = "if_not";
    static constexpr const char *merge_bitmask = "MergeBitmask";
    static constexpr const char *setter = "WantsSetter";

    static auto vector() -> std::vector<std::string>
    {
        return {
            builtin, explicit_type, no_declaration, synthesize, constexpression,
            api, example, deprecated, condition, merge_bitmask, setter, not_condition
        };
    }
}