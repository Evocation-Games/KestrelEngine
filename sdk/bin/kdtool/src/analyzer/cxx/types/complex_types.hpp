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

#include "project/structure/type/type.hpp"

namespace kdtool::cxx::type
{
    struct basic : project::structure::type::base
    {
        explicit basic(const std::string& name) : m_name(name) {}
        [[nodiscard]] auto name() const -> std::string override { return m_name; }
    private:
        std::string m_name;
    };

    struct template_specialised : basic
    {
        explicit template_specialised(const std::string& name, const std::vector<std::shared_ptr<struct project::structure::type::base>>& specialisations)
            : basic(name), m_template_specialisations(specialisations)
        {}

        [[nodiscard]] auto name() const -> std::string override
        {
            std::vector<std::string> out;
            for (const auto& s : m_template_specialisations) {
                out.emplace_back(s->name());
            }
            return basic::name() + "<" + foundation::string::joined(out, ", ") + ">";
        }
    private:
        std::vector<std::shared_ptr<struct project::structure::type::base>> m_template_specialisations;
    };

    struct const_qualified : project::structure::type::base
    {
        explicit const_qualified(const std::shared_ptr<struct project::structure::type::base>& type) : m_type(type) {}
        [[nodiscard]] auto name() const -> std::string override { return "const " + m_type->name(); }
    private:
        std::shared_ptr<struct project::structure::type::base> m_type;
    };

    struct reference : project::structure::type::base
    {
        explicit reference(const std::shared_ptr<struct project::structure::type::base>& type) : m_type(type) {}
        [[nodiscard]] auto name() const -> std::string override { return m_type->name() + "&"; }
    private:
        std::shared_ptr<struct project::structure::type::base> m_type;
    };

    struct pointer : project::structure::type::base
    {
        explicit pointer(const std::shared_ptr<struct project::structure::type::base>& type) : m_type(type) {}
        [[nodiscard]] auto name() const -> std::string override { return m_type->name() + "*"; }
    private:
        std::shared_ptr<struct project::structure::type::base> m_type;
    };
}