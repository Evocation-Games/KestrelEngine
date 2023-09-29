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
#include "project/structure/construct_definition.hpp"
#include "project/structure/function/parameter/parameter_definition.hpp"

namespace kdtool::project::structure
{
    struct constructor_definition : public construct_definition
    {
        constructor_definition()
            : construct_definition(nullptr)
        {}

        static auto type() -> enum type
        {
            return type::is_constructor;
        }

        [[nodiscard]] auto instance_type() const -> enum type override
        {
            return type();
        }

        auto add(const std::shared_ptr<struct parameter_definition>& parameter) -> void;
        [[nodiscard]] auto parameter_type_names() const -> std::vector<std::string>;

    private:
        std::vector<std::shared_ptr<struct parameter_definition>> m_parameters;
    };
}