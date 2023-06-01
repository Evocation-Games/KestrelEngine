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
#include <vector>
#include <initializer_list>

#include <libFoundation/stream/expectation.hpp>

namespace foundation
{

    template<typename T>
    struct stream
    {
    public:
        stream() = default;
        explicit stream(const std::vector<T>& items) : m_items(items) {}
        stream(const std::initializer_list<T>& items) : m_items(items) {}

        // Accessors

        [[nodiscard]] auto size() const -> std::size_t { return m_items.size(); }
        [[nodiscard]] auto at(std::int32_t idx) -> T { return m_items.at(idx); }
        [[nodiscard]] auto finished(std::int32_t offset = 0, std::size_t count = 1) const -> bool
        {
            auto ptr = m_ptr + offset;
            auto end_ptr = ptr + count;
            auto size = m_items.size();
            return (ptr > size) || (end_ptr > size);
        }

        // Reset

        auto reset() -> void
        {
            m_ptr = 0;
            clear_pushed_items();
        }

        // Actions / Manipulation

        auto consume(expectation_function<T> expect) -> stream<T>
        {
            std::vector<T> items;
            while (!finished() && expect(peek())) {
                items.emplace_back(read());
            }
            return stream<T>(items);
        }

        auto advance(std::int32_t delta = 1) -> void
        {
            for (auto n = 0; n < delta; ++n) {
                if (!m_tmp_items.empty()) {
                    m_tmp_items.erase(m_tmp_items.begin());
                }
                else {
                    m_ptr++;
                }
            }
        }

        auto push(const T& item) -> void { m_tmp_items.emplace_back(item); }
        auto push(std::initializer_list<T> items) -> void { m_tmp_items = { items }; }
        auto clear_pushed_items() -> void { m_tmp_items.clear(); }

        [[nodiscard]] auto peek(std::int32_t offset = 0) const -> T
        {
            if (!m_tmp_items.empty() && (offset >= 0 && offset < m_tmp_items.size())) {
                return m_tmp_items.at(offset);
            }
            else if (!m_tmp_items.empty() && offset >= m_tmp_items.size()) {
                offset -= m_tmp_items.size();
            }

            if (finished(offset, 1)) {
                throw std::logic_error("[lexeme_stream] Attempted to access lexeme beyond end of lexeme_stream.");
            }
            return m_items[m_ptr + offset];
        }

        auto read(std::int32_t offset = 0) -> T
        {
            auto item = peek(offset);
            if (m_tmp_items.empty() || (offset >= m_tmp_items.size())) {
                advance(offset + 1);
            }
            else {
                for (auto n = 0; n <= offset; ++n) {
                    m_tmp_items.erase(m_tmp_items.begin());
                }
            }
            return item;
        }

        [[nodiscard]] auto expect(std::initializer_list<expectation_function<T>> expect, std::int32_t offset = 0) const -> bool
        {
            auto ptr = offset;
            for (const auto& f : expect) {
                if (m_ptr + ptr >= size() || !f(peek(ptr++))) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] auto expect_any(std::initializer_list<expectation_function<T>> expect, std::int32_t offset = 0) const -> bool
        {
            return expect_any(std::vector(expect), offset);
        }

        [[nodiscard]] auto expect_any(const std::vector<expectation_function<T>>& expect, std::int32_t offset = 0) const -> bool
        {
            for (const auto& f : expect) {
                if (f(peek(offset))) {
                    return true;
                }
            }
            return false;
        }

        auto ensure(std::initializer_list<expectation_function<T>> expect) -> stream<T>
        {
            std::vector<T> items;
            for (const auto& f : expect) {
                auto item = read();
                if (!f(item)) {
                    throw std::logic_error("[stream] Could not ensure the correctness of the item.");
                }
                items.emplace_back(item);
            }
            return stream(items);
        }

        auto insert(const std::vector<T>& items, std::int32_t offset = 0) -> void
        {
            if (finished(offset, 1)) {
                // We're trying to insert at the end of the stream.
                m_items.insert(m_items.end(), items.begin(), items.end());
            }
            else {
                // We're inserting midstream.
                auto insertion_ptr = m_items.begin() + m_ptr + offset;
                m_items.insert(insertion_ptr, items.begin(), items.end());
            }
        }

        auto insert(const stream<T>& stream, std::int32_t offset = 0) -> void { insert(stream.m_items, offset); }
        auto insert(const T& item, std::int32_t offset = 0) -> void { insert(std::vector<T>({ item }), offset); }

        auto append(const T& item) -> void { m_items.emplace_back(item); }

    private:
        std::int32_t m_ptr { 0 };
        std::vector<T> m_items;
        std::vector<T> m_tmp_items;
    };

}