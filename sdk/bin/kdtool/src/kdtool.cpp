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
#include "project/project.hpp"
#include "builder/api/api.hpp"
#include "builder/documentation/documentation.hpp"
#include <libCodeGen/languages/markup/markdown.hpp>
#include <libCodeGen/languages/markup/html.hpp>
#include <libCodeGen/languages/procedural/cxx.hpp>

auto main(const std::int32_t argc, const char *argv[]) -> std::int32_t
{
    auto project = std::make_shared<kdtool::project::index>();

    struct {
        std::string path;
        bool cxx { false };
    } lua_api;

    struct {
        std::string path;
        std::string ref_root;
        std::string title;
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
            else if (arg == "-api") {
                lua_api.path = argv[++n];
            }
            else if (arg == "-documentation" || arg == "-docs") {
                documentation.path = argv[++n];
            }
            else if (arg == "-root") {
                documentation.ref_root = argv[++n];
            }
            else if (arg == "-md") {
                documentation.markdown = true;
            }
            else if (arg == "-html") {
                documentation.html = true;
            }
            else if (arg == "-cxx") {
                lua_api.cxx = true;
            }
            else if (arg == "-v" || arg == "--verbose") {
                project->set_verbose_mode(true);
            }
            else if (arg == "-doctitle") {
                documentation.title = argv[++n];
                project->set_title(documentation.title);
            }
        }
        else {
            project->add_translation_unit(argv[n]);
        }
    }

    if (!lua_api.path.empty()) {
        if (lua_api.cxx) {
            if (project->verbose_logging()) {
                std::cout << "Build C++ API to: " << lua_api.path << std::endl;
            }
            auto api = std::make_shared<kdtool::builder::api<codegen::language::cxx>>(project, lua_api.path);
            api->build();
        }
    }

    if (!documentation.path.empty()) {
        foundation::filesystem::path path(documentation.path);
        foundation::filesystem::path root(documentation.ref_root);

        if (documentation.markdown) {
            if (project->verbose_logging()) {
                std::cout << "Build Markdown Documentation to: " << documentation.path << std::endl;
            }
            auto api = std::make_shared<kdtool::builder::documentation<codegen::language::markdown>>(project, path, root);
            api->build();
        }

        if (documentation.html) {
            if (project->verbose_logging()) {
                std::cout << "Build HTML Documentation to: " << documentation.path << std::endl;
            }
            auto api = std::make_shared<kdtool::builder::documentation<codegen::language::html>>(project, path, root);
            api->build();
        }
    }

    return 0;
}
