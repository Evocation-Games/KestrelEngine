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
#include <type_traits>

namespace kdl::codegen::lua::ast
{
    struct symbol;
    struct node;
    struct block;
    struct class_definition;
    struct function_definition;
    struct property_definition;

    struct generator
    {
        generator();
        ~generator();

        [[nodiscard]] auto generate() const -> std::string;

        auto add_comment(const std::string& text) -> void;
        auto new_line(std::int8_t n = 1) -> void;
        auto comma(struct node *node) -> struct node *;

        auto begin_block() -> void;
        auto else_block(struct node *condition = nullptr) -> void;
        auto end_block() -> void;

        auto declare_class(const std::string& name, bool implicit = false) -> struct class_definition *;
        auto construct(struct class_definition *klass) -> struct node *;

        auto declare_function(bool implicit, struct symbol *name, const std::vector<struct symbol *>& parameters = {}) -> struct function_definition *;
        auto declare_function(bool implicit, struct symbol *name, struct class_definition *klass, const std::vector<struct symbol *>& parameters = {}) -> struct function_definition *;
        auto declare_static_function(bool implicit, struct symbol *name, struct class_definition *klass, const std::vector<struct symbol *>& parameters = {}) -> struct function_definition *;

        auto declare_property(struct class_definition *klass, struct symbol *name, struct symbol *symbol = nullptr, bool implicit = false) -> struct property_definition *;
        auto synthesize_getter(struct property_definition *property) -> struct node *;
        auto synthesize_setter(struct property_definition *property) -> struct node *;

        auto camel_case(const std::string& name) -> struct symbol *;
        auto symbol(const std::string& name) -> struct symbol *;
        auto private_symbol(const std::string& name) -> struct symbol *;
        auto symbol(struct symbol *symbol) -> struct symbol *;
        auto private_symbol(struct symbol *symbol) -> struct symbol *;
        auto member(struct node *member, struct node *object = nullptr) -> struct node *;
        auto apply_not(struct node *expression) -> struct node *;
        auto nil() -> struct node *;
        auto self() -> struct node *;
        auto number(std::int64_t n) -> struct node *;
        auto string(const std::string& str) -> struct node *;
        auto condition(struct node *condition) -> struct node *;
        auto for_loop(struct node *index, struct node *lower, struct node *upper) -> struct node *;
        auto return_statement(struct node *expression) -> struct node *;
        auto local_assign(struct symbol *var, struct node *expression) -> struct node *;
        auto assign(struct node *lhs, struct node *expression) -> struct node *;
        auto call(struct function_definition *function, const std::vector<struct node *>& arguments = {}) -> struct node *;
        auto call(struct node *expression, struct function_definition *function, const std::vector<struct node *>& arguments = {}) -> struct node *;
        auto userdata_literal() -> struct node *;
        auto subscript(struct node *value, struct node *index) -> struct node *;
        auto count(struct node *value) -> struct node *;
        auto add(struct node *lhs, struct node *rhs) -> struct node *;

        auto push(struct block *block) -> void;
        auto pop() -> void;

        auto emit(struct node *node) -> void;

    private:
        std::vector<struct node *> m_nodes;
        struct block *m_file { nullptr };
        struct block *m_scope { nullptr };
    };

    // -----

    struct node
    {
        [[nodiscard]] virtual auto identifier() const -> std::string { return ""; }
        [[nodiscard]] virtual auto generate(std::uint8_t indent) const -> std::vector<std::string> { return {""}; }
    };

    struct comment : node
    {
        explicit comment(const std::string& text);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        std::vector<std::string> m_lines;
    };

    // -----

    struct comma_terminator : node
    {
        explicit comma_terminator(struct node *node);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_node { nullptr };
    };

    // -----

    struct symbol : node
    {
        explicit symbol(const std::string& name);
        [[nodiscard]] auto identifier() const -> std::string override;
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        std::string m_name;
    };

    struct member : node
    {
        explicit member(struct node *member, struct node *object = nullptr);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_member { nullptr };
        struct node *m_object { nullptr };
    };

    // -----

