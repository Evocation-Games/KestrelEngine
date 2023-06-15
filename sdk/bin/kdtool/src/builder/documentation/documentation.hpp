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
        documentation(const std::shared_ptr<project::index>& project, const std::string& root)
            : base(project), codegen::builder<L>(root)
        {}

        auto build() -> void override
        {
            codegen::builder<L>::build();

            codegen::builder<L>::template add<codegen::ast::heading<L>>("Hello, World", 1);

            auto root = std::make_shared<directory::entry>("Root Element");
            root->add(std::make_shared<directory::entry>("Foo"));
            root->add(std::make_shared<directory::entry>("Bar"));
            root->add(std::make_shared<directory::entry>("Baz"));
            root->add(std::make_shared<directory::entry>("Fubar"));

            auto dir = std::make_shared<component::directory<L>>(root);

            codegen::builder<L>::template add<codegen::ast::begin_list<L>>();
            codegen::builder<L>::add_component(dir);
            codegen::builder<L>::template add<codegen::ast::end_list<L>>();

            codegen::builder<L>::save("test");
        }

    private:

    };
}