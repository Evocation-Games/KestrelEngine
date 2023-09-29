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

#include <string>
#include <libFoundation/system/filesystem/path.hpp>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/builder/component.hpp>

namespace kdtool::builder
{
    template<codegen::language::metadata L>
    struct buffer
    {
        [[nodiscard]] auto segments() const -> codegen::emit::segment
        {
            return std::move(codegen::emit::segment(m_segments));
        }

        template<typename T, typename std::enable_if<std::is_base_of<codegen::ast::node, T>::value>::type* = nullptr>
        auto add(const std::shared_ptr<T>& n) -> void
        {
            m_segments.emplace_back(n->emit());
        }

        template<typename T, typename ...Args, typename std::enable_if<std::is_base_of<codegen::ast::node, T>::value>::type* = nullptr>
        auto add(Args&& ...args) -> void
        {
            return add(std::make_shared<T>(std::forward<Args>(args)...));
        }

        auto add(const std::shared_ptr<codegen::component>& component) -> void
        {
            m_segments.emplace_back(component->emit());
        }

    private:
        std::vector<codegen::emit::segment> m_segments;
    };
}