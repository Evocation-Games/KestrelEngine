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

namespace resource::definition::binary_template
{
    struct instance;

    struct type
    {
    public:
        enum $type
        {
            DBYT = 'DBYT', DWRD = 'DWRD', DLNG = 'DLNG', DQWD = 'DQWD',
            HBYT = 'HBYT', HWRD = 'HWRD', HLNG = 'HLNG', HQWD = 'HQWD',
            RECT = 'RECT', PSTR = 'PSTR', CSTR = 'CSTR', Cnnn = 'Cnnn',
            LSTR = 'LSTR', OSTR = 'OSTR', CHAR = 'CHAR', OCNT = 'OCNT',
            LSTC = 'LSTC', LSTE = 'LSTE', BBIT = 'BBIT', BOOL = 'BOOL',
            HEXD = 'HEXD', RSRC = 'RSRC', NESTED = 'Nnnn',
        };

    public:
        type() = default;
        explicit type(enum $type type);
        explicit type(const instance *nested);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto value() const -> enum $type;
        [[nodiscard]] auto nested_type() const -> const instance *;

    private:
        enum $type m_base { DWRD };
        std::int64_t m_size { 0 };
        struct {
            const instance *definition { nullptr };
        } m_nested;
    };
}