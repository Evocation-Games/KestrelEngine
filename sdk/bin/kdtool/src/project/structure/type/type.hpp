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
#include <type_traits>
#include "project/structure/symbol.hpp"

namespace kdtool::project::structure::type
{
    enum class scalar
    {
        error_type, void_type, boolean_type, char_type, short_type, long_type, long_long_type,
        uchar_type, ushort_type, ulong_type, ulong_long_type, float_type, double_type,
        long_double_type, int_type, uint_type
    };

    struct base
    {
        [[nodiscard]] virtual auto name() const -> std::string = 0;
    };

    struct internal_void_type : base
    {
        [[nodiscard]] auto name() const -> std::string override { return "void"; }
    };

    struct internal_error_type : base
    {
        [[nodiscard]] auto name() const -> std::string override { return "void"; }
    };

    template<class T>
    concept type_provider = requires (const T& type, enum scalar& base, const std::string& str) {
        { T::type_for(base) } -> std::same_as<std::shared_ptr<struct type::base>>;
        { T::type_for(str) } -> std::same_as<std::shared_ptr<struct type::base>>;
    };

    template<type_provider T>
    static auto parse_string(const std::string& str) -> std::shared_ptr<struct type::base>
    {
        return T::type_for(str);
    }

    template<type_provider T>
    static auto make(enum scalar scalar) -> std::shared_ptr<struct type::base>
    {
        return T::type_for(scalar);
    }

    static auto void_type() -> std::shared_ptr<struct type::base>
    {
        return std::make_shared<internal_void_type>();
    }

    static auto error_type() -> std::shared_ptr<struct type::base>
    {
        return std::make_shared<internal_error_type>();
    }

}