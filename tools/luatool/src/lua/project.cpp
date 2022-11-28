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

#include "lua/project.hpp"
#include "codegen/documentation/documentation.hpp"
#include "codegen/cxx/generator.hpp"

// MARK: - Translation Unit Management

auto luatool::lua_api::project::add_translation_unit(const std::string &path) -> void
{
    auto tu = new ast::translation_unit(path, true, m_include_dirs);
    tu->process();
    m_units.emplace_back(tu);
}

// MARK: - Include Dirs

auto luatool::lua_api::project::add_include_directory(const std::string &path) -> void
{
    m_include_dirs.emplace_back(path);
}

// MARK: - Actions

auto luatool::lua_api::project::generate_markdown_documentation(const std::string& path) -> void
{
    codegen::documentation::generator markdown;
    for (auto tu : m_units) {
        markdown.add_translation_unit(tu);
    }
    markdown.compile(path);
}

auto luatool::lua_api::project::generate_cxx_source(const std::string& path) -> void
{
    codegen::cxx::generator api;
    for (auto tu : m_units) {
        api.add_translation_unit(tu);
    }
    api.compile(path);
}