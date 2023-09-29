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

#include <sstream>
#include <functional>
#include <libKDL/codegen/lua/ast.hpp>

// MARK: - Indentation

static inline auto indent_line(const std::string& input, std::uint8_t depth) -> std::string
{
    std::string indentation_str = "   ";
    std::string result = input;
    for (auto i = 0; i < depth; ++i) {
        result.insert(result.begin(), indentation_str.begin(), indentation_str.end());
    }
    return std::move(result);
}

// MARK: - Generator

kdl::codegen::lua::ast::generator::generator()
{
    m_file = new block();
    m_scope = m_file;
}

kdl::codegen::lua::ast::generator::~generator()
{
    delete m_file;
    for (auto it : m_nodes) {
        delete it;
    }
}

auto kdl::codegen::lua::ast::generator::generate() const -> std::string
{
    std::string result;
    for (const auto& it : m_file->generate(0)) {
        result += it + "\n";
    }
    return std::move(result);
}

auto kdl::codegen::lua::ast::generator::emit(struct node *node) -> void
{
    m_scope->add_node(node);
}

// MARK: - Misc

auto kdl::codegen::lua::ast::generator::new_line(std::int8_t n) -> void
{
    for (auto i = 0; i < n; ++i) {
        auto node = m_scope->add_node(new struct node());
        m_nodes.emplace_back(node);
    }
}

// MARK: - Comment

auto kdl::codegen::lua::ast::generator::add_comment(const std::string& text) -> void
{
    auto node = m_scope->add_node(new comment(text));
    m_nodes.emplace_back(node);
}

kdl::codegen::lua::ast::comment::comment(const std::string &text)
{
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        m_lines.emplace_back(line);
    }
}

auto kdl::codegen::lua::ast::comment::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::vector<std::string> result;
    for (const auto& line : m_lines) {
        result.emplace_back(indent_line("-- " + line, indent));
    }
    return std::move(result);
}

// MARK: - Symbol

kdl::codegen::lua::ast::symbol::symbol(const std::string& name)
    : m_name(name)
{}

auto kdl::codegen::lua::ast::symbol::identifier() const -> std::string
{
    return m_name;
}

auto kdl::codegen::lua::ast::symbol::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    return { indent_line(identifier(), indent) };
}

