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
#include <memory>
#include <string>
#include <unordered_map>
#include "project/structure/construct_definition.hpp"
#include "project/structure/class/constructor/constructor_definition.hpp"
#include "project/structure/class/property/property_definition.hpp"
#include "project/structure/function/function_definition.hpp"
#include "project/structure/enrollment/enrollment.hpp"

namespace kdtool::project::structure
{
    struct class_definition : public construct_definition
    {
        explicit class_definition(const std::shared_ptr<struct symbol>& symbol)
            : construct_definition(symbol)
        {}

        static auto type() -> enum type
        {
            return type::is_class;
        }

        [[nodiscard]] auto instance_type() const -> enum type override
        {
            return type();
        }

        auto set(const std::shared_ptr<struct enrollment>& enrollment) -> void override;
        auto set(const std::shared_ptr<struct constructor_definition>& constructor) -> void;
        auto add(const std::shared_ptr<struct function_definition>& function) -> void;

        [[nodiscard]] auto property(const std::shared_ptr<struct symbol>& name) -> std::shared_ptr<struct property_definition>;
        [[nodiscard]] auto constructor() const -> std::shared_ptr<struct constructor_definition>;
        [[nodiscard]] auto all_properties() const -> std::vector<std::shared_ptr<struct property_definition>>;
        [[nodiscard]] auto all_functions() const -> std::vector<std::shared_ptr<struct function_definition>>;

    private:
        std::shared_ptr<struct constructor_definition> m_constructor;
        std::vector<std::shared_ptr<struct function_definition>> m_functions;
        std::unordered_map<std::string, std::shared_ptr<struct property_definition>> m_properties;
    };
}