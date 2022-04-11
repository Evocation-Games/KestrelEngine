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

#include <array>
#include <type_traits>

namespace renderer::shader
{
    struct program
    {
    public:
        program() = default;
        explicit program(uintptr_t handle) : m_value({ .handle = handle }) {};
        explicit program(void *ptr) : m_value({ .ptr = ptr }) {};
        program(const program& program) = default;

        template<typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
        [[nodiscard]] inline auto get() const -> T { return reinterpret_cast<T>(m_value.ptr); }

        template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
        [[nodiscard]] inline auto get() const -> T { return static_cast<T>(m_value.handle); }

    private:
        union { void *ptr; uintptr_t handle; } m_value { .ptr = nullptr };
    };
}


