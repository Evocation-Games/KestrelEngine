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

#include <stdexcept>
#include <memory>
#include <libLexer/lexer.hpp>
#include <libKDL/parser/parser.hpp>
#include <libKDL/session/session.hpp>
#include <libFoundation/stream/stream.hpp>
#include <libFoundation/system/filesystem/file.hpp>
#include <libKDL/session/interpreter/builtin/functions.hpp>
#include <libKDL/exception/unrecognised_type_definition_exception.hpp>
#include <libKDL/exception/duplicate_type_definition_exception.hpp>

// MARK: - Construction

kdl::session::session::session()
{
    interpreter::builtin::functions::install(m_global_scope);
}

// MARK: - Importing / Parsing

auto kdl::session::session::import_file(const std::string &path) -> void
{
    foundation::filesystem::path fs_path(path);
    if (fs_path.empty() || !fs_path.exists()) {
        // TODO: Throw exception
        return;
    }

    auto file = std::make_shared<foundation::filesystem::file>(fs_path);
    if (!file->exists()) {
        // TODO: Throw exception
        return;
    }

    // Perform lexical analysis and parsing...
    auto result = lexer::lexer(file).analyze();
    parse_file_stream(result, this, m_global_scope);
}

auto kdl::session::session::format() const -> enum format
{
    return m_format;
}

// MARK: - Type Definitions

auto kdl::session::session::has_type_definition(const std::string &name) const -> bool
{
    auto it = m_type_definitions.find(foundation::hashing::string(name));
    return it != m_type_definitions.end();
}

auto kdl::session::session::add_type_definition(const kdl::session::type_definition &type) -> void
{
    const auto& name_hash = foundation::hashing::string(type.name());
    auto it = m_type_definitions.find(name_hash);
    if (it != m_type_definitions.end()) {
        throw duplicate_type_definition_exception("Attempted to duplicate type definition '" + type.name() + "'.");
    }
    m_type_definitions.insert(std::pair(name_hash, type));
}

auto kdl::session::session::type_definition(const std::string &name) const -> const struct type_definition *
{
    const auto& name_hash = foundation::hashing::string(name);
    auto it = m_type_definitions.find(name_hash);
    return &it->second;
}

// MARK: - Namespace Containers

auto kdl::session::session::namespace_container(const std::string &name) -> struct namespace_container&
{
    const auto& name_hash = foundation::hashing::string(name);
    auto it = m_containers.find(name_hash);
    if (it != m_containers.end()) {
        return it->second;
    }

    // Construct a new container.
    struct namespace_container container(name);
    m_containers.insert(std::pair(name_hash, container));

    it = m_containers.find(name_hash);
    if (it == m_containers.end()) {
        // TODO: Something seriously wrong here... throw a custom exception
        throw std::runtime_error("");
    }
    return it->second;
}

// MARK: - Resource Type Containers

auto kdl::session::session::type_container(const std::string &type, const std::string &name) -> resources::type_container&
{
    if (!has_type_definition(type)) {
        // TODO: Report using custom error...
        throw std::runtime_error("");
    }

    const auto& type_hash = foundation::hashing::string(type);
    auto it = m_type_definitions.find(type_hash);
    return namespace_container(name).type_container(&it->second);
}