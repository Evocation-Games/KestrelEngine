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
#include <variant>
#include <vector>

namespace interpreter
{
    struct token
    {
    public:
        enum type
        {
            integer, decimal, string, boolean, reference,
            plus, minus, multiply, divide, amp, pipe, tilde, carat, exclaim,
            left_shift, right_shift, identifier, identifier_path, l_paren, r_paren,
            comma, semi, equals
        };

    public:
        explicit token(enum type type) : m_type(type), m_value(0) {}
        explicit token(const std::string& str) : m_type(string), m_value(str) {}
        explicit token(const std::vector<std::string>& v) : m_type(string), m_value(v) {}
        explicit token(std::int64_t value) : m_type(integer), m_value(value) {}
        explicit token(bool value) : m_type(boolean), m_value(value) {}
        explicit token(double value) : m_type(decimal), m_value(value) {}

        static auto id(const std::string& str) -> token
        {
            token tk(str);
            tk.m_type = identifier;
            return tk;
        }

        static auto id(const std::vector<std::string>& v) -> token
        {
            token tk(v);
            tk.m_type = identifier_path;
            return tk;
        }

        [[nodiscard]] auto type() const -> enum type
        {
            return m_type;
        }

        [[nodiscard]] auto string_value() const -> std::string
        {
            if (m_value.index() == 0) {
                return std::get<std::string>(m_value);
            }
            else if (m_value.index() == 4) {
                auto values = std::get<std::vector<std::string>>(m_value);
                std::string out;
                for (const auto& v : values) {
                    out += (out.empty() ? "" : ".") + v;
                }
                return out;
            }
            else {
                return "";
            }
        }

        [[nodiscard]] auto integer_value() const -> std::int64_t
        {
            return static_cast<std::int64_t>(std::get<__int128>(m_value));
        }

        [[nodiscard]] auto decimal_value() const -> double
        {
            return std::get<double>(m_value);
        }

        [[nodiscard]] auto bool_value() const -> bool
        {
            return std::get<bool>(m_value);
        }

        [[nodiscard]] auto is(enum type type) const -> bool
        {
            return m_type == type;
        }

        [[nodiscard]] auto is(const std::string& value) const -> bool
        {
            return string_value() == value;
        }

        [[nodiscard]] auto is(const std::vector<std::string>& options) const -> bool
        {
            if (m_value.index() == 0) {
                for (const auto& str : options) {
                    if (string_value() == str) {
                        return true;
                    }
                }
            }
            return false;
        }

        [[nodiscard]] auto is(const enum type type, const std::string& value) const -> bool
        {
            return is(type) && is(value);
        }

        [[nodiscard]] auto is(const enum type type, const std::vector<std::string>& options) const -> bool
        {
            return is(type) && is(options);
        }

        [[nodiscard]] auto operator_value() const -> std::int8_t
        {
            switch (m_type) {
                case token::plus:
                case token::minus:      return op_prec::addition;
                case token::multiply:
                case token::divide:     return op_prec::multiplication;
                case token::carat:      return op_prec::carat;
                case token::left_shift:
                case token::right_shift:return op_prec::shift;
                case token::pipe:       return op_prec::exclusive_or;
                case token::amp:        return op_prec::logical_and;
                default:                return op_prec::unknown;
            }
        }

        [[nodiscard]] auto left_associative() const -> bool
        {
            return m_type != token::carat;
        }

