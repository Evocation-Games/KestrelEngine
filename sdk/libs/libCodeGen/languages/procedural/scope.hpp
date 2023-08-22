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
#include <string>
#include <unordered_map>
#include <libCodeGen/ast/core/node.hpp>
#include <libCodeGen/languages/procedural/symbol.hpp>
#include <libFoundation/string/split.hpp>

namespace codegen::language::procedural
{
    template<general_procedural_support L>
    struct scope : public std::enable_shared_from_this<scope<L>>
    {
        scope() = default;
        explicit scope(const std::shared_ptr<scope<L>>& parent)
            : m_parent(parent)
        {}

        [[nodiscard]] auto subscope() -> std::shared_ptr<scope<L>>
        {
            return std::make_shared<scope<L>>(this->shared_from_this());
        }

        // MARK: - Symbol Management

        struct symbol_ref {
            std::shared_ptr<symbol<L>> symbol;
            std::shared_ptr<scope<L>> child;
        };

        [[nodiscard]] auto symbol(
            const std::string& name
        ) -> std::shared_ptr<struct procedural::symbol<L>> {
            auto name_components = foundation::string::split(name, L::scope_resolution_operator_string());
            return symbol(name_components);
        }

        [[nodiscard]] auto symbol(
            const std::vector<std::string>& name,
            const std::vector<std::string>& resolved = {}
        ) -> std::shared_ptr<struct procedural::symbol<L>> {
            // Get the first name in the vector, and remove it.
            auto components = name;
            auto current_name = components.front();
            components.erase(components.begin());

            auto resolved_name = resolved;
            resolved_name.emplace_back(current_name);

            auto it = m_symbols.find(current_name);
            if (it == m_symbols.end()) {
                // Create the symbol, and its scope.
                struct symbol_ref ref;
                ref.symbol = std::make_shared<struct procedural::symbol<L>>(resolved_name);
                ref.child = subscope();
                m_symbols.emplace(current_name, ref);

                return components.empty() ? ref.symbol : ref.child->symbol(components, resolved_name);
            }
            else {
                return components.empty() ? it->second.symbol : it->second.child->symbol(components, resolved_name);
            }
        }

    private:
        std::weak_ptr<scope<L>> m_parent;
        std::unordered_map<std::string, struct symbol_ref> m_symbols;
    };
}