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

#include <libResource/decorator/decoratable.hpp>
#include <libResource/definition/template/field.hpp>
#include <libResource/definition/type/descriptor.hpp>
#include <libInterpreter/scope/scope.hpp>

namespace resource::definition::type
{
    struct field_value: public decoratable
    {
    public:
        explicit field_value(const std::string& base_name);
        explicit field_value(const binary_template::field *field);

        [[nodiscard]] auto base_name() const -> std::string;
        [[nodiscard]] auto extended_name(const interpreter::scope& scope) const -> std::string;
        auto add_name_extension(const std::string& name) -> void;
        [[nodiscard]] auto has_extended_name() const -> bool;

        [[nodiscard]] auto has_export_name() const -> bool;
        [[nodiscard]] auto export_name() const -> std::string;
        auto set_export_name(const std::string& name) -> void;

        [[nodiscard]] auto is_type_explicit() const -> bool;
        [[nodiscard]] auto type() const -> const descriptor&;
        auto set_type(const descriptor& type, bool is_explicit) -> void;

    private:
        const binary_template::field *m_field { nullptr };
        descriptor m_type_descriptor;
        std::string m_base_name;
        std::vector<std::string> m_name_extensions;
    };
}