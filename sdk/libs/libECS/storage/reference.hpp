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

namespace ecs
{
    struct reference
    {
        reference() = default;

        explicit reference(std::uint32_t value, std::function<auto()->void> on_release)
            : m_id(new identity(on_release)), m_value(value)
        {}

        reference(const reference& r) : m_id(r.m_id->retain()), m_value(r.m_value) {}

        ~reference()
        {
            if (m_id) m_id->release();
        }

        auto operator= (const reference& r) -> reference&
        {
            if (m_id) {
                m_id->release();
            }
            m_id = r.m_id->retain();
            m_value = r.m_value;
            return *this;
        }

        [[nodiscard]] inline auto value() const -> std::uint32_t { return m_value; }

    private:
        struct identity
        {
            std::function<auto()->void> release_callback;
            std::uint32_t ref_count { 1 };

            identity(std::function<auto()->void> callback) : release_callback(callback) {}

            auto retain() -> identity * { ref_count++; return this; }
            auto release() -> void
            {
                if (--ref_count == 0) {
                    release_callback();
                }
            }
        };

        identity *m_id { nullptr };
        std::uint32_t m_value { 0 };
    };
}