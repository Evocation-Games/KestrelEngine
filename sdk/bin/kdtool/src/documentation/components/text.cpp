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

#include "documentation/components/text.hpp"

auto kdtool::documentation::text(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>
{
    return std::make_shared<codegen::spec::markup::text>(str);
}
auto kdtool::documentation::bold(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>
{
    return std::make_shared<codegen::spec::markup::text>(str, codegen::spec::markup::text::style::bold);
}
auto kdtool::documentation::italic(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>
{
    return std::make_shared<codegen::spec::markup::text>(str, codegen::spec::markup::text::style::italic);
}
auto kdtool::documentation::strikethrough(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>
{
    return std::make_shared<codegen::spec::markup::text>(str, codegen::spec::markup::text::style::strikethrough);
}
auto kdtool::documentation::inline_code(const std::string& str) -> std::shared_ptr<codegen::spec::markup::text>
{
    return std::make_shared<codegen::spec::markup::text>(str, codegen::spec::markup::text::style::code);
}

auto kdtool::documentation::anchor(const std::string& str, const std::shared_ptr<codegen::spec::markup::page>& page, bool wants_extension) -> std::shared_ptr<codegen::spec::markup::anchor>
{
    return std::make_shared<codegen::spec::markup::anchor>(text(str), page ? page->filename() : "#", wants_extension);
}

auto kdtool::documentation::anchor(const std::shared_ptr<codegen::spec::markup::markup_node>& body, const std::shared_ptr<codegen::spec::markup::page>& page, bool wants_extension) -> std::shared_ptr<codegen::spec::markup::anchor>
{
    return std::make_shared<codegen::spec::markup::anchor>(body, page ? page->filename() : "#", wants_extension);
}