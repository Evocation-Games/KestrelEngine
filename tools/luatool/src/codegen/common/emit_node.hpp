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

#pragma once

#include <vector>
#include <string>
#include <memory>

namespace luatool::codegen::common
{
    class emit_node
    {
    public:
        [[nodiscard]] virtual auto value() const -> std::string
        {
            return "";
        }

        [[nodiscard]] virtual auto emit() const -> std::vector<std::string>
        {
            std::vector<std::string> out;
            out.emplace_back(value());
            if (!m_children.empty()) {
                    for (auto it : m_children) {
                        auto lines = it->emit();
                        out.insert(out.end(), lines.begin(), lines.end());
                    }
            }
            return std::move(out);
        }

        auto add_child(std::shared_ptr<emit_node> node) -> std::shared_ptr<emit_node>
        {
            m_children.emplace_back(node);
            return node;
        }

        template<typename T, typename ...Args, typename std::enable_if<std::is_base_of<emit_node, T>::value>::type* = nullptr>
        auto add_child(Args&& ...args) -> std::shared_ptr<T>
        {
            auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
            m_children.template emplace_back(ptr);
            return ptr;
        }

    private:
        std::vector<std::shared_ptr<emit_node>> m_children;

    };
}