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

#include <cstdint>
#include <numeric>
#include <string>

namespace resource
{
    struct reference
    {
    public:
        typedef std::int64_t identifier;

    public:
        reference() = default;

        explicit reference(identifier id, const std::string& type_name = "", const std::string& container = "");

        static auto auto_assigning() -> reference;
        static auto any() -> reference;

        [[nodiscard]] auto id() const -> identifier;
        [[nodiscard]] auto type_name() const -> std::string;
        [[nodiscard]] auto container_name() const -> std::string;

        [[nodiscard]] auto has_type_name() const -> bool;
        [[nodiscard]] auto has_container() const -> bool;
        [[nodiscard]] auto is_auto() const -> bool;
        [[nodiscard]] auto is_any() const -> bool;
        [[nodiscard]] auto is_type_name(const std::string& type_name) const -> bool;
        [[nodiscard]] auto is_container(const std::string& container) const -> bool;

        [[nodiscard]] auto with_id(identifier id) const -> reference;
        [[nodiscard]] auto with_type_name(const std::string& type_name) const -> reference;
        [[nodiscard]] auto with_container(const std::string& container) const -> reference;

    private:
        static constexpr identifier auto_value = std::numeric_limits<identifier>::min();
        static constexpr identifier any_value = auto_value + 1;

        identifier m_id { any_value };
        std::string m_type_name;
        std::string m_container;
    };
}