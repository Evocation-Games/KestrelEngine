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

#pragma once

#include <type_traits>
#include <concepts>
#include "lua/ast/symbol.hpp"
#include "lua/ast/lua_reference.hpp"

namespace luatool::lua_api::ast
{
    template<typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
    concept is_lua_object = requires(T v) {
        { v->object_reference() } -> std::same_as<ast::lua_reference>;
        { v->object_symbol() } -> std::same_as<std::shared_ptr<ast::symbol>>;
    };

    struct lua_enrollment
    {
        lua_enrollment() = default;

        template<typename T>
            requires is_lua_object<T>
        explicit lua_enrollment(const T *object, std::shared_ptr<ast::symbol> symbol);
        explicit lua_enrollment(std::shared_ptr<ast::symbol> symbol);

        [[nodiscard]] auto is_defined() const -> bool;
        [[nodiscard]] auto requires_runtime() const -> bool;
        [[nodiscard]] auto requires_custom_name() const -> bool;

        [[nodiscard]] auto symbol() const -> std::shared_ptr<ast::symbol>;

        auto set_reference(const ast::lua_reference& reference) -> void;

        auto set_requires_runtime(bool f) -> void;
        auto set_requires_custom_name(bool f) -> void;
        auto set_uses_custom_implementation(bool f) -> void;

    private:
        std::shared_ptr<ast::symbol> m_symbol { ast::symbol::make_cxx("enroll_lua_api") };
        ast::lua_reference m_reference;
        bool m_requires_runtime { true };
        bool m_custom_name { false };
        bool m_custom_implementation { false };
    };
}