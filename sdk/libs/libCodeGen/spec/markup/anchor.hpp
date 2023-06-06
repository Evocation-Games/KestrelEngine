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

#include <libCodeGen/spec/markup/node.hpp>

namespace codegen::spec::markup
{
    struct anchor : public markup_node
    {
        explicit anchor(const std::shared_ptr<markup_node>& body, const std::string& href, bool wants_extension = false)
            : m_body(body), m_href(href), m_wants_extension(wants_extension)
        {}

        [[nodiscard]] auto value(const std::shared_ptr<markup_language>& language) const -> std::string override
        {
            return language->anchor(m_body, m_href, m_wants_extension);
        }

    private:
        std::shared_ptr<markup_node> m_body;
        std::string m_href;
        bool m_wants_extension;
    };
}