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
#include <libFoundation/stream/stream.hpp>
#include <libKDL/tokenizer/token.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/declaration/instance.hpp>
#include <libResource/declaration/value.hpp>
#include <libKDL/sema/context.hpp>

namespace kdl::sema::declaration::resource::field
{
    auto test(const foundation::stream<tokenizer::token>& stream) -> bool;
    auto parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> std::unordered_map<std::string, ::resource::value_container>;

    auto parse_value(
        foundation::stream<tokenizer::token>& stream,
        context& ctx,
        interpreter::scope *scope,
        std::unordered_map<std::string, ::resource::value_container>& values,
        const ::resource::definition::type::field_value& value,
        const std::string& value_name
    ) -> void;
}