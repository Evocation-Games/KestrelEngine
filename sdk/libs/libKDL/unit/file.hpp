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
#include <libKDL/tokenizer/token.hpp>
#include <libKDL/sema/context.hpp>
#include <libFoundation/system/filesystem/file.hpp>
#include <libFoundation/stream/stream.hpp>
#include <libLexer/lexeme.hpp>
#include <libResourceCore/file.hpp>

namespace kdl::unit
{
    struct file
    {
    public:
        explicit file(sema::context& ctx);
        explicit file(resource_core::file& output, sema::context& ctx);
        auto import_file(const std::string& path, const std::vector<std::string>& definitions) -> void;
        auto import_file(const foundation::filesystem::path& path, const std::vector<std::string>& definitions) -> void;

        static auto import_and_tokenize_file(const std::string& path, const std::vector<std::string>& definitions, sema::context& ctx) -> foundation::stream<kdl::tokenizer::token>;

    private:
        auto find_config_files(const std::vector<std::string>& definitions) -> void;
        auto import_config_file(const foundation::filesystem::path& path, const std::vector<std::string>& definitions) -> void;

    private:
        resource_core::file *m_output { nullptr };
        sema::context *m_context { nullptr };
    };
}