// Copyright (c) 2022 Tom Hancocks
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

#include <stdexcept>
#include <libKestrel/command/container.hpp>

// MARK: - Program Management

auto kestrel::command::container::add_program(const command::program::lua_reference &program) -> void
{
    m_programs.emplace_back(program);
}

// MARK: - Property Management

auto kestrel::command::container::set_property(const property::lua_reference &property) -> void
{
    auto it = m_properties.find(property->name_hash());
    if (it == m_properties.end()) {
        // New property
        m_properties[property->name_hash()] = property;
    }
    else {
        // Replace existing
        it->second = property;
    }
}

auto kestrel::command::container::create_property(const std::string &name, const std::int32_t expected_arguments) -> property::lua_reference
{
    auto new_property = property::lua_reference(new property(name, expected_arguments));
    set_property(new_property);
    return new_property;
}

// MARK: - Execution

auto kestrel::command::container::execute() -> void
{
    for (const auto& program : m_programs) {
        execute_program(program);
    }
}

auto kestrel::command::container::execute_program(const program::lua_reference &program) -> void
{
    if (program->successful_execution_occurred()) {
        return;
    }

    for (const auto& node : program->nodes()) {
        if (!execute_node(node)) {
            return;
        }
    }

    // The program successfully completed.
    program->successful_execution_did_occur();
}

auto kestrel::command::container::execute_node(const node::lua_reference &node, const property::lua_reference& parent_property) -> bool
{
    property::lua_reference property(nullptr);

    if (parent_property.get()) {
        property = parent_property->property_value(node->name_hash());
        if (!property.get()) {
            // Return false as the property does not exist for the node.
            return false;
        }
    }
    else {
        const auto it = m_properties.find(node->name_hash());
        if (it == m_properties.end()) {
            // Return false as the property does not exist for the node
            return false;
        }
        property = it->second;
    }

    // If the property expects arguments, then verify that the node provides enough of them.
    if (property->expected_argument_count() > 0 && property->expected_argument_count() == node->argument_count()) {
        // We have the correct number of arguments. Get the appropriate value for the argument.
        // TODO: Expand this to handle multiple arguments.
        if (node->argument_count() != 1) {
            throw std::runtime_error("Currently unable to handle multiple arguments in a command node.");
        }

        // If the node does not have children, then we are expecting an explicit value.
        if (!node->has_children()) {
            return property->boolean_value();
        }

        // The node has children, so handle each of them...
        for (const auto& child : node->children()) {
            if (!execute_node(child, property)) {
                return false;
            }
        }
    }
    else if (!node->has_children()){
        // No arguments and no children, so this property returns a value directly.
        return property->boolean_value();
    }
    else {
        // No arguments, but child nodes, so we need to handle them.
        for (const auto& child : node->children()) {
            if (!execute_node(child, property)) {
                return false;
            }
        }
    }

    // Reaching this point indicates the program executed successfully.
    return true;
}

