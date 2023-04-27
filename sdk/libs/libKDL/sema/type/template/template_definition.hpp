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

#include <libFoundation/stream/stream.hpp>
#include <libKDL/tokenizer/token.hpp>
#include <libResource/definition/template/instance.hpp>
#include <libResource/definition/template/type.hpp>
#include <libKDL/sema/context.hpp>

namespace kdl::sema::type_definition::template_definition
{
    auto test(const foundation::stream<tokenizer::token>& stream) -> bool;
    auto parse(foundation::stream<tokenizer::token>& stream, context& ctx) -> resource::definition::binary_template::instance;
    auto parse_field(foundation::stream<tokenizer::token>& stream, context& ctx) -> resource::definition::binary_template::field;

    auto test_binary_type(const foundation::stream<tokenizer::token>& stream) -> bool;
    auto binary_type(const tokenizer::token& token, context& ctx) -> resource::definition::binary_template::type;
}