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
#include "project/structure/construct_definition.hpp"
#include <libResource/definition/type/symbol.hpp>

namespace kdtool::project::structure
{
    struct resource_value_symbol_definition : public construct_definition
    {
        explicit resource_value_symbol_definition(const std::shared_ptr<struct symbol>& symbol)
            : construct_definition(symbol)
        {}

        static auto type() -> enum type
        {
            return type::is_resource_value_symbol;
        }

        [[nodiscard]] auto instance_type() const -> enum type override
        {
            return type();
        }

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto basename() const -> std::string;
        [[nodiscard]] auto value() const -> std::string;

        auto set_instance(const ::resource::definition::type::symbol& instance) -> void;
        [[nodiscard]] auto instance() const -> const ::resource::definition::type::symbol&;

    private:
        ::resource::definition::type::symbol m_instance { "", {} };
    };
}