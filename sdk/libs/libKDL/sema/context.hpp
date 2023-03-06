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

#include <unordered_map>
#include <vector>
#include <libKDL/sema/decorator/decorator.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/declaration/instance.hpp>
#include <libInterpreter/scope/scope.hpp>

namespace kdl::sema
{
    struct context
    {
        std::unordered_map<std::string, resource::definition::type::instance> registered_types;
        std::vector<resource::instance> resources;
        decorator::collection current_decorators;

        std::vector<interpreter::scope *> scope_stack;
        const resource::definition::type::descriptor *current_type_descriptor { nullptr };
        const resource::definition::type::instance *current_type { nullptr };
        const resource::definition::binary_template::field *current_binary_field { nullptr };

        auto register_type(const resource::definition::type::instance& type) -> const resource::definition::type::instance *;
        [[nodiscard]] auto type_named(const std::string& name) const -> const resource::definition::type::instance *;
        [[nodiscard]] auto type(const resource::definition::type::descriptor& descriptor) const -> const resource::definition::type::instance *;

        auto create_scope() -> interpreter::scope *;
        auto pop_scope() -> void;
        auto active_scope() -> interpreter::scope *;
    };
}