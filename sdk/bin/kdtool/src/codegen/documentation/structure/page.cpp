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

#include <dirent.h>
#include <sys/stat.h>
#include <stdexcept>
#include <fstream>
#include "codegen/documentation/structure/page.hpp"
#include "codegen/documentation/structure/heading.hpp"

// MARK: - Construction

kdtool::codegen::documentation::page::page(const std::string &title, const std::string& filename)
    : common::emit_node(), m_title(title), m_filename(filename.empty() ? title : filename)
{
    add_child<heading>(title, 1);
}

// MARK: - Accessors

auto kdtool::codegen::documentation::page::filename() const -> std::string
{
    return m_filename + ".md";
}

auto kdtool::codegen::documentation::page::basename() const -> std::string
{
    auto path = filename();
    return path.substr(path.find_last_of('/') + 1);
}

// MARK: - Render

auto kdtool::codegen::documentation::page::render(const std::string& path) -> void
{
    // Export the page content.
    std::string filepath = path;
    if (!filepath.ends_with('/')) {
        filepath += "/";
    }
    filepath += filename();

    // Try and create the appropriate directory for the export.
    create_directory(parent(filepath));

    std::ofstream file(filepath, std::ofstream::out);
    for (const auto& line : emit()) {
        file << line << std::endl;
    }
    file.close();
}

// MARK: - Directories

auto kdtool::codegen::documentation::page::directory_exists(const std::string &path) -> bool
{
    DIR *dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return true;
    }
    else {
        return false;
    }
}

auto kdtool::codegen::documentation::page::parent(const std::string &path, const char delimiter) -> std::string
{
    return path.substr(0, path.find_last_of(delimiter));
}

auto kdtool::codegen::documentation::page::create_directory(const std::string &path, bool intermediates) -> void
{
    if (directory_exists(path)) {
        return;
    }

    auto parent_directory_path = parent(path);
    if (!directory_exists(parent_directory_path)) {
        if (intermediates) {
            create_directory(parent_directory_path, intermediates);
        }
        else {
            throw std::runtime_error("Could not create directory: " + path);
        }
    }

#if TARGET_WINDOWS
    if (mkdir(path.c_str())) {
        throw std::runtime_error("Failed to create directory: " + path);
    }
#else
    if (mkdir(path.c_str(), 0755)) {
        throw std::runtime_error("Failed to create directory: " + path);
    }
#endif

}