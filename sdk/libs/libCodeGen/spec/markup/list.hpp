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
#include <libCodeGen/spec/markup/text.hpp>
#include <libCodeGen/spec/markup/list_item.hpp>

namespace codegen::spec::markup
{
    struct list : public markup_node
    {
        auto add_item(const std::string& item) -> void
        {
            add_item(std::make_shared<text>(item));
        }

        auto add_item(const std::shared_ptr<markup_node>& item) -> void
        {
            m_items->add(std::make_shared<list_item>(item));
        }

        [[nodiscard]] auto emit(const std::shared_ptr<markup_language>& language) const -> std::vector<std::string> override
        {
            return std::move(language->list(m_items));
        }

    private:
        std::shared_ptr<markup_node> m_items { std::make_shared<markup_node>() };
    };
}