auto kdl::codegen::lua::ast::generator::camel_case(const std::string &name) -> struct symbol *
{
    auto camel_case = name;
    camel_case[0] = std::tolower(camel_case[0]);

    auto node = new struct symbol(camel_case);
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::symbol(const std::string &name) -> struct symbol *
{
    auto node = new struct symbol(name);
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::private_symbol(const std::string &name) -> struct symbol *
{
    auto node = new struct symbol("_" + name);
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::symbol(struct symbol *name) -> struct symbol *
{
    auto node = new struct symbol(name->identifier());
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::private_symbol(struct symbol *name) -> struct symbol *
{
    auto node = new struct symbol("_" + name->identifier());
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Block

kdl::codegen::lua::ast::block::block(struct block *parent)
    : m_parent(parent)
{}

auto kdl::codegen::lua::ast::block::is_root() const -> bool
{
    return (m_parent == nullptr);
}

auto kdl::codegen::lua::ast::block::has_children() const -> bool
{
    return !m_nodes.empty();
}

auto kdl::codegen::lua::ast::block::parent() const -> struct block *
{
    return m_parent;
}

auto kdl::codegen::lua::ast::block::else_condition() const -> bool
{
    return m_else;
}

auto kdl::codegen::lua::ast::block::set_else_condition(bool f, struct node *condition) -> void
{
    m_else = f;
    m_else_condition = condition;
}

auto kdl::codegen::lua::ast::block::remove_end() -> void
{
    m_endless = true;
}

auto kdl::codegen::lua::ast::block::add_node(struct node *node) -> struct node *
{
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::block::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::vector<std::string> result;
    std::uint8_t depth = is_root() ? 0 : indent + 1;

    for (const auto it : m_nodes) {
        auto node_out = it->generate(depth);
        result.insert(result.end(), node_out.begin(), node_out.end());
    }

    if (m_parent && !m_endless) {
        if (m_else && m_else_condition) {
            auto condition = m_else_condition->generate(0).front();
            result.emplace_back(indent_line("else" + condition, depth));
        }
        else if (m_else) {
            result.emplace_back(indent_line("else", depth));
        }
        else {
            result.emplace_back(indent_line("end", depth));
        }
    }

    return std::move(result);
}

auto kdl::codegen::lua::ast::generator::begin_block() -> void
{
    auto node = m_scope->add_node(new struct block(m_scope));
    m_scope = reinterpret_cast<decltype(m_scope)>(node);
    m_nodes.emplace_back(node);
}

auto kdl::codegen::lua::ast::generator::end_block() -> void
{
    if (m_scope->is_root()) {
        return;
    }
    m_scope = m_scope->parent();
}

auto kdl::codegen::lua::ast::generator::else_block(struct node *condition) -> void
{
    if (m_scope->is_root()) {
        return;
    }
    m_scope->set_else_condition(true, condition);
    m_scope = m_scope->parent();
}

auto kdl::codegen::lua::ast::block::adopt_parent(struct block *parent) -> struct block *
{
    m_parent = parent;
    return this;
};

// MARK: - Class Definition

kdl::codegen::lua::ast::class_definition::class_definition(const std::string& name)
    : m_name(name)
{}

auto kdl::codegen::lua::ast::generator::declare_class(const std::string& name, bool implicit) -> struct class_definition *
{
    auto node = new struct class_definition(name);
    if (!implicit) {
        m_scope->add_node(node);
    }
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::class_definition::identifier() const -> std::string
{
    return m_name;
}

auto kdl::codegen::lua::ast::class_definition::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    struct symbol new_symbol("new");
    struct class_definition klass("Class");
    struct function_definition klass_new(&new_symbol, &klass, true);

    struct symbol lhs(m_name);
    struct function_call rhs(&klass_new);

    struct assign_statement stmt(&lhs, &rhs);
    return stmt.generate(indent);
}

auto kdl::codegen::lua::ast::generator::construct(struct class_definition *klass) -> struct node *
{
    auto new_symbol = new struct symbol("new");
    auto klass_new = new struct function_definition(new_symbol, klass, true);
    auto call = new struct function_call(klass_new);
    m_nodes.emplace_back(new_symbol);
    m_nodes.emplace_back(klass_new);
    m_nodes.emplace_back(call);
    return call;
}

// MARK: - Function Definition

kdl::codegen::lua::ast::function_definition::function_definition(struct symbol *name, struct class_definition *klass, bool member, const std::vector<struct symbol *> &parameters)
    : m_name(name), m_class(klass), m_member(member), m_parameters(parameters)
{}

auto kdl::codegen::lua::ast::function_definition::identifier() const -> std::string
{
    return m_name->identifier();
}

auto kdl::codegen::lua::ast::function_definition::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::string result = "function ";

    if (m_class) {
        result += m_class->identifier();
        result += m_member ? ":" : ".";
    }

    result += m_name->identifier() + "(";
    for (auto it = m_parameters.begin(); it != m_parameters.end(); ++it) {
        if (it != m_parameters.begin()) {
            result += ", ";
        }
        result += const_cast<struct symbol *>(*it)->identifier();
    }
    result += ")";

    return { indent_line(result, indent) };
}

auto kdl::codegen::lua::ast::function_definition::is_member() const -> bool
{
    return m_member;
}

auto kdl::codegen::lua::ast::function_definition::parent_class() const -> struct class_definition *
{
    return m_class;
}

auto kdl::codegen::lua::ast::generator::declare_function(bool implicit, struct symbol *name, const std::vector<struct symbol *> &parameters) -> struct function_definition *
{
    auto node = new struct function_definition(name, nullptr, false, parameters);
    if (!implicit) {
        m_scope->add_node(node);
    }
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::declare_function(bool implicit, struct symbol *name, struct class_definition *klass, const std::vector<struct symbol *> &parameters) -> struct function_definition *
{
    auto node = new struct function_definition(name, klass, true, parameters);
    if (!implicit) {
        m_scope->add_node(node);
    }
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::declare_static_function(bool implicit, struct symbol *name, struct class_definition *klass, const std::vector<struct symbol *> &parameters) -> struct function_definition *
{
    auto node = new struct function_definition(name, klass, false, parameters);
    if (!implicit) {
        m_scope->add_node(node);
    }
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Function Call

kdl::codegen::lua::ast::function_call::function_call(struct function_definition *function, const std::vector<struct node *> &arguments)
    : m_function(function), m_arguments(arguments)
{}

kdl::codegen::lua::ast::function_call::function_call(struct node *expression, struct function_definition *function, const std::vector<struct node *> &arguments)
    : m_function(function), m_arguments(arguments), m_object_expression(expression)
{}

auto kdl::codegen::lua::ast::function_call::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::string result;
    if (m_function->parent_class()) {
        if (m_object_expression && m_function->is_member()) {
            result += m_object_expression->generate(0).front();
        }
        else {
            result += m_function->parent_class()->identifier();
        }
        result += m_function->is_member() ? ":" : ".";
    }

    result += m_function->identifier() + "(";
    for (auto it = m_arguments.begin(); it != m_arguments.end(); ++it) {
        if (it != m_arguments.begin()) {
            result += ", ";
        }
        result += const_cast<struct node *>(*it)->generate(0).front();
    }
    result += ")";

    return { indent_line(result, indent) };
}

auto kdl::codegen::lua::ast::generator::call(struct function_definition *function, const std::vector<struct node *> &arguments) -> struct node *
{
    auto node = new struct function_call(function, arguments);
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::call(struct node *expression, struct function_definition *function, const std::vector<struct node *> &arguments) -> struct node *
{
    auto node = new struct function_call(expression, function, arguments);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Assign Statement

kdl::codegen::lua::ast::assign_statement::assign_statement(struct node *lhs, struct node *expression)
    : m_lhs(lhs), m_rhs(expression)
{}

auto kdl::codegen::lua::ast::assign_statement::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::vector<std::string> result;
    std::string lhs = m_lhs->generate(0).front();
    auto rhs = m_rhs->generate(indent + 1);

    auto first_start = rhs.front().find_first_not_of(' ');
    auto first = rhs.front().substr(first_start == std::string::npos ? 0 : first_start);
    rhs.erase(rhs.begin());

    result.emplace_back(indent_line(lhs + " = " + first, indent));
    for (const auto& it : rhs) {
        result.emplace_back(it);
    }

    return result;
}

auto kdl::codegen::lua::ast::generator::assign(struct node *lhs, struct node *expression) -> struct node *
{
    auto node = m_scope->add_node(new struct assign_statement(lhs, expression));
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Local Assignment

kdl::codegen::lua::ast::local_assign_statement::local_assign_statement(struct symbol *var, struct node *expression)
    : m_symbol(var), m_expression(expression)
{}

auto kdl::codegen::lua::ast::local_assign_statement::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto var = m_symbol->generate(0).front();
    auto expr = m_expression->generate(0).front();
    return { indent_line("local " + var + " = " + expr, indent) };
}

auto kdl::codegen::lua::ast::generator::local_assign(struct symbol *var, struct node *expression) -> struct node *
{
    auto node = m_scope->add_node(new struct local_assign_statement(var, expression));
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Not Expression

kdl::codegen::lua::ast::not_expression::not_expression(struct node *expression)
    : m_expression(expression)
{}

auto kdl::codegen::lua::ast::not_expression::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto expr = m_expression->generate(0).front();
    return { indent_line("not " + expr, indent) };
}

auto kdl::codegen::lua::ast::generator::apply_not(struct node *expression) -> struct node *
{
    auto node = new struct not_expression(expression);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - If Condition

kdl::codegen::lua::ast::if_condition::if_condition(struct node *condition)
    : m_condition(condition)
{}

auto kdl::codegen::lua::ast::if_condition::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto expr = m_condition->generate(0).front();
    return { indent_line("if " + expr + " then", indent) };
}

auto kdl::codegen::lua::ast::generator::condition(struct node *condition) -> struct node *
{
    auto node = m_scope->add_node(new struct if_condition(condition));
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - For Loop

kdl::codegen::lua::ast::for_loop::for_loop(struct node *index, struct node *lower, struct node *upper)
    : m_index(index), m_lower(lower), m_upper(upper)
{}

auto kdl::codegen::lua::ast::for_loop::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto index = m_index->generate(0).front();
    auto lower = m_lower->generate(0).front();
    auto upper = m_upper->generate(0).front();
    return { indent_line("for " + index + " = " + lower + "," + upper + " do", indent) };
}

auto kdl::codegen::lua::ast::generator::for_loop(struct node *index, struct node *lower, struct node *upper) -> struct node *
{
    auto node = m_scope->add_node(new struct for_loop(index, lower, upper));
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Nil Literal

auto kdl::codegen::lua::ast::nil_literal::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    return { "nil" };
}

auto kdl::codegen::lua::ast::generator::nil() -> struct node *
{
    auto node = new struct nil_literal();
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Self Literal

auto kdl::codegen::lua::ast::self_literal::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    return { "self" };
}

auto kdl::codegen::lua::ast::generator::self() -> struct node *
{
    auto node = new struct self_literal();
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Return Statement

kdl::codegen::lua::ast::return_statement::return_statement(struct node *expression)
    : m_expression(expression)
{}

auto kdl::codegen::lua::ast::return_statement::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto expr = m_expression->generate(0).front();
    return { indent_line("return " + expr, indent) };
}

auto kdl::codegen::lua::ast::generator::return_statement(struct node *expression) -> struct node *
{
    auto node = m_scope->add_node(new struct return_statement(expression));
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - String Literal

kdl::codegen::lua::ast::string_literal::string_literal(const std::string &str)
    : m_value(str)
{}

auto kdl::codegen::lua::ast::string_literal::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    return { indent_line("\"" + m_value + "\"", indent) };
}

auto kdl::codegen::lua::ast::generator::string(const std::string &str) -> struct node *
{
    auto node = new struct string_literal(str);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Member

kdl::codegen::lua::ast::member::member(struct node *member, struct node *object)
    : m_object(object), m_member(member)
{}

auto kdl::codegen::lua::ast::member::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::string result;
    if (m_object) {
        result += m_object->generate(0).front() + ".";
    }
    result += m_member->generate(0).front();
    return { result };
}

auto kdl::codegen::lua::ast::generator::member(struct node *member, struct node *object) -> struct node *
{
    auto node = new struct member(member, object);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Property Definition

kdl::codegen::lua::ast::property_definition::property_definition(struct class_definition *klass, struct symbol *name, struct symbol *symbol)
    : m_class(klass), m_name(name), m_symbol(symbol ?: name)
{}

auto kdl::codegen::lua::ast::property_definition::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    return { indent_line(path() + " = {}", indent) };
}

auto kdl::codegen::lua::ast::property_definition::name() const -> struct symbol *
{
    return m_name;
}

auto kdl::codegen::lua::ast::property_definition::symbol() const -> struct symbol *
{
    return m_symbol;
}

auto kdl::codegen::lua::ast::property_definition::path() const -> std::string
{
    auto klass = m_class->identifier();
    auto name = m_name->identifier();
    return klass + ".properties." + name;
}

auto kdl::codegen::lua::ast::generator::declare_property(struct class_definition *klass, struct symbol *name, struct symbol *symbol, bool implicit) -> struct property_definition *
{
    auto node = new struct property_definition(klass, name, symbol);
    if (!implicit) {
        m_scope->add_node(node);
    }
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Property Accessors

kdl::codegen::lua::ast::property_accessor::property_accessor(struct property_definition *property, bool getter)
    : m_property(property), m_getter(getter)
{}

auto kdl::codegen::lua::ast::property_accessor::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::vector<std::string> result;

    struct symbol value("newValue");
    std::vector<struct symbol *>params;
    if (!m_getter) {
        params.emplace_back(&value);
    }

    struct symbol name(m_getter ? "get" : "set");
    struct class_definition shim(m_property->path());
    struct function_definition accessor(&name, &shim, true, params);

    result.emplace_back(accessor.generate(indent).front());

    struct self_literal self;
    struct symbol property_value("_" + m_property->symbol()->identifier());
    struct member member_value(&property_value, &self);

    if (m_getter) {
        struct return_statement ret(&member_value);
        result.emplace_back(ret.generate(indent + 1).front());
    }
    else {
        struct assign_statement assign(&member_value, &value);
        result.emplace_back(assign.generate(indent + 1).front());
    }
    result.emplace_back(indent_line("end", indent));
    return std::move(result);
}

auto kdl::codegen::lua::ast::generator::synthesize_getter(struct property_definition *property) -> struct node *
{
    auto node = m_scope->add_node(new struct property_accessor(property, true));
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::synthesize_setter(struct property_definition *property) -> struct node *
{
    auto node = m_scope->add_node(new struct property_accessor(property, false));
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Array / Userdata Literal

kdl::codegen::lua::ast::userdata_literal::userdata_literal(struct block *block)
    : m_block(block)
{}

auto kdl::codegen::lua::ast::userdata_literal::block() -> struct block *
{
    return m_block;
}

auto kdl::codegen::lua::ast::userdata_literal::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::vector<std::string> result;
    if (m_block && m_block->has_children()) {
        m_block->remove_end();
        result.emplace_back(indent_line("{", indent - 1));

        auto children = m_block->generate(indent - 1);
        for (const auto& child : children) {
            result.emplace_back(child);
        }

        result.emplace_back(indent_line("}", indent - 1));
    }
    else {
        result.emplace_back(indent_line("{}", indent - 1));
    }
    return result;
}

auto kdl::codegen::lua::ast::generator::userdata_literal() -> struct node *
{
    auto block = new struct block();
    auto node = new struct userdata_literal(block);
    m_nodes.emplace_back(block);
    m_nodes.emplace_back(node);
    return node;
}

auto kdl::codegen::lua::ast::generator::push(struct block *block) -> void
{
    m_scope = block->adopt_parent(m_scope);
}

auto kdl::codegen::lua::ast::generator::pop() -> void
{
    if (m_scope->is_root()) {
        return;
    }
    m_scope = m_scope->parent();
}

// MARK: - Subscript Expression

kdl::codegen::lua::ast::subscript_expression::subscript_expression(struct node *value, struct node *key)
    : m_value(value), m_key(key)
{}

auto kdl::codegen::lua::ast::subscript_expression::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto key = m_key->generate(0).front();
    if (m_value) {
        auto value = m_value->generate(0).front();
        return { value + "[" + key + "]" };
    }
    else {
        return { "[" + key + "]" };
    }
}

auto kdl::codegen::lua::ast::generator::subscript(struct node *value, struct node *index) -> struct node *
{
    auto node = new struct subscript_expression(value, index);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Number Literal

kdl::codegen::lua::ast::number_literal::number_literal(std::int64_t n)
    : m_value(n)
{}

auto kdl::codegen::lua::ast::number_literal::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    return { std::to_string(m_value) };
}

auto kdl::codegen::lua::ast::generator::number(std::int64_t n) -> struct node *
{
    auto node = new struct number_literal(n);
    m_nodes.emplace_back(node);
    return node;
}

// MARK; - Count Operator

kdl::codegen::lua::ast::count_operator::count_operator(struct node *value)
    : m_value(value)
{}

auto kdl::codegen::lua::ast::count_operator::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto value = m_value->generate(0).front();
    return { "#" + value };
}

auto kdl::codegen::lua::ast::generator::count(struct node *value) -> struct node *
{
    auto node = new struct count_operator(value);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Addition

kdl::codegen::lua::ast::addition_expression::addition_expression(struct node *lhs, struct node *rhs)
    : m_lhs(lhs), m_rhs(rhs)
{}

auto kdl::codegen::lua::ast::addition_expression::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    auto lhs = m_lhs->generate(0).front();
    auto rhs = m_rhs->generate(0).front();
    return { lhs + " + " + rhs };
}

auto kdl::codegen::lua::ast::generator::add(struct node *lhs, struct node *rhs) -> struct node *
{
    auto node = new struct addition_expression(lhs, rhs);
    m_nodes.emplace_back(node);
    return node;
}

// MARK: - Comma

kdl::codegen::lua::ast::comma_terminator::comma_terminator(struct node *node)
    : m_node(node)
{}

auto kdl::codegen::lua::ast::comma_terminator::generate(std::uint8_t indent) const -> std::vector<std::string>
{
    std::string comma = ",";
    auto result = m_node->generate(indent);
    result.back().insert(result.back().end(), comma.begin(), comma.end());
    return std::move(result);
}

auto kdl::codegen::lua::ast::generator::comma(struct node *node) -> struct node *
{
    auto wrapper_node = new struct comma_terminator(node);
    m_nodes.emplace_back(wrapper_node);
    return wrapper_node;
}