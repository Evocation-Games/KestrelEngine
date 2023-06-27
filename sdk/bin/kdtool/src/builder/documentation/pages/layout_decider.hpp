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
#include "builder/documentation/pages/basic_page.hpp"
#include "builder/documentation/pages/namespace_page.hpp"
#include "builder/documentation/pages/class_page.hpp"
#include "builder/documentation/pages/function_page.hpp"
#include "builder/documentation/pages/enum_page.hpp"
#include "builder/documentation/pages/enum_case_page.hpp"
#include "builder/documentation/pages/resource_type_page.hpp"
#include "builder/documentation/pages/resource_field_page.hpp"
#include "builder/documentation/pages/resource_value_page.hpp"
#include "builder/documentation/pages/resource_value_symbol_page.hpp"

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct layout_decider
    {
        static auto using_definition(
            const std::weak_ptr<project::structure::construct_definition>& weak_definition,
            const std::string& root_dir,
            const std::string& reference_root
        ) -> void {
            auto definition = weak_definition.lock();
            if (!definition) {
                // There is no definition to synthesize a page from.
                return;
            }

            switch (definition->instance_type()) {
                case project::structure::construct_definition::type::is_namespace: {
                    auto ns_page = page::namespace_page<L>(definition, root_dir, reference_root);
                    ns_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    ns_page.build();
                    break;
                }
                case project::structure::construct_definition::type::is_class: {
                    auto class_page = page::class_page<L>(definition, root_dir, reference_root);
                    class_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    class_page.build();
                    break;
                }
                case project::structure::construct_definition::type::is_enum: {
                    auto enum_page = page::enum_page<L>(definition, root_dir, reference_root);
                    enum_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    enum_page.build();
                    break;
                }
                case project::structure::construct_definition::type::is_enum_case: {
                    page::enum_case_page<L>(definition, root_dir, reference_root).build();
                    break;
                }
                case project::structure::construct_definition::type::is_property: {
                    page::basic<L>(definition, root_dir, reference_root).build();
                    break;
                }
                case project::structure::construct_definition::type::is_function: {
                    page::function_page<L>(definition, root_dir, reference_root).build();
                    break;
                }
                case project::structure::construct_definition::type::is_constructor: {
                    page::basic<L>(definition, root_dir, reference_root).build();
                    break;
                }

                case project::structure::construct_definition::type::is_resource_type: {
                    auto resource_type_page = page::resource_type_page<L>(definition, root_dir, reference_root);
                    resource_type_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    resource_type_page.build();
                    break;
                }
                case project::structure::construct_definition::type::is_resource_field: {
                    auto resource_field_page = page::resource_field_page<L>(definition, root_dir, reference_root);
                    resource_field_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    resource_field_page.build();
                    break;
                }
                case project::structure::construct_definition::type::is_resource_value: {
                    auto resource_value_page = page::resource_value_page<L>(definition, root_dir, reference_root);
                    resource_value_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    resource_value_page.build();
                    break;
                }
                case project::structure::construct_definition::type::is_resource_value_symbol: {
                    auto resource_value_symbol_page = page::resource_value_symbol_page<L>(definition, root_dir, reference_root);
                    resource_value_symbol_page.handle_layout_decision([&] (const auto& construct) {
                        using_definition(construct, root_dir, reference_root);
                    });
                    resource_value_symbol_page.build();
                    break;
                }

                default: break;
            }
        }
    };
}