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

#include <libKDL/sema/context.hpp>

// MARK: - Types

auto kdl::sema::context::register_type(const resource::definition::type::instance& type) -> const resource::definition::type::instance *
{
    auto it = registered_types.find(type.name());
    if (it == registered_types.end()) {
        registered_types.emplace(type.name(), type);
    }

    it = registered_types.find(type.name());
    if (it == registered_types.end()) {
        throw std::runtime_error("");
    }
    return &it->second;
}

auto kdl::sema::context::type_named(const std::string& name) const -> const resource::definition::type::instance *
{
    auto it = registered_types.find(name);
    if (it == registered_types.end()) {
        throw std::runtime_error("");
    }
    return &it->second;
}

// MARK: -

auto kdl::sema::context::create_scope() -> interpreter::scope *
{
    if (scope_stack.empty()) {
        scope_stack.emplace_back();
    }
    else {
        scope_stack.emplace_back(active_scope()->subscope());
    }
    return &scope_stack.back();
}

auto kdl::sema::context::pop_scope() -> void
{
    scope_stack.pop_back();
}

auto kdl::sema::context::active_scope() -> interpreter::scope *
{
    return &scope_stack.back();
}
