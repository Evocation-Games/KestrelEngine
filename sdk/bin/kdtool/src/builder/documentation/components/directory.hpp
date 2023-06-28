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

#include <libCodeGen/builder/component.hpp>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/ast/markup.hpp>
#include "builder/buffer.hpp"
#include "builder/documentation/directory/entry.hpp"
#include "builder/documentation/pages/layout_decider.hpp"

namespace kdtool::builder::component
{
    template<codegen::language::metadata L>
    struct directory : public codegen::component
    {
        explicit directory(const std::shared_ptr<struct builder::directory::entry>& entry, const std::string& root_dir, const std::string& reference_root, bool is_root = false)
            : m_entries({ entry }), m_root(is_root), m_root_dir(root_dir), m_reference_root(reference_root)
        {}

        explicit directory(const std::vector<std::shared_ptr<struct builder::directory::entry>>& entry, const std::string& root_dir, const std::string& reference_root, bool is_root = false)
            : m_entries(entry), m_root(is_root), m_root_dir(root_dir), m_reference_root(reference_root)
        {}

        [[nodiscard]] auto emit() const -> codegen::emit::segment override
        {
            auto list = std::make_shared<codegen::ast::list<L>>();
            for (const auto& entry : m_entries) {
                if (entry->is_leaf() && !entry->is_root()) {
                    continue;
                }

                if (entry->has_file() && entry->symbol()->definition().lock()) {
                    auto path = entry->path().string();
                    if (path.starts_with("/")) {
                        path.erase(0, 1);
                    }

                    list->template add_item<codegen::ast::list_item<L>>(
                        std::make_shared<codegen::ast::anchor<L>>(
                            entry->name(),
                            path + "." + L::extension()
                        )
                    )->add_style_class(entry->style_class());

                    // Synthesize the page for this entry.
                    page::layout_decider<L>::using_definition(entry->symbol()->definition(), m_root_dir, m_reference_root);
                }
                else {
                    list->template add_item<codegen::ast::list_item<L>>(entry->name())
                        ->add_style_class(entry->style_class());
                }

                // TODO: Fix this...
//                if (!entry->is_leaf()) {
//                    decltype(m_entries) children;
//                    entry->each([&] (const auto& child) {
//                        children.emplace_back(std::make_shared<builder::directory::entry>(child));
//                    });
//                    buffer.add(std::make_shared<directory<L>>(children, m_root_dir, m_reference_root));
//                }
            }
            return list->emit();
        }

    private:
        bool m_root { false };
        std::string m_root_dir;
        std::string m_reference_root;
        std::vector<std::shared_ptr<struct builder::directory::entry>> m_entries;
    };
}