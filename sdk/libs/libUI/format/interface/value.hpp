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
#include <string>
#include <variant>
#include <vector>
#include <libUI/format/interface/indexed_list_lookup.hpp>
#include <libUI/format/interface/indexed_lookup.hpp>
#include <libUI/format/interface/named_lookup.hpp>
#include <libUI/format/interface/lookup.hpp>
#include <libResource/reference.hpp>

namespace ui::format
{
    struct value
    {
    public:
        enum class type : std::uint16_t {
            no_value = 0x0000,
            string = 0x0001,
            integer = 0x0002,
            unsigned_integer = 0x0003,
            color = 0x0004,
            indexed_lookup = 0x0005,
            named_lookup = 0x0006,
            string_list = 0x0007,
            integer_list = 0x0008,
            unsigned_integer_list = 0x0009,
            lookup = 0x000a,
            indexed_list_lookup = 0x000b,
            image = 0x000c,
            image_list = 0x000d
        };

        value() = default;
        explicit value(const std::string& value);
        explicit value(std::int64_t value);
        explicit value(std::uint64_t value);
        explicit value(const indexed_lookup& value);
        explicit value(const indexed_list_lookup& value);
        explicit value(const named_lookup& value);
        explicit value(const lookup& value);
        explicit value(std::uint32_t value);
        explicit value(const std::vector<std::string>& value);
        explicit value(const std::vector<std::int64_t>& value);
        explicit value(const std::vector<std::uint64_t>& value);
        explicit value(const resource::reference& value);
        explicit value(const std::vector<resource::reference>& value);

        [[nodiscard]] auto type() const -> enum type;

        template<typename T>
        [[nodiscard]] auto get() const -> T
        {
            return std::get<T>(m_value);
        }

    private:
        enum type m_type { type::no_value };
        std::variant<
            std::string,
            std::int64_t,
            std::uint64_t,
            indexed_lookup,
            indexed_list_lookup,
            named_lookup,
            lookup,
            std::uint32_t,
            std::vector<std::string>,
            std::vector<std::int64_t>,
            std::vector<std::uint64_t>,
            resource::reference,
            std::vector<resource::reference>
        > m_value;
    };
}