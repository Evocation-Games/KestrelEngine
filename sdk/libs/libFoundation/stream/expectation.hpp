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

#include <functional>
#include <concepts>

namespace foundation
{

    template<typename T>
    struct expectation_function
    {
    public:
        typedef std::function<auto(T)->bool> inner_function;

    public:
        explicit expectation_function(const inner_function& fn) : m_fn(fn) {}

        [[nodiscard]] auto to_be(bool r) const -> expectation_function<T>
        {
            return expectation_function<T>([&] (T t) -> bool {
                return (this->call(t) == r);
            });
        }

        [[nodiscard]] auto be_true() const -> expectation_function<T> { return to_be(true); }
        [[nodiscard]] auto be_false() const -> expectation_function<T> { return to_be(false); }

        [[nodiscard]] auto optional() const -> expectation_function<T>
        {
            expectation_function<T> fn(m_fn);
            fn.m_optional = true;
            return fn;
        }
        [[nodiscard]] auto is_optional() const -> bool { return m_optional; }

        auto on_expectation_failure(const std::function<auto(T)->void>& handler) -> void
        {
            m_has_failure_handler = true;
            m_failure_handler = handler;
        }

        auto fail(const T& t) const -> void
        {
            if (m_has_failure_handler) {
                m_failure_handler(t);
            }
            else {
                throw std::runtime_error("[stream] Could not ensure the correctness of the item.");
            }
        }

        auto operator()(T t) const -> bool { return m_fn(t); }

    private:
        inner_function m_fn;
        bool m_optional { false };
        bool m_has_failure_handler { false };
        std::function<auto(T)->void> m_failure_handler {};
    };

}