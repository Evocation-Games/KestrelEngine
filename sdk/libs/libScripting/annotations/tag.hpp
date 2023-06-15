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

namespace scripting::annotation
{
    enum class tag
    {
        as_namespace,
        is_constructor,
        is_setter,
        is_getter,
        available,
        deprecated,
        mutability,
        is_enrollment,
        requires_enrollment_name,
        reference,
        symbol,
        parameter_type,
        template_variant,
        undocumented,
        undefined
    };

    struct tag_name
    {
        static auto from(enum tag tag) -> std::string;
        static auto from(const std::string& name) -> enum tag;

    private:
        static constexpr const char *undefined = "undefined";
        static constexpr const char *undocumented = "undocumented";
        static constexpr const char *available = "available";
        static constexpr const char *deprecated = "deprecated";
        static constexpr const char *mutability = "mutability";
        static constexpr const char *getter = "getter";
        static constexpr const char *setter = "setter";
        static constexpr const char *constructor = "constructor";
        static constexpr const char *as_namespace = "namespace";
        static constexpr const char *enrollment = "enrollment";
        static constexpr const char *enrollment_name = "enrollment-name";
        static constexpr const char *reference = "reference";
        static constexpr const char *symbol = "symbol";
        static constexpr const char *parameter_type = "parameter-type";
        static constexpr const char *template_variant = "template-variant";
    };

}