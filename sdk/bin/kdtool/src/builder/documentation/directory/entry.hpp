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

#include <map>
#include <string>
#include <optional>
#include <functional>
#include <libFoundation/system/filesystem/path.hpp>
#include "project/structure/symbol.hpp"

namespace kdtool::builder::directory
{
    struct entry
    {
        explicit entry(const std::shared_ptr<project::structure::symbol>& symbol)
            : m_symbol(symbol), m_path(m_symbol->filename())
        {}

        [[nodiscard]] auto name() const -> std::string { return m_symbol->display_name(); }
        [[nodiscard]] auto symbol() const -> std::shared_ptr<project::structure::symbol> { return m_symbol; }
        [[nodiscard]] auto has_file() const -> bool { return m_path.has_value(); }
        [[nodiscard]] auto is_leaf() const -> bool { return m_symbol->children().empty(); }
        [[nodiscard]] auto is_root() const -> bool { return m_symbol->parent().lock() == nullptr; }

        [[nodiscard]] auto path() const -> foundation::filesystem::path
        {
            if (auto definition = symbol()->definition().lock()) {
                switch (definition->instance_type()) {
                    case project::structure::construct_definition::type::is_namespace:
                    case project::structure::construct_definition::type::is_class:
                    case project::structure::construct_definition::type::is_enum:
                        return m_path->child("index");
                    default:
                        break;
                }
            }
            return m_path.value();
        }

        auto each(const std::function<auto(const std::shared_ptr<project::structure::symbol>&)->void>& callback) -> void
        {
            for (const auto& it : m_symbol->children()) {
                if (auto child = it.lock()) {
                    callback(child);
                }
            }
        }

    private:
        std::shared_ptr<project::structure::symbol> m_symbol;
        std::optional<foundation::filesystem::path> m_path;
    };
}