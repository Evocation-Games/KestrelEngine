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
#include <fstream>
#include <libCodeGen/spec/markup/node.hpp>
#include <libFoundation/system/filesystem/path.hpp>

namespace codegen::spec::markup
{
    struct page : public markup_node
    {
        explicit page(const std::string& title, const std::string& filename = "")
            : m_title(title), m_filename(filename.empty() ? title : filename)
        {}

        [[nodiscard]] auto filename() const -> std::string
        {
            return m_filename;
        }

        auto save(const std::shared_ptr<codegen::markup_language>& language, const foundation::filesystem::path& path) const -> void
        {
            std::ofstream file(path.string(), std::ofstream::out);
            if (file.is_open()) {
                for (const auto& line : emit(language)) {
                    file << line << std::endl;
                }
                file.flush();
                file.close();
            }
        }

    private:
        std::string m_title;
        std::string m_filename;
    };
}