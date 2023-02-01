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
#include <libKDL/lexer/lexeme.hpp>

namespace kdl::session
{
    struct type_field_repeatable
    {
    public:
        explicit type_field_repeatable(bool enabled = false);

        auto set_bounds(std::int32_t lower, std::int32_t upper) -> void;
        auto set_count_field(const lexer::lexeme& field) -> void;

        [[nodiscard]] auto lower_bound() const -> std::int32_t;
        [[nodiscard]] auto upper_bound() const -> std::int32_t;
        [[nodiscard]] auto enabled() const -> bool;
        [[nodiscard]] auto count_field() const -> const lexer::lexeme&;

        [[nodiscard]] auto has_count_field() const -> bool;

    private:
        bool m_enabled { false };
        std::int32_t m_lower_bound { 0 };
        std::int32_t m_upper_bound { 0 };
        lexer::lexeme m_count_field { "", kdl::lexer::lexeme::type::identifier };
    };
}