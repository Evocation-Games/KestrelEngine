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

#include "project/structure/type/type.hpp"

namespace kdtool::cxx::scalar
{
    struct void_type : project::structure::type::internal_void_type
    {
        [[nodiscard]] auto name() const -> std::string override { return "void"; }
    };

    struct error_type : project::structure::type::internal_error_type
    {
        [[nodiscard]] auto name() const -> std::string override { return "error"; }
    };

    struct bool_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "bool"; }
    };

    struct int8_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::int8_t"; }
    };

    struct int16_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::int16_t"; }
    };

    struct int32_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::int32_t"; }
    };

    struct int64_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::int64_t"; }
    };

    struct uint8_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::uint8_t"; }
    };

    struct uint16_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::uint16_t"; }
    };

    struct uint32_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::uint32_t"; }
    };

    struct uint64_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "std::uint64_t"; }
    };

    struct float_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "float"; }
    };

    struct double_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "double"; }
    };

    struct long_double_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "long double"; }
    };

    struct int_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "int"; }
    };

    struct unsigned_int_type : project::structure::type::base
    {
        [[nodiscard]] auto name() const -> std::string override { return "unsigned int"; }
    };
}