        auto operator+(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() + rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(static_cast<std::int64_t>(integer_value() + rhs.decimal_value()));
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() + (rhs.bool_value() ? 1 : 0));
                }
            }
            else if (m_type == token::string) {
                if (rhs.m_type == token::integer) {
                    return token(string_value() + std::to_string(rhs.integer_value()));
                }
                else if (rhs.m_type == token::string) {
                    return token(string_value() + rhs.string_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(string_value() + std::to_string(rhs.decimal_value()));
                }
                else if (rhs.m_type == token::boolean) {
                    return token(string_value() + (rhs.bool_value() ? "true" : "false"));
                }
            }
            else if (m_type == token::decimal) {
                if (rhs.m_type == token::integer) {
                    return token(decimal_value() + rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(decimal_value() + rhs.decimal_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(decimal_value() + (rhs.bool_value() ? 1 : 0));
                }
            }
            throw std::logic_error("");
        }

        auto operator-(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() - rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(static_cast<std::int64_t>(integer_value() - rhs.decimal_value()));
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() - (rhs.bool_value() ? 1 : 0));
                }
            }
            else if (m_type == token::decimal) {
                if (rhs.m_type == token::integer) {
                    return token(decimal_value() - rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(decimal_value() - rhs.decimal_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(decimal_value() - (rhs.bool_value() ? 1 : 0));
                }
            }
            throw std::logic_error("");
        }

        auto operator*(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() * rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(static_cast<std::int64_t>(integer_value() * rhs.decimal_value()));
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() * (rhs.bool_value() ? 1 : 0));
                }
            }
            else if (m_type == token::decimal) {
                if (rhs.m_type == token::integer) {
                    return token(decimal_value() * rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(decimal_value() * rhs.decimal_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(decimal_value() * (rhs.bool_value() ? 1 : 0));
                }
            }
            throw std::logic_error("");
        }

        auto operator/(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() / rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(static_cast<std::int64_t>(integer_value() / rhs.decimal_value()));
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() / (rhs.bool_value() ? 1 : 0));
                }
            }
            else if (m_type == token::decimal) {
                if (rhs.m_type == token::integer) {
                    return token(decimal_value() / rhs.integer_value());
                }
                else if (rhs.m_type == token::decimal) {
                    return token(decimal_value() / rhs.decimal_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(decimal_value() / (rhs.bool_value() ? 1 : 0));
                }
            }
            throw std::logic_error("");
        }

        auto operator&(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() & rhs.integer_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() & (rhs.bool_value() ? 1 : 0));
                }
            }
            else if (m_type == token::boolean) {
                if (rhs.m_type == token::boolean) {
                    return token(static_cast<bool>(bool_value() & rhs.bool_value()));
                }
            }
            throw std::logic_error("");
        }

        auto operator|(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() | rhs.integer_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() | (rhs.bool_value() ? 1 : 0));
                }
            }
            else if (m_type == token::boolean) {
                if (rhs.m_type == token::boolean) {
                    return token(static_cast<bool>(bool_value() | rhs.bool_value()));
                }
            }
            throw std::logic_error("");
        }

        auto operator<<(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() << rhs.integer_value());
                }
            }
            throw std::logic_error("");
        }

        auto operator>>(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() >> rhs.integer_value());
                }
            }
            throw std::logic_error("");
        }

        auto operator^(const token& rhs) const -> token
        {
            if (m_type == token::integer) {
                if (rhs.m_type == token::integer) {
                    return token(integer_value() ^ rhs.integer_value());
                }
                else if (rhs.m_type == token::boolean) {
                    return token(integer_value() ^ (rhs.bool_value() ? 1 : 0));
                }
            }
            throw std::logic_error("");
        }

        auto operator~() const -> token
        {
            if (m_type == token::integer) {
                return token(~integer_value());
            }
            else if (m_type == token::boolean) {
                return token(static_cast<bool>(!bool_value()));
            }
            throw std::logic_error("");
        }

    private:
        enum type m_type;
        std::variant<std::string, bool, __int128, double, std::vector<std::string>> m_value;

        struct op_prec
        {
            static constexpr std::int8_t unknown = 0;
            static constexpr std::int8_t addition = 2;
            static constexpr std::int8_t multiplication = 3;
            static constexpr std::int8_t carat = 4;
            static constexpr std::int8_t shift = 5;
            static constexpr std::int8_t exclusive_or = 6;
            static constexpr std::int8_t logical_and = 7;
        };
    };
}