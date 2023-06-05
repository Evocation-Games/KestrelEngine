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

#include <vector>
#include <string>
#include <memory>
#include <concepts>
#include <type_traits>
#include <libCodeGen/common/node.hpp>
#include <libCodeGen/languages/markup/markup.hpp>

namespace codegen::spec::markup
{
    struct markup_node : public node
    {
    public:
        [[nodiscard]] virtual auto value(const std::shared_ptr<markup_language>& language) const -> std::string
        {
            return "";
        }

        [[nodiscard]] virtual auto emit(const std::shared_ptr<markup_language>& language) const -> std::vector<std::string>
        {
            std::vector<std::string> out;

            std::string value_out = value(language);
            if (!value_out.empty()) {
                out.emplace_back(value_out);
            }

            if (!m_children.empty()) {
                for (const auto& it : m_children) {
                    auto lines = it->emit(language);
                    out.insert(out.end(), lines.begin(), lines.end());
                }
            }
            return std::move(out);
        }

        template<typename T, typename std::enable_if<std::is_base_of<markup_node, T>::value>::type* = nullptr>
        auto add(const std::shared_ptr<T>& n) -> std::shared_ptr<T>
        {
            if (n) {
                m_children.emplace_back(n);
            }
            return n;
        }

        template<typename T, typename ...Args, typename std::enable_if<std::is_base_of<markup_node, T>::value>::type* = nullptr>
        auto add(Args&& ...args) -> std::shared_ptr<T>
        {
            return add(std::make_shared<T>(std::forward<Args>(args)...));
        }

    private:
        std::vector<std::shared_ptr<markup_node>> m_children;
    };
}