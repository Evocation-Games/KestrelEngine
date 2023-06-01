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

#include <functional>
#include <unordered_map>
#include <libInterpreter/token/token.hpp>
#include <libInterpreter/construct/function.hpp>
#include <libInterpreter/construct/variable.hpp>
#include <libFoundation/hashing/hashing.hpp>
#include <libResource/declaration/value.hpp>

namespace interpreter
{
    struct scope
    {
    public:
        enum class notify_reason { create, update };

        explicit scope(const scope* parent = nullptr);

        [[nodiscard]] auto subscope() const -> scope;

        auto notify_changes(const std::function<auto(const variable&, notify_reason)->void>& callback) -> void;

        [[nodiscard]] auto has_variable(const std::string& name, bool check_parent = false) const -> bool;
        [[nodiscard]] auto variable(const std::string& name) const -> token;
        [[nodiscard]] auto get_variable(const std::string& name) -> struct variable&;
        [[nodiscard]] auto local_variables() const -> std::vector<struct variable>;

        [[nodiscard]] auto has_function(const std::string& name, bool check_parent = false) const -> bool;
        [[nodiscard]] auto call(const std::string& name, const std::vector<token>& arguments = {}) -> token;
        [[nodiscard]] auto call(const std::string& name, scope& scope, const std::vector<token>& arguments = {}) const -> token;

        auto add_variable(const std::string& var, double value) -> void;
        auto add_variable(const std::string& var, bool value) -> void;
        auto add_variable(const std::string& var, std::int64_t value) -> void;
        auto add_variable(const std::string& var, const std::string& value) -> void;
        auto add_variable(const std::string& var, const token& value) -> void;
        auto add_variable(const std::string& var, const resource::value_container& value) -> void;
        auto add_variable(const struct variable& var) -> void;
        auto add_function(const struct function& fn) -> void;

    private:
        const scope *m_parent { nullptr };
        std::unordered_map<foundation::hashing::value, struct variable> m_variables;
        std::unordered_map<foundation::hashing::value, struct function> m_functions;
        std::function<auto(const struct variable&, notify_reason)->void> m_var_callback {
            [] (const struct variable&, notify_reason) {}
        };
    };
}