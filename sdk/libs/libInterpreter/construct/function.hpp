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

#include <variant>
#include <functional>
#include <libFoundation/hashing/hashing.hpp>
#include <libFoundation/stream/stream.hpp>
#include <libInterpreter/token/token.hpp>

namespace interpreter
{
    class scope;

    struct function
    {
    public:
        typedef std::function<auto(interpreter::scope *)->token> native_function_body;

        function(const std::string& name, const native_function_body& body);
        function(const std::string& name, const foundation::stream<token>& stream);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto hash() const -> foundation::hashing::value;

        [[nodiscard]] auto expected_argument_count() const -> std::size_t;
        [[nodiscard]] auto argument_name_at(std::int32_t idx) const -> std::string;
        [[nodiscard]] auto argument_type_at(std::int32_t idx) const -> enum token::type;

        auto add_parameter(const std::string& name, enum token::type type = token::integer) -> void;

        [[nodiscard]] auto execute(scope& scope, const std::vector<token>& arguments) const -> token;

    private:
        bool m_commit_variables { false };
        std::string m_name;
        std::variant<native_function_body, foundation::stream<token>> m_body;
        std::vector<std::tuple<std::string, enum token::type>> m_parameters;
    };
}