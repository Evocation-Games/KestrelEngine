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

#include "project/project.hpp"
#include "documentation/builder.hpp"
#include <libCodeGen/languages/markup/markdown.hpp>
#include <libCodeGen/languages/markup/html.hpp>

auto main(const std::int32_t argc, const char *argv[]) -> std::int32_t
{
    auto project = std::make_shared<kdtool::project>();

    struct {
        std::string path;
        bool generate { false };
    } lua_api;

    struct {
        std::string path;
        bool markdown { false };
        bool html { false };
    } documentation;

    for (auto n = 1; n < argc; ++n) {
        std::string arg(argv[n]);
        if (arg[0] == '-') {
            if (arg == "-I") {
                std::string path(argv[++n]);
                project->add_include_path(path);
            }
            else if (arg == "-lua") {
                lua_api.path = argv[++n];
                lua_api.generate = true;
            }
            else if (arg == "-documentation") {
                documentation.path = argv[++n];
            }
            else if (arg == "-md") {
                documentation.markdown = true;
            }
            else if (arg == "-html") {
                documentation.html = true;
            }
        }
        else {
            project->add_translation_unit(argv[n]);
        }
    }

    if (!lua_api.generate) {
//        project.generate_cxx_source(lua_api_path);
    }

    if (!documentation.path.empty()) {
        auto docs = std::make_shared<kdtool::documentation::builder>(project, documentation.path);
        docs->build();

        if (documentation.markdown) {
            docs->export_as(std::make_shared<codegen::markdown>());
        }

        if (documentation.html) {
            docs->export_as(std::make_shared<codegen::html>());
        }
    }

    return 0;
}
