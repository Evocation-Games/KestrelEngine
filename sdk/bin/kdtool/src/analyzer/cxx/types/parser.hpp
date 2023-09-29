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
#include <libLexer/lexer.hpp>
#include "project/structure/type/type.hpp"

namespace kdtool::cxx
{
    struct type_parser
    {
        explicit type_parser(const std::string& type_string);

        static auto type_for(enum project::structure::type::scalar scalar) -> std::shared_ptr<struct project::structure::type::base>;
        static auto type_for(const std::string& str) -> std::shared_ptr<struct project::structure::type::base>;

        [[nodiscard]] auto parse() const -> std::shared_ptr<struct project::structure::type::base>;

    private:
        struct context {
            bool is_const { false };
            bool is_reference { false };
            bool is_pointer { false };
            bool is_subscripted { false };
            std::vector<std::string> subscript_components;
            std::vector<std::string> name;
            std::vector<std::shared_ptr<struct project::structure::type::base>> template_field_types;
        };

        static auto construct_type_for(const std::string& str, struct context *ctx) -> std::shared_ptr<struct project::structure::type::base>;

        [[nodiscard]] auto parse(struct context& ctx, lexer::lexer::lexical_result& stream) const -> std::shared_ptr<struct project::structure::type::base>;

    private:
        std::string m_raw;
    };
}