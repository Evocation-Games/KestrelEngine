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

#include <libInterpreter/scope/scope.hpp>

// MARK: - Construction

interpreter::scope::scope(const scope *parent)
    : m_parent(parent)
{}

// MARK: - Shadowing

auto interpreter::scope::subscope() const -> scope
{
    return scope(this);
}

// MARK: - Resolving



// MARK: - Variable Management

auto interpreter::scope::variable(const std::string &name) const -> token
{
    const auto& name_hash = foundation::hashing::string(name);
    auto it = m_variables.find(name_hash);
    if (it == m_variables.end()) {
        if (m_parent) {
            return m_parent->variable(name);
        }
        throw std::runtime_error("Unrecognised variable referenced.");
    }
    return it->second.value();
}

auto interpreter::scope::get_variable(const std::string &name) -> struct variable &
{
    const auto& name_hash = foundation::hashing::string(name);
    auto it = m_variables.find(name_hash);
    if (it == m_variables.end()) {
        if (m_parent) {
            return const_cast<scope *>(m_parent)->get_variable(name);
        }
        throw std::runtime_error("Unrecognised variable referenced.");
    }
    return it->second;
}

auto interpreter::scope::local_variables() const -> std::vector<struct variable>
{
    std::vector<struct variable> variables;
    for (const auto& [name, variable] : m_variables) {
        variables.emplace_back(variable);
    }
    return variables;
}

auto interpreter::scope::add_variable(const std::string &var, bool value) -> void
{
    add_variable({ var, value });
}

auto interpreter::scope::add_variable(const std::string &var, double value) -> void
{
    add_variable({ var, value });
}

auto interpreter::scope::add_variable(const std::string &var, std::int64_t value) -> void
{
    add_variable({ var, value });
}

auto interpreter::scope::add_variable(const std::string &var, const std::string &value) -> void
{
    add_variable({ var, value });
}

auto interpreter::scope::add_variable(const std::string &var, const token &value) -> void
{
    add_variable({ var, value });
}

auto interpreter::scope::add_variable(const struct variable &var) -> void
{
    if (!has_variable(var.name())) {
        m_variables.insert(std::pair(var.hash(), var));
        m_var_callback(var, notify_reason::create);
    }
}

auto interpreter::scope::has_variable(const std::string &name, bool check_parent) const -> bool
{
    const auto& hash = foundation::hashing::string(name);
    auto it = m_variables.find(hash);
    if (it == m_variables.end()) {
        if (check_parent && m_parent) {
            return m_parent->has_variable(name, check_parent);
        }
        return false;
    }
    return true;
}

// MARK: - Function Management

auto interpreter::scope::has_function(const std::string &name, bool check_parent) const -> bool
{
    const auto& name_hash = foundation::hashing::string(name);
    auto it = m_functions.find(name_hash);
    if (it == m_functions.end()) {
        if (check_parent && m_parent) {
            return m_parent->has_function(name, check_parent);
        }
        return false;
    }
    return true;
}

auto interpreter::scope::call(const std::string& name, const std::vector<token>& arguments) -> token
{
    return call(name, *this, arguments);
}

auto interpreter::scope::call(const std::string &name, scope &scope, const std::vector<token> &arguments) const -> token
{
    const auto& name_hash = foundation::hashing::string(name);
    auto it = m_functions.find(name_hash);
    if (it == m_functions.end()) {
        if (m_parent) {
            return m_parent->call(name, scope, arguments);
        }
        throw std::runtime_error("Unrecognised function referenced/called.");
    }
    return it->second.execute(scope, arguments);
}

auto interpreter::scope::add_function(const struct function &fn) -> void
{
    m_functions.insert(std::pair(fn.hash(), fn));
}

// MARK: - Callbacks

auto interpreter::scope::notify_changes(const std::function<auto(const struct variable &, notify_reason)->void>& callback) -> void
{
    m_var_callback = callback;
}