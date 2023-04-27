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
#include <cstdlib>
#include <cassert>

namespace kestrel::memory
{
    template<typename T, std::size_t C>
    struct slab
    {
        typedef std::uint64_t reference;

        slab()
        {
            // TODO: Investigate memory alignment here?
            m_queue.item_stack_base = new reference[C];
            m_queue.allocated_base = new reference[C];
            m_queue.next_item = m_queue.item_stack_base;
            for (std::size_t n = 0; n < C; ++n) {
                *m_queue.next_item++ = n;
            }
            
            m_queue.item_stack_top = --m_queue.next_item;
            m_queue.allocated_ptr = m_queue.allocated_base;
        }

        ~slab()
        {
            delete[] m_pool;
            delete m_queue.item_stack_base;
            delete m_queue.allocated_base;
        }

        [[nodiscard]] inline auto allocated() const -> std::size_t { return m_queue.allocated; }
        [[nodiscard]] inline auto remaining() const -> std::size_t { return C - allocated(); }
        [[nodiscard]] inline auto depleted() const -> bool { return remaining() == 0; }

        inline auto operator[](reference item) noexcept -> T&
        {
            return m_pool[item];
        }

        inline auto first() noexcept -> T&
        {
            return m_pool[*m_queue.allocated_base];
        }

        inline auto purge() -> void
        {
            m_queue.next_item = m_queue.item_stack_base;
            m_queue.allocated_ptr = m_queue.allocated_base;
            m_queue.allocated = 0;
        }

        inline auto release(reference item) -> void
        {
            if (m_queue.next_item >= m_queue.item_stack_top || m_queue.allocated == 0) {
                return;
            }
            *++m_queue.next_item = item;
            m_queue.allocated_ptr--;
            m_queue.allocated--;
        }

        inline auto request() noexcept -> reference
        {
            // Do not check for exceptions here, as this code is likely going to be _VERY HOT_.
            // We should apply assertions to test this in debug to find edge cases.
            assert(!depleted());
            m_queue.allocated++;
            *m_queue.allocated_ptr++ = *m_queue.next_item;
            return *m_queue.next_item--;
        }

        inline auto get(reference item) noexcept -> T&
        {
            return m_pool[item];
        }

        inline auto get_allocated(std::uint64_t item) noexcept -> T&
        {
            return m_pool[m_queue.allocated_base[item]];
        }

        inline auto available() noexcept -> bool
        {
            return (m_pool != nullptr);
        }

    private:
        T *m_pool { new T[C] };

        struct {
            reference *item_stack_base { nullptr };
            reference *item_stack_top { nullptr };
            reference *next_item { nullptr };
            reference *allocated_base { nullptr };
            reference *allocated_ptr { nullptr };
            std::size_t allocated { 0 };
        } m_queue;

    };
}