    struct block : node
    {
        explicit block(struct block *parent = nullptr);
        [[nodiscard]] auto is_root() const -> bool;
        [[nodiscard]] auto has_children() const -> bool;
        [[nodiscard]] auto parent() const -> struct block *;
        [[nodiscard]] auto else_condition() const -> bool;
        auto adopt_parent(struct block *parent) -> struct block *;
        auto set_else_condition(bool f, struct node *condition = nullptr) -> void;
        auto remove_end() -> void;
        auto add_node(struct node *node) -> struct node *;
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        std::vector<struct node *> m_nodes;
        struct block *m_parent { nullptr };
        bool m_else { false };
        bool m_endless { false };
        struct node *m_else_condition { nullptr };
    };

    // -----

    struct function_definition : node
    {
        explicit function_definition(
            struct symbol *name,
            struct class_definition *klass = nullptr,
            bool member = true,
            const std::vector<struct symbol *>& parameters = {}
        );
        [[nodiscard]] auto is_member() const -> bool;
        [[nodiscard]] auto parent_class() const -> struct class_definition *;
        [[nodiscard]] auto identifier() const -> std::string override;
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct class_definition *m_class { nullptr };
        bool m_member { true };
        struct symbol *m_name { nullptr };
        std::vector<struct symbol *> m_parameters;
    };

    struct function_call : node
    {
        explicit function_call(struct function_definition *function, const std::vector<struct node *>& arguments = {});
        function_call(struct node *expression, struct function_definition *function, const std::vector<struct node *>& arguments = {});
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct function_definition *m_function { nullptr };
        std::vector<struct node *> m_arguments;
        struct node *m_object_expression { nullptr };
    };

    // -----

    struct class_definition : node
    {
        explicit class_definition(const std::string& name);
        [[nodiscard]] auto identifier() const -> std::string override;
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        std::string m_name;
    };

    // -----

    struct property_definition : node
    {
        property_definition(struct class_definition *klass, struct symbol *name, struct symbol *symbol);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;
        [[nodiscard]] auto name() const -> struct symbol *;
        [[nodiscard]] auto symbol() const -> struct symbol *;
        [[nodiscard]] auto path() const -> std::string;

    private:
        struct class_definition *m_class { nullptr };
        struct symbol *m_name { nullptr };
        struct symbol *m_symbol { nullptr };
    };

    // -----

    struct property_accessor : node
    {
        property_accessor(struct property_definition *property, bool getter = true);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct property_definition *m_property { nullptr };
        bool m_getter { true };
    };

    // -----

    struct return_statement : node
    {
        explicit return_statement(struct node *expression);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_expression { nullptr };
    };

    // -----

    struct assign_statement : node
    {
        assign_statement(struct node *lhs, struct node *expression);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_lhs { nullptr };
        struct node *m_rhs { nullptr };
    };

    struct local_assign_statement : node
    {
        local_assign_statement(struct symbol *var, struct node *expression);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct symbol *m_symbol { nullptr };
        struct node *m_expression { nullptr };
    };

    // -----

    struct addition_expression : node
    {
        addition_expression(struct node *lhs, struct node *rhs);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_lhs { nullptr };
        struct node *m_rhs { nullptr };
    };

    struct subscript_expression : node
    {
        subscript_expression(struct node *value, struct node *key);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_value { nullptr };
        struct node *m_key { nullptr };
    };

    struct not_expression : node
    {
        explicit not_expression(struct node *expression);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_expression { nullptr };
    };

    // -----

    struct nil_literal: node
    {
        nil_literal() = default;
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;
    };

    struct self_literal: node
    {
        self_literal() = default;
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;
    };

    struct userdata_literal : node
    {
        explicit userdata_literal(struct block *block = nullptr);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;
        auto block() -> struct block *;

    private:
        struct block *m_block { nullptr };
    };

    struct number_literal : node
    {
        explicit number_literal(std::int64_t n);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        std::int64_t m_value;
    };

    struct string_literal : node
    {
        explicit string_literal(const std::string& str);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        std::string m_value;
    };

    // -----

    struct count_operator : node
    {
        explicit count_operator(struct node *value);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_value { nullptr };
    };

    // -----

    struct if_condition : node
    {
        explicit if_condition(struct node *condition);
        [[nodiscard]] auto generate(std::uint8_t indent) const -> std::vector<std::string> override;

    private:
        struct node *m_condition { nullptr };
    };

    struct for_loop : node
    {
        for_loop(struct node *index, struct node *lower, struct node *upper);
        [[nodiscard]] auto generate(std::uint8_t indentation) const -> std::vector<std::string> override;

    private:
        struct node *m_index { nullptr };
        struct node *m_lower { nullptr };
        struct node *m_upper { nullptr };
    };
}