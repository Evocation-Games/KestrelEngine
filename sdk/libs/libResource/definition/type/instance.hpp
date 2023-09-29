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

#include <map>
#include <string>
#include <vector>
#include <libResource/definition/template/instance.hpp>
#include <libResource/definition/type/assertion.hpp>
#include <libResource/definition/type/field.hpp>
#include <libResource/decorator/decoratable.hpp>
#include <libResource/definition/type/constructor.hpp>

namespace resource::definition::type
{
    struct instance: public decoratable
    {
    public:
        instance(const std::string& name, const std::string& code);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto code() const -> std::string;

        [[nodiscard]] auto binary_template() const -> const binary_template::instance *;
        auto set_binary_template(const binary_template::instance& tmpl) -> void;

        [[nodiscard]] auto constructors() const -> const std::vector<constructor>&;
        auto add_constructor(const constructor& constructor) -> void;

        [[nodiscard]] auto all_fields() const -> std::vector<field>;
        [[nodiscard]] auto has_field_named(const std::string& name) const -> bool;
        [[nodiscard]] auto field_named(const std::string& name) const -> const field&;
        auto add_field(const field& field) -> void;
        auto add_field(const std::string& name) -> field&;

    private:
        std::string m_name;
        std::string m_code;
        binary_template::instance m_template;
        std::vector<assertion> m_assertions;
        std::map<field::hash_value, field> m_fields;
        std::vector<constructor> m_constructors;
    };
}