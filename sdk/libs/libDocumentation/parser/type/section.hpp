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

namespace documentation
{
    struct section
    {
        struct name
        {
            static constexpr const char *description = "@description";
            static constexpr const char *example = "@example";
            static constexpr const char *param = "@param";
            static constexpr const char *tparam = "@tparam";
            static constexpr const char *returns = "@returns";
            static constexpr const char *warning = "@warning";
            static constexpr const char *see_also = "@see_also";
            static constexpr const char *important = "@important";
        };

        section(const std::string& name, const std::string& body, const std::string& title = "")
            : m_name(name), m_body(body), m_title(title)
        {}

        [[nodiscard]] virtual auto name() const -> std::string { return m_name; }
        [[nodiscard]] virtual auto title() const -> std::string { return m_title; }
        [[nodiscard]] virtual auto body() const -> std::string { return m_body; }

    private:
        std::string m_name;
        std::string m_title;
        std::string m_body;
    };

}