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
#include <vector>
#include <type_traits>
#include <libCodeGen/languages/language.hpp>
#include <libFoundation/system/filesystem/path.hpp>
#include <libCodeGen/builder/emit/segment.hpp>
#include <libCodeGen/builder/emit/buffer.hpp>
#include <libCodeGen/builder/component.hpp>
#include <libCodeGen/languages/procedural/scope.hpp>

namespace codegen
{
    template<language::metadata L>
    struct builder
    {
        explicit builder(const std::string& path)
            : m_root(path)
        {}

        explicit builder(const foundation::filesystem::path& root)
            : m_root(root)
        {}

        [[nodiscard]] virtual auto root() const -> foundation::filesystem::path { return m_root; }

        virtual auto build() -> void {};

        auto save(const std::string& filename) const -> void
        {
            auto buffer = std::make_shared<emit::buffer>();
            buffer->write(m_segments);
            buffer->save(m_root.child(filename + "." + L::extension()));
        }

        template<typename T, typename std::enable_if<std::is_base_of<ast::node, T>::value>::type* = nullptr>
        auto add(const std::shared_ptr<T>& node) -> void
        {
            m_segments.emplace_back(node->emit());
        }

        template<typename T, typename ...Args, typename std::enable_if<std::is_base_of<ast::node, T>::value>::type* = nullptr>
        auto add(Args&& ...args) -> void
        {
            return add(std::make_shared<T>(std::forward<Args>(args)...));
        }

        template<typename T, typename ...Args, typename std::enable_if<std::is_base_of<component, T>::value>::type* = nullptr>
        auto add_component(Args&& ...args) -> void
        {
            return add_component(std::make_shared<T>(std::forward<Args>(args)...));
        }

        auto add_component(const std::shared_ptr<component>& component) -> void
        {
            add(component->emit());
        }

        auto add(const emit::segment& segment) -> void
        {
            m_segments.emplace_back(segment);
        }

        auto add(const std::vector<emit::segment>& segments) -> void
        {
            for (const auto& segment : segments) {
                add(segment);
            }
        }

    private:
        foundation::filesystem::path m_root;
        std::vector<emit::segment> m_segments;
    };

    // MARK: - Procedural Specific Builder

    template<language::general_procedural_support L>
    struct procedural_builder : public builder<L>
    {
        explicit procedural_builder(const std::string& path)
            : builder<L>(path)
        {}

        explicit procedural_builder(const foundation::filesystem::path& path)
            : builder<L>(path)
        {}

        [[nodiscard]] virtual auto scope() const -> std::shared_ptr<language::procedural::scope<L>> { return m_scope; }

    private:
        std::shared_ptr<language::procedural::scope<L>> m_scope { std::make_shared<language::procedural::scope<L>>() };
    };
}