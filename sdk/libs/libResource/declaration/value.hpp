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
#include <type_traits>
#include <libResource/reference.hpp>
#include <libResource/definition/type/instance.hpp>

namespace resource
{
    struct value_container
    {
        enum class type { nil, integer, string, reference, data };

        value_container() = default;
        explicit value_container(const std::string& value);
        explicit value_container(const std::vector<std::uint8_t>& bytes);
        explicit value_container(const std::uint8_t *bytes, std::size_t length);
        explicit value_container(const reference& value);

        value_container(const value_container&) = default;
        value_container(value_container&&) noexcept = default;

        auto operator=(const value_container&) -> value_container& = default;
        auto operator=(value_container&&) -> value_container& = default;

        [[nodiscard]] auto type() const -> enum type;

        template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        explicit value_container(T value)
            : m_type(type::integer), m_value(value)
        {}

        template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        [[nodiscard]] auto integer_value() const -> T
        {
            return std::get<__int128>(m_value);
        }

        [[nodiscard]] auto string_value() const -> std::string;
        [[nodiscard]] auto reference_value() const -> reference;
        [[nodiscard]] auto data_value() const -> std::vector<std::uint8_t>;

    private:
        enum type m_type { type::nil };
        std::variant<char, __int128, std::string, reference, std::vector<std::uint8_t>> m_value { '\0' };
    };
}