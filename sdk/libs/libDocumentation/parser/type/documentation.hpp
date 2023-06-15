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
#include <libDocumentation/parser/type/section.hpp>
#include <libDocumentation/parser/type/parameter.hpp>

namespace documentation
{
    struct object
    {
        object() = default;

        [[nodiscard]] auto sections() const -> const std::vector<struct section>& { return m_sections; }
        [[nodiscard]] auto parameters() const -> const std::vector<struct parameter>& { return m_parameters; }

        auto add(const struct section& section) -> void
        {
            m_sections.emplace_back(section);
        }

        auto add(const struct parameter& parameter) -> void
        {
            m_parameters.emplace_back(parameter);
        }

    private:
        std::vector<struct section> m_sections;
        std::vector<struct parameter> m_parameters;
    };
}