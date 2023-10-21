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

#include <libLexer/expect/expectation.hpp>

// MARK: - Construction

lexer::expectation::expectation(enum lexeme_type Ty)
    : m_Ty(Ty)
{}

lexer::expectation::expectation(const std::string &Tx)
    : m_Ty(static_cast<lexeme_type>(any)), m_Tx_options({ Tx })
{}

lexer::expectation::expectation(const std::vector<std::string> &options)
    : m_Ty(static_cast<lexeme_type>(any)), m_Tx_options(options)
{}

lexer::expectation::expectation(enum lexeme_type Ty, const std::string &Tx)
    : m_Ty(Ty), m_Tx_options({ Tx })
{}

lexer::expectation::expectation(enum lexeme_type Ty, const std::vector<std::string>& options)
    : m_Ty(Ty), m_Tx_options(options)
{}

// MARK: - Conditions

auto lexer::expectation::to_be(bool r) const -> foundation::expectation_function<lexeme>
{
    auto& Tx_options = m_Tx_options;
    auto& Ty = m_Ty;

    auto expect = foundation::expectation_function<lexeme>([Tx_options, Ty, r] (lexeme lx) -> bool {
        auto outcome = true;
        if (!Tx_options.empty() && !lx.is(Tx_options)) {
            outcome = false;
        }
        if ((static_cast<std::int32_t>(Ty) != any) && !lx.is(Ty)) {
            outcome = false;
        }
        return (outcome == r);
    });

    return expect;
}

auto lexer::expectation::be_true() const -> foundation::expectation_function<lexeme>
{
    return to_be(true);
}

auto lexer::expectation::be_false() const -> foundation::expectation_function<lexeme>
{
    return to_be(false);
}