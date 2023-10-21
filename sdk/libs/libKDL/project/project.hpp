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
#include <vector>
#include <libFoundation/system/filesystem/path.hpp>

namespace kdl
{
    struct project
    {
    public:
        project(const std::string& path, const std::string& type);

        auto add_author(const std::string& name) -> void;
        auto set_version(const std::string& name) -> void;
        auto set_copyright(const std::string& name) -> void;
        auto set_license(const std::string& name) -> void;

        auto add_basic_scene(const std::string& name) -> void;
        auto add_basic_dialog(const std::string& name) -> void;

        auto build() -> void;

    private:
        auto build_project_file() -> void;
        auto build_scene_file(const std::string& name) -> void;
        auto build_dialog_file(const std::string& name) -> void;
        auto build_main_script() -> void;
        auto build_scene_script(const std::string& name) -> void;
        auto build_dialog_script(const std::string& name) -> void;

        auto export_file(const foundation::filesystem::path& path, const std::string& contents) -> void;

    private:
        foundation::filesystem::path m_path;
        foundation::filesystem::path m_scripts_path;
        foundation::filesystem::path m_scenes_path;
        std::string m_type;
        std::string m_name;
        std::vector<std::string> m_author;
        std::string m_version { "1.0" };
        std::string m_license { "MIT" };
        std::string m_copyright { "Your Copyright Here" };
        std::string m_default_scene;
        std::vector<std::string> m_scenes;
        std::vector<std::string> m_dialogs;
    };
}