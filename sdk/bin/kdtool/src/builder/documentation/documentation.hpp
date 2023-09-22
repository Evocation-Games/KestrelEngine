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

#include <memory>
#include <libCodeGen/builder/builder.hpp>
#include <libCodeGen/ast/markup.hpp>
#include "builder/builder.hpp"
#include "builder/documentation/directory/entry.hpp"
#include "builder/documentation/components/directory.hpp"

namespace kdtool::builder
{
    template<codegen::language::markup_support L>
    struct documentation : public std::enable_shared_from_this<documentation<L>>, public codegen::builder<L>, public base
    {
        documentation(const std::shared_ptr<project::index>& project, const foundation::filesystem::path& root, const foundation::filesystem::path& reference_root = {})
            : base(project), codegen::builder<L>(root.child(L::lc_name()), reference_root)
        {}

        auto build() -> void override
        {
            codegen::builder<L>::build();

            codegen::builder<L>::template add<codegen::ast::prologue<L>>(
                project()->title(),
                codegen::builder<L>::reference_root().child("style.css").string()
            );
            codegen::builder<L>::template add<codegen::ast::heading<L>>(project()->title(), 1);

            std::vector<std::shared_ptr<directory::entry>> root_entries;
            for (const auto& symbol : project()->all_root_symbols()) {
                if (symbol->is_builtin()) {
                    // Skip build in
                    continue;
                }
                root_entries.emplace_back(std::make_shared<directory::entry>(symbol));
            }

            auto dir = std::make_shared<component::directory<L>>(root_entries, codegen::builder<L>::root().string(), codegen::builder<L>::reference_root().string(), true);
            codegen::builder<L>::add_component(dir);

            codegen::builder<L>::template add<codegen::ast::epilogue<L>>();
            codegen::builder<L>::save("index");
        }

    private:
    };
}