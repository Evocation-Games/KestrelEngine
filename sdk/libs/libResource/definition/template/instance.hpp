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
#include <libResource/definition/template/field.hpp>

namespace resource::definition::binary_template
{
    struct instance
    {
    public:
        instance() = default;

        auto add_field(struct type& type, const std::string& label) -> void;
        auto add_field(enum type::$type type, const std::string& label) -> void;
        auto add_field(enum type::$type type, std::uint16_t size, const std::string& label) -> void;
        auto add_field(struct field& field) -> void;

        [[nodiscard]] auto field_count() const -> std::size_t;
        [[nodiscard]] auto field_named(const std::string& name) const -> const field&;
        [[nodiscard]] auto all_fields() const -> std::vector<field>;
        [[nodiscard]] auto has_field_named(const std::string& name) const -> bool;

        [[nodiscard]] auto field_named(const std::vector<std::string>& name) const -> const field&;

    private:
        std::vector<field> m_fields;
    };
}