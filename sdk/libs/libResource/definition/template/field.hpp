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
#include <libFoundation/hashing/hashing.hpp>
#include <libResource/definition/template/type.hpp>

namespace resource::definition::binary_template
{
    struct instance;

    struct field
    {
    public:
        typedef foundation::hashing::value hash_value;

        field(const field&) = default;
        field(field&&) = default;
        field(const struct type& type, const std::string& label);
        field(enum type::$type type, const std::string& label);
        field(enum type::$type type, std::uint16_t size, const std::string& label);

        auto operator=(const field&) -> field& = default;
        auto operator=(field&&) -> field& = default;

        [[nodiscard]] auto has_nested_type() const -> bool;
        [[nodiscard]] auto nested_type() const -> const instance *;

        [[nodiscard]] auto type() const -> const struct type&;
        [[nodiscard]] auto label() const -> std::string;

        [[nodiscard]] auto hash() const -> hash_value;

        [[nodiscard]] auto skip_length() const -> std::size_t;
        [[nodiscard]] auto size() const -> std::uint16_t;

        [[nodiscard]] auto is_list() const -> bool;
        [[nodiscard]] auto list_fields() const -> const std::vector<field>&;
        [[nodiscard]] auto list_field_named(const std::string& label) const -> const field *;
        auto add_list_field(const field& item) -> void;
        auto add_list_field(const struct type& type, const std::string& label) -> void;
        auto add_list_field(enum type::$type type, const std::string& label) -> void;
        auto add_list_field(enum type::$type type, std::uint16_t size, const std::string& label) -> void;

    private:
        struct type m_type;
        std::string m_label;
        std::uint16_t m_size { 0 };
        std::vector<field> m_list_fields;
    };
}