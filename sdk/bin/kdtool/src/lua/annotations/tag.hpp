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

namespace kdtool::lua_api
{
    struct annotation_tag
    {
        enum class name {
            unknown,
            symbol,
            available,
            deprecated,
            undocumented,
            getter,
            setter,
            constructor,
            is_mutable,
            as_namespace,
            enrollment,
            reference,
            named,
            type_fix,
            template_variant,
        };

        annotation_tag() = default;
        explicit annotation_tag(const std::string& tag_string);
        annotation_tag(const std::string& tag_name, const std::string& tag_value);
        explicit annotation_tag(enum name tag_name, const std::string& tag_value);

        [[nodiscard]] auto name() const -> enum name;
        [[nodiscard]] auto value() const -> std::string;

    private:
        struct constants {
            static constexpr const char *symbol = "symbol";
            static constexpr const char *available = "available";
            static constexpr const char *deprecated = "deprecated";
            static constexpr const char *undocumented = "undocumented";
            static constexpr const char *getter = "getter";
            static constexpr const char *setter = "setter";
            static constexpr const char *constructor = "constructor";
            static constexpr const char *is_mutable = "mutable";
            static constexpr const char *as_namespace = "namespace";
            static constexpr const char *enrollment = "enrollment";
            static constexpr const char *reference = "reference";
            static constexpr const char *named = "named";
            static constexpr const char *type_fix = "type-fix";
            static constexpr const char *template_variant = "template-variant";
        };

        static auto name(const std::string& value) -> enum name;

    private:
        enum name m_name;
        std::string m_value;

    };
}