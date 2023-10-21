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
    struct abstract_table_cell : public markup_node<L>
    {
        virtual auto add_content(const std::string& text) -> void = 0;
        virtual auto add_content(const std::shared_ptr<markup_node<L>>& node) -> void = 0;
    };

    template<language::markup_support L>
    struct abstract_table_header_cell : public abstract_table_cell<L>
    {};

    template<language::markup_support L, typename T, typename std::enable_if<std::is_base_of<abstract_table_cell<L>, T>::value>::type* = nullptr>
    struct abstract_table_row : public markup_node<L>
    {
        virtual auto add_cell() -> std::shared_ptr<T> = 0;
    };

    template<language::markup_support L>
    struct abstract_table : public markup_node<L>
    {
        virtual auto header_row() -> std::shared_ptr<abstract_table_row<L, abstract_table_header_cell<L>>> = 0;
        virtual auto add_row() -> std::shared_ptr<abstract_table_row<L, abstract_table_cell<L>>> = 0;
    };


    template<language::markup_support L>
    struct table_cell : public abstract_table_cell<L>
    {
        auto add_content(const std::shared_ptr<markup_node<L>>& node) -> void override
        {
            m_content.emplace_back(node);
        }

        auto add_content(const std::string& text) -> void override
        {
            add_content(std::make_shared<ast::text<L>>(text));
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> segments;
            segments.emplace_back(L::begin_table_cell(markup_node<L>::style_classes()));
            for (const auto& node : m_content) {
                segments.emplace_back(node->emit());
            }
            segments.emplace_back(L::end_table_cell());
            return emit::segment(segments);
        }

    private:
        std::vector<std::shared_ptr<markup_node<L>>> m_content;
    };

    template<language::markup_support L>
    struct table_header_cell : public abstract_table_header_cell<L>
    {
        auto add_content(const std::shared_ptr<markup_node<L>>& node) -> void override
        {
            m_content.emplace_back(node);
        }

        auto add_content(const std::string& text) -> void override
        {
            add_content(std::make_shared<ast::text<L>>(text));
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> segments;
            segments.emplace_back(L::begin_table_header_cell(markup_node<L>::style_classes()));
            for (const auto& node : m_content) {
                segments.emplace_back(node->emit());
            }
            segments.emplace_back(L::end_table_header_cell());
            return emit::segment(segments);
        }

    private:
        std::vector<std::shared_ptr<markup_node<L>>> m_content;
    };

    template<language::markup_support L, typename T, typename std::enable_if<std::is_base_of<abstract_table_cell<L>, T>::value>::type* = nullptr>
    struct table_row : public abstract_table_row<L, T>
    {
        auto add_cell() -> std::shared_ptr<T> override
        {
            auto cell = std::make_shared<T>();
            m_cells.emplace_back(cell);
            return cell;
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> segments;
            segments.emplace_back(L::begin_table_row(markup_node<L>::style_classes()));
            for (const auto& cell : m_cells) {
                segments.emplace_back(cell->emit());
            }
            segments.emplace_back(L::end_table_row());
            return emit::segment(segments);
        }

    private:
        std::vector<std::shared_ptr<T>> m_cells;
    };

    template<language::markup_support L>
    struct table : public abstract_table<L>
    {
        auto header_row() -> std::shared_ptr<abstract_table_row<L, abstract_table_header_cell<L>>> override
        {
            if (!m_header_row) {
                m_header_row = std::make_shared<table_row<L, table_header_cell<L>>>();
            }
            return std::reinterpret_pointer_cast<abstract_table_row<L, abstract_table_header_cell<L>>>(m_header_row);
        }

        auto add_row() -> std::shared_ptr<abstract_table_row<L, abstract_table_cell<L>>> override
        {
            auto row = std::make_shared<table_row<L, table_cell<L>>>();
            m_rows.emplace_back(row);
            return std::reinterpret_pointer_cast<abstract_table_row<L, abstract_table_cell<L>>>(row);
        }

        auto add_row(const std::initializer_list<std::string>& cells) -> void
        {
            const auto& row = add_row();
            for (const auto& cell : cells) {
                row->add_cell()->add_content(cell);
            }
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> segments;
            segments.emplace_back(L::begin_table(markup_node<L>::style_classes()));
            if (m_header_row) {
                segments.emplace_back(m_header_row->emit());
            }
            for (const auto& row : m_rows) {
                segments.emplace_back(row->emit());
            }
            segments.emplace_back(L::end_table());
            return emit::segment(segments);
        }

    private:
        std::shared_ptr<table_row<L, table_header_cell<L>>> m_header_row;
        std::vector<std::shared_ptr<table_row<L, table_cell<L>>>> m_rows;
    };

}