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

#include <iostream>
#include "lua/project.hpp"


auto main(const std::int32_t argc, const char *argv[]) -> std::int32_t
{
    luatool::lua_api::project project;
    std::string lua_api_path;
    std::string md_documentation_path;

    for (auto n = 1; n < argc; ++n) {
        std::string arg(argv[n]);
        if (arg[0] == '-') {
            if (arg == "-I") {
                project.add_include_directory(argv[++n]);
            }
            else if (arg == "-lua") {
                lua_api_path = argv[++n];
            }
            else if (arg == "-md") {
                md_documentation_path = argv[++n];
            }
        }
        else {
            project.add_translation_unit(argv[n]);
        }
    }

    if (!lua_api_path.empty()) {
        project.generate_cxx_source(lua_api_path);
    }

    if (!md_documentation_path.empty()) {
        project.generate_markdown_documentation(md_documentation_path);
    }

    return 0;
}
