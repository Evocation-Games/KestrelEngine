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

#include <set>
#include <string>
#include <vector>
#include <libInterpreter/script/statement.hpp>
#include <libInterpreter/token/token.hpp>
#include <libInterpreter/scope/scope.hpp>

namespace interpreter::script
{
    struct  script
    {
    public:
        struct result {
            enum class status { success, error } status { status::success };
            token result { static_cast<std::int64_t>(0) };
            std::unordered_map<std::string, token> variables;
        };

    public:
        script() = default;
        script(const std::initializer_list<statement>& statements);
        explicit script(const std::vector<statement>& statements);

        auto add_statement(const statement& stmt) -> void;
        auto add_variable_watch(const std::string& var) -> void;

        [[nodiscard]] auto execute(scope *scope) const -> result;

    private:
        std::vector<statement> m_statements;
        std::set<std::string> m_output_variables;
    };
}