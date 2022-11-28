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

#include "lua/annotations/annotation.hpp"
#include "lua/annotations/tag.hpp"
#include "analyzer/string_helpers.hpp"
#include "analyzer/clang.hpp"

// MARK: - Construction

luatool::lua_api::annotation::annotation(const std::string &string)
    : m_string(string)
{
    parse();
}

luatool::lua_api::annotation::annotation(CXCursor cursor)
    : m_string(clang::spelling(cursor))
{
    parse();
}

// MARK: - Parsing

auto luatool::lua_api::annotation::parse() -> void
{
    auto components = helpers::strings::split(m_string, "/");
    if (components.empty() || components.at(0) != constants::master_tag) {
        return;
    }
    m_verified = true;
    components.erase(components.begin());

    // Aggregate the tags within the annotation
    for (const auto& tag_string : components) {
        annotation_tag tag(tag_string);
        m_tags.emplace(std::pair(tag.name(), std::move(tag)));
    }
}

// MARK: - Accessors

auto luatool::lua_api::annotation::tag(enum annotation_tag::name name) const -> std::string
{
    auto it = m_tags.find(name);
    if (it == m_tags.end()) {
        return "";
    }
    return it->second.value();
}

auto luatool::lua_api::annotation::has(enum annotation_tag::name name) const -> bool
{
    return (m_tags.find(name) != m_tags.end());
}