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
#include <libCodeGen/spec/markup/text.hpp>
#include <libCodeGen/spec/markup/anchor.hpp>
#include <libCodeGen/spec/markup/page.hpp>

namespace kdtool::documentation
{
    auto text(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>;
    auto bold(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>;
    auto italic(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>;
    auto strikethrough(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>;
    auto inline_code(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>;

    auto anchor(const std::string& str, const std::shared_ptr<codegen::spec::markup::page>& page, bool wants_extension) -> std::shared_ptr<codegen::spec::markup::anchor>;
    auto anchor(const std::shared_ptr<codegen::spec::markup::markup_node>& body, const std::shared_ptr<codegen::spec::markup::page>& page, bool wants_extension) -> std::shared_ptr<codegen::spec::markup::anchor>;
}