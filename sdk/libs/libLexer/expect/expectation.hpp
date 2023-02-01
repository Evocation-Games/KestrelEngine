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
#include <functional>
#include <libLexer/lexeme.hpp>
#include <libFoundation/stream/expectation.hpp>

namespace lexer
{
    /**
     * The lexer::expectation is used to represent an expectation of a token in the parser lexeme
     * stream. Expectations can contain a string value, a lexeme type and a truthiness expectation.
     */
    struct expectation
    {
    public:
        explicit expectation(enum lexeme_type Ty);
        explicit expectation(const std::string& Tx);
        explicit expectation(const std::vector<std::string>& Tx_options);
        expectation(enum lexeme_type Ty, const std::string& Tx);
        expectation(enum lexeme_type Ty, const std::vector<std::string>& Tx_options);

        [[nodiscard]] auto to_be(bool r) const -> foundation::expectation_function<lexeme>;
        [[nodiscard]] auto be_true() const -> foundation::expectation_function<lexeme>;
        [[nodiscard]] auto be_false() const -> foundation::expectation_function<lexeme>;

    private:
        enum lexeme_type m_Ty;
        std::vector<std::string> m_Tx_options;

        static constexpr std::int32_t any = -1;
    };
}
