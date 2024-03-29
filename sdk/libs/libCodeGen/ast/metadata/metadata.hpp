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
#include <sstream>
#include <vector>
#include <libCodeGen/ast/core/node.hpp>

namespace codegen::ast
{
    template<language::definition_metadata L>
    struct project_name : public node
    {
        explicit project_name(const std::string& name)
            : m_name(name)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return { std::move(L::metadata("project", m_name)) };
        }

    private:
        std::string m_name;
    };

    template<language::definition_metadata L>
    struct author_name : public node
    {
        explicit author_name(const std::string& name)
            : m_name(name)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return { std::move(L::metadata("author", m_name)) };
        }

    private:
        std::string m_name;
    };

    template<language::definition_metadata L>
    struct version_name : public node
    {
        explicit version_name(const std::string& name)
            : m_name(name)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return { std::move(L::metadata("version", m_name)) };
        }

    private:
        std::string m_name;
    };

    template<language::definition_metadata L>
    struct license_name : public node
    {
        explicit license_name(const std::string& name)
            : m_name(name)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return { std::move(L::metadata("license", m_name)) };
        }

    private:
        std::string m_name;
    };

    template<language::definition_metadata L>
    struct copyright_name : public node
    {
        explicit copyright_name(const std::string& name)
            : m_name(name)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return { std::move(L::metadata("copyright", m_name)) };
        }

    private:
        std::string m_name;
    };
}