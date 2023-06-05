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

#include <libCodeGen/languages/markup/markup.hpp>
#include <libCodeGen/spec/markup/node.hpp>

namespace codegen::spec::markup
{
    struct text : public markup_node
    {
        enum class style { normal, bold, italic, strikethrough, code };

        explicit text(const std::string& body, enum style style = style::normal)
        {
            append(body, style);
        }

        auto append(const std::string& body, enum style style = style::normal) -> void
        {
            m_body.emplace_back(style, body);
        }

        [[nodiscard]] auto value(const std::shared_ptr<markup_language>& language) const -> std::string override
        {
            std::string out;
            for (const auto& chunk : m_body) {
                switch (chunk.style) {
                    case style::normal: {
                        out += language->plain(chunk.text);
                        break;
                    }
                    case style::bold: {
                        out += language->bold(chunk.text);
                        break;
                    }
                    case style::italic: {
                        out += language->italic(chunk.text);
                        break;
                    }
                    case style::strikethrough: {
                        out += language->strikethrough(chunk.text);
                        break;
                    }
                    case style::code: {
                        out += language->inline_code(chunk.text);
                        break;
                    }
                }
            }
            return std::move(out);
        }

    private:
        struct chunk
        {
            enum style style;
            std::string text;

            chunk(enum style style, const std::string& text) : style(style), text(text) {}
        };

        std::vector<chunk> m_body;
    };
}