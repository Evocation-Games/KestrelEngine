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
#include <vector>
#include <libResource/decorator/decoratable.hpp>
#include <libInterpreter/script/script.hpp>
#include <libInterpreter/token/token.hpp>

namespace resource::definition::binary_template
{
    struct instance;
}

namespace resource::definition::type
{
    struct constructor: public decoratable
    {
    public:
        struct result {
            std::unordered_map<std::string, interpreter::token> variables;
        };

    public:
        explicit constructor(const std::vector<std::string>& arguments);

        [[nodiscard]] auto has_arguments() const -> bool;
        [[nodiscard]] auto argument_count() const -> std::size_t;
        [[nodiscard]] auto argument_at(std::int32_t idx) const -> std::string;
        [[nodiscard]] auto arguments() const -> std::vector<std::string>;

        [[nodiscard]] auto script() const -> const interpreter::script::script&;
        auto set_script(const std::vector<interpreter::script::statement>& statements) -> void;
        auto set_script(const interpreter::script::script& script) -> void;

        [[nodiscard]] auto execute(
            interpreter::scope& scope,
            const std::vector<interpreter::token>& arguments,
            const resource::definition::binary_template::instance& tmpl
        ) const -> result;

    private:
        std::vector<std::string> m_arguments;
        interpreter::script::script m_script;
    };
}