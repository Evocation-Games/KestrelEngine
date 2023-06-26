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

#include <memory>
#include <string>
#include <concepts>
#include <libFoundation/system/filesystem/path.hpp>

namespace kdtool::project::structure
{
    struct symbol;
    struct enrollment;

    struct construct_definition
    {
        explicit construct_definition(const std::shared_ptr<struct symbol>& symbol)
            : m_symbol(symbol)
        {}

        [[nodiscard]] auto symbol() const -> std::shared_ptr<struct symbol>
        {
            return m_symbol;
        }

        enum class type
        {
            // CXX
            none, is_namespace, is_class, is_enum, is_function, is_constructor, is_property, is_enum_case,
            // KDL
            is_resource_type, is_resource_field, is_resource_value, is_resource_value_symbol, is_resource_constructor,
        };

        static auto type() -> enum type
        {
            return type::none;
        }

        [[nodiscard]] virtual auto instance_type() const -> enum type
        {
            return type();
        }

        virtual auto set_location(const std::string& path) -> void
        {
            m_location = path;
        }

        [[nodiscard]] virtual auto location() const -> std::string
        {
            return m_location;
        }

        virtual auto set(const std::shared_ptr<struct enrollment>& enrollment) -> void
        {
            m_enrollment = enrollment;
        }

        [[nodiscard]] virtual auto enrollment() const -> std::shared_ptr<struct enrollment>
        {
            return m_enrollment;
        }

    public:
        struct reference {
            reference() = default;
            explicit reference(const std::string& name) : m_name(name) {}

            [[nodiscard]] virtual auto is_defined() const -> bool { return !name().empty(); }
            [[nodiscard]] virtual auto name() const -> std::string { return m_name; }

        private:
            std::string m_name;
        };

    private:
        std::string m_location;
        std::shared_ptr<struct symbol> m_symbol;
        std::shared_ptr<struct reference> m_reference;
        std::shared_ptr<struct enrollment> m_enrollment;
    };

    template<class T>
    concept is_construct_definition = requires (const T& t) {
        { T::type() } -> std::same_as<enum construct_definition::type>;
    };
}