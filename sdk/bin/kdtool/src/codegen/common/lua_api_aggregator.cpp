// Copyright (c) 2022 Tom Hancocks
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

#include "codegen/common/lua_api_aggregator.hpp"

// MARK: - Merging

auto kdtool::codegen::common::api::aggregator::merge_namespace(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> void
{
    m_namespaces.emplace(std::pair(ns->hash_key(), ns));
}

auto kdtool::codegen::common::api::aggregator::merge_class(std::shared_ptr<lua_api::ast::lua_class> klass) -> void
{
    m_classes.emplace(std::pair(klass->hash_key(), klass));
}

auto kdtool::codegen::common::api::aggregator::merge_enum(std::shared_ptr<lua_api::ast::lua_enum> lua_enum) -> void
{
    m_enums.emplace(std::pair(lua_enum->hash_key(), lua_enum));
}

// MARK: - Access

auto kdtool::codegen::common::api::aggregator::all_namespaces() const -> std::vector<std::shared_ptr<lua_api::ast::lua_namespace>>
{
    std::vector<std::shared_ptr<lua_api::ast::lua_namespace>> namespaces;
    for (const auto& it : m_namespaces) {
        namespaces.emplace_back(it.second);
    }
    return namespaces;
}

auto kdtool::codegen::common::api::aggregator::all_classes() const -> std::vector<std::shared_ptr<lua_api::ast::lua_class>>
{
    std::vector<std::shared_ptr<lua_api::ast::lua_class>> classes;
    for (const auto& it : m_classes) {
        classes.emplace_back(it.second);
    }
    return classes;
}

auto kdtool::codegen::common::api::aggregator::all_enums() const -> std::vector<std::shared_ptr<lua_api::ast::lua_enum>>
{
    std::vector<std::shared_ptr<lua_api::ast::lua_enum>> enums;
    for (const auto& it : m_enums) {
        enums.emplace_back(it.second);
    }
    return enums;
}