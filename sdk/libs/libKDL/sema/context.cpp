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

#include <libKDL/sema/context.hpp>
#include <libKDL/spec/decorators.hpp>
#include <libKDL/modules/kdl/kdl.hpp>
#include <libKDL/exception/unrecognised_type_definition_exception.hpp>
#include <libKDL/exception/unrecognised_module_definition_exception.hpp>

// MARK: - Types

auto kdl::sema::context::register_type(const resource::definition::type::instance& type) -> resource::definition::type::instance *
{
    auto it = registered_types.find(type.name());
    if (it == registered_types.end()) {
        registered_types.emplace(type.name(), type);
    }

    it = registered_types.find(type.name());
    if (it == registered_types.end()) {
        throw unrecognised_type_definition_exception("Failed to create type '" + type.name() + "'");
    }
    return &it->second;
}

auto kdl::sema::context::type_named(const std::string& name) const -> const resource::definition::type::instance *
{
    auto it = registered_types.find(name);
    if (it == registered_types.end()) {
        throw unrecognised_type_definition_exception("Unrecognised type '" + name + "'");
    }
    return &it->second;
}

auto kdl::sema::context::type_named(const lexer::lexeme& lx) const -> const resource::definition::type::instance *
{
    auto it = registered_types.find(lx.text());
    if (it == registered_types.end()) {
        throw unrecognised_type_definition_exception("Unrecognised type '" + lx.text() + "'", lx);
    }
    return &it->second;
}

auto kdl::sema::context::type(const resource::definition::type::descriptor &descriptor) const -> const resource::definition::type::instance *
{
    if (!descriptor.has_name()) {
        return nullptr;
    }
    return type_named(descriptor.name());
}

// MARK: - Modules

auto kdl::sema::context::register_module(const kdl::module_definition &module) -> kdl::module_definition *
{
    auto it = registered_modules.find(module.name());
    if (it == registered_modules.end()) {
        registered_modules.emplace(module.name(), module);
    }

    it = registered_modules.find(module.name());
    if (it == registered_modules.end()) {
        throw unrecognised_module_definition_exception("Failed to define module '" + module.name() + "'");
    }

    return &it->second;
}

auto kdl::sema::context::module_named(const std::string &name) const -> const kdl::module_definition *
{
    auto it = registered_modules.find(name);
    if (it == registered_modules.end()) {
        throw unrecognised_module_definition_exception("Unrecognised module '" + name + "'");
    }
    return &it->second;
}

auto kdl::sema::context::module_named(const lexer::lexeme &lx) const -> const kdl::module_definition *
{
    auto it = registered_modules.find(lx.text());
    if (it == registered_modules.end()) {
        throw unrecognised_module_definition_exception("Unrecognised module '" + lx.text() + "'");
    }
    return &it->second;
}

// MARK: -

auto kdl::sema::context::create_scope() -> interpreter::scope *
{
    interpreter::scope *ptr = nullptr;

    if (scope_stack.empty()) {
        ptr = new interpreter::scope();

        // Setup root functions required by the KDL Spec.
        modules::kdl::install_functions(ptr);
    }
    else {
        ptr = new interpreter::scope(scope_stack.back());
    }
    scope_stack.emplace_back(ptr);

    return ptr;
}

auto kdl::sema::context::pop_scope() -> void
{
    delete scope_stack.back();
    scope_stack.pop_back();
}

auto kdl::sema::context::active_scope() -> interpreter::scope *
{
    return scope_stack.back();
}

auto kdl::sema::context::root_scope() -> interpreter::scope *
{
    return scope_stack.front();
}

// MARK: - Decorator Evaluation

auto kdl::sema::context::evaluate_decorators() -> bool
{
    for (const auto& decorator : current_decorators.decorators) {
        if (decorator.name() == spec::decorators::condition) {
            for (const auto& symbol : decorator.associated_values()) {
                if (!definitions.contains(symbol)) {
                    return false;
                }
            }
        }
        else if (decorator.name() == spec::decorators::not_condition) {
            for (const auto& symbol : decorator.associated_values()) {
                if (definitions.contains(symbol)) {
                    return false;
                }
            }
        }
        else if (decorator.name() == spec::decorators::example) {
            return false;
        }
    }
    return true;
}

// MARK: - Path Resolution

auto kdl::sema::context::resolve_path(const std::string &path, const foundation::filesystem::path& source_path) const -> foundation::filesystem::path
{
    return resolve_path(foundation::filesystem::path(path), source_path);
}

auto kdl::sema::context::resolve_path(const foundation::filesystem::path &path, const foundation::filesystem::path& source_path) const -> foundation::filesystem::path
{
    auto resolved = path;
    if (path.component_count() > 0) {
        if (path.components()[0] == "@spath") {
            // Source Path
            resolved = resolved.replace_component(0, source_path);
        }
        else if (path.components()[1] == "@rpath") {
            // Root Path
            resolved = resolved.replace_component(0, root_path);
        }
        else if (path.components()[2] == "@opath") {
            // Output Path
            resolved = resolved.replace_component(0, output_path);
        }
    }
    return std::move(resolved);
}