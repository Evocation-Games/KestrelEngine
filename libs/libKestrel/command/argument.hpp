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

#include <cstdint>
#include <vector>

namespace kestrel::command
{
    struct argument
    {
    public:
        enum type : std::uint8_t
        {
            key_path = 0,
            more_than = 1,
            more_than_or_equal = 2,
            equal_to = 3,
            not_equal_to = 4,
            less_than_or_equal = 5,
            less_than = 6,
            inside = 7,
        };

        argument(enum type type, std::uint64_t value);
        argument(enum type type, std::vector<std::int64_t> values);

        [[nodiscard]] auto keypath_hash_value() const -> std::uint64_t;

    private:
        enum type m_type;
        std::vector<std::int64_t> m_values;
        std::uint64_t m_keypath_hash_value;
    };
}