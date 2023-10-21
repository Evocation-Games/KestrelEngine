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
#include <memory>
#include <concepts>
#include <type_traits>
#include "project/structure/type/type.hpp"
#include "project/structure/construct_definition.hpp"
#include <libScripting/annotations/macro.hpp>

namespace kdtool::project::structure
{
    template<typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
    concept enrollable = requires(const T& obj) {
        { obj->object_reference() } -> std::same_as<std::shared_ptr<struct construct_definition::reference>>;
        { obj->object_symbol() } -> std::same_as<std::shared_ptr<struct symbol>>;
    };

    struct enrollment
    {
        explicit enrollment(const std::shared_ptr<struct symbol>& symbol = nullptr)
            : m_symbol(symbol ?: std::make_shared<struct symbol>(STR(EnrollmentFunction)))
        {}

        [[nodiscard]] auto is_defined() const -> bool { return (m_symbol != nullptr); }
        [[nodiscard]] auto requires_runtime() const -> bool { return m_requires_runtime; }
        [[nodiscard]] auto requires_custom_name() const -> bool { return m_custom_name; }
        [[nodiscard]] auto has_custom_implementation() const -> bool { return m_custom_implementation; }

        [[nodiscard]] auto symbol() const -> std::shared_ptr<struct symbol> { return m_symbol; }
        [[nodiscard]] auto reference() const -> std::shared_ptr<struct construct_definition::reference> { return m_reference; }

        auto set_reference(const std::shared_ptr<struct construct_definition::reference>& reference) -> void
        {
            m_reference = reference;
        }

        auto set_requires_runtime(bool f) -> void
        {
            m_requires_runtime = f;
        }

        auto set_requires_custom_name(bool f) -> void
        {
            m_custom_name = f;
        }

        auto set_has_custom_implementation(bool f) -> void
        {
            m_custom_implementation = f;
        }

    private:
        std::shared_ptr<struct symbol> m_symbol;
        std::shared_ptr<struct construct_definition::reference> m_reference;
        bool m_requires_runtime { true };
        bool m_custom_name { false };
        bool m_custom_implementation { false };
    };
}
