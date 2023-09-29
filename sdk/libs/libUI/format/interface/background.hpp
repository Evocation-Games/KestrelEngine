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
#include <libResource/reference.hpp>

namespace ui::format
{
    struct background
    {
    public:
        enum class type : std::uint8_t {
            none = 0,
            is_static = 1,
            is_dynamic = 2,
        };

        background() = default;
        explicit background(const resource::reference& fill);
        explicit background(const resource::reference& top, const resource::reference& fill, const resource::reference& bottom);

        [[nodiscard]] auto type() const -> enum type;

        [[nodiscard]] auto top() const -> resource::reference;
        [[nodiscard]] auto fill() const -> resource::reference;
        [[nodiscard]] auto bottom() const -> resource::reference;

    private:
        enum type m_type { type::none };
        resource::reference m_fill;
        resource::reference m_top;
        resource::reference m_bottom;
    };
}