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

#include <unordered_map>
#include <libFoundation/hashing/hashing.hpp>
#include <libKDL/session/interpreter/scope.hpp>
#include <libKDL/session/types/definition.hpp>
#include <libKDL/session/resources/container.hpp>
#include <libKDL/session/containers/container.hpp>

namespace kdl::session
{
    class session
    {
    public:
        session();

        /**
         * The output format of the session.
         */
        [[nodiscard]] auto format() const -> enum format;

        /**
         * Import and parse the specified KDL file into the session
         */
        auto import_file(const std::string& file) -> void;

        /**
         * Check if we have the specified type definition created in the current session.
         * @param name The name of the type definition
         */
        [[nodiscard]] auto has_type_definition(const std::string& name) const -> bool;

        /**
         * Add a type definition into the current session.
         * @param type
         */
        auto add_type_definition(const struct type_definition& type) -> void;

        [[nodiscard]] auto type_definition(const std::string& name) const -> const struct type_definition*;

        /**
         * Returns the requested namespace container (and creates it if necessary.)
         */
        auto namespace_container(const std::string& name) -> namespace_container&;

        auto type_container(const std::string& type, const std::string& name = "") -> resources::type_container&;

    private:
        enum format m_format { format::extended };
        kdl::session::interpreter::scope m_global_scope;
        std::unordered_map<foundation::hashing::value, struct type_definition> m_type_definitions;
        std::unordered_map<foundation::hashing::value, struct namespace_container> m_containers;
    };
}