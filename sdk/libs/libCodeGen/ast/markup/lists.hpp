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
#include <libCodeGen/ast/markup/markup_node.hpp>
#include <libCodeGen/ast/markup/text.hpp>

namespace codegen::ast
{
    template<language::markup_support L>
    struct abstract_list : markup_node<L>
    {

    };

    template<language::markup_support L>
    struct abstract_list_item : markup_node<L>
    {
        virtual auto add_content(const std::shared_ptr<markup_node<L>>& node) -> void = 0;
        virtual auto add_sublist() -> std::shared_ptr<abstract_list<L>> = 0;
    };

    template<language::markup_support L>
    struct list : public abstract_list<L>
    {
        explicit list(bool sublist = false)
            : m_sublist(sublist)
        {}

        template<typename T, typename std::enable_if<std::is_base_of<abstract_list_item<L>, T>::value>::type* = nullptr>
        auto add_item(const std::string& item) -> std::shared_ptr<T>
        {
            return add_item<T>(std::make_shared<ast::text<L>>(item));
        }

        template<typename T, typename std::enable_if<std::is_base_of<abstract_list_item<L>, T>::value>::type* = nullptr>
        auto add_item(const std::shared_ptr<markup_node<L>>& item) -> std::shared_ptr<T>
        {
            auto item_node = std::make_shared<T>();
            item_node->add_content(item);
            m_items.emplace_back(item_node);
            return item_node;
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> segments;

            if (m_sublist) {
                segments.emplace_back(L::begin_list(markup_node<L>::style_classes()));
                for (const auto& item : m_items) {
                    segments.emplace_back(item->emit());
                }
                segments.emplace_back(L::end_list());
            }
            else {
                segments.emplace_back(L::begin_sublist(markup_node<L>::style_classes()));
                for (const auto& item : m_items) {
                    segments.emplace_back(item->emit());
                }
                segments.emplace_back(L::end_sublist());
            }

            return emit::segment(segments);
        }

    private:
        bool m_sublist { false };
        std::vector<std::shared_ptr<abstract_list_item<L>>> m_items;
    };

    template<language::markup_support L>
    struct list_item : public abstract_list_item<L>
    {
        auto add_content(const std::shared_ptr<markup_node<L>>& node) -> void override
        {
            m_content.emplace_back(node);
        }

        auto add_sublist() -> std::shared_ptr<abstract_list<L>> override
        {
            auto list = std::make_shared<ast::list<L>>(true);
            m_content.emplace_back(list);
            return list;
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> segments;
            segments.emplace_back(L::begin_list_item(markup_node<L>::style_classes()));
            for (const auto& node : m_content) {
                segments.emplace_back(node->emit());
            }
            segments.emplace_back(L::end_list_item());
            return emit::segment(segments);
        }

    private:
        std::vector<std::shared_ptr<markup_node<L>>> m_content;
    };

}