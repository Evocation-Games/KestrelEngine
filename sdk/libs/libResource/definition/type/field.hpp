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

#include <libFoundation/hashing/hashing.hpp>
#include <libResource/decorator/decoratable.hpp>
#include <libResource/definition/type/descriptor.hpp>
#include <libResource/definition/type/value.hpp>

namespace resource::definition::binary_template
{
    struct field;
}

namespace resource::definition::type
{
    struct field: public decoratable
    {
    public:
        struct repeatable_info
        {
        public:
            explicit repeatable_info(bool enabled = false);

            auto set_bounds(std::int32_t lower, std::int32_t upper) -> void;
            auto set_count_field(const binary_template::field *field) -> void;

            [[nodiscard]] auto lower_bound() const -> std::int32_t;
            [[nodiscard]] auto upper_bound() const -> std::int32_t;
            [[nodiscard]] auto enabled() const -> bool;
            [[nodiscard]] auto count_field() const -> const binary_template::field*;

            [[nodiscard]] auto has_count_field() const -> bool;

        private:
            bool m_enabled { false };
            std::int32_t m_lower_bound { 0 };
            std::int32_t m_upper_bound { 0 };
            const binary_template::field *m_count_field { nullptr };
        };

    public:
        typedef foundation::hashing::value hash_value;

        explicit field(const std::string& name);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto hash() const -> hash_value;

        auto repeatable() -> repeatable_info&;
        auto make_repeatable(std::int32_t lower, std::int32_t upper) -> repeatable_info&;

        [[nodiscard]] auto values() const -> const std::vector<field_value>&;
        [[nodiscard]] auto value_count() const -> std::size_t;
        [[nodiscard]] auto value_at(std::int32_t idx) const -> const field_value&;
        auto add_value(const field_value& value) -> void;

    private:
        std::string m_name;
        hash_value m_hash;
        repeatable_info m_repeatable;
        std::vector<field_value> m_values;
    };
}