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

#include <libKDL/unit/file.hpp>
#include <libLexer/lexer.hpp>
#include <libKDL/tokenizer/tokenizer.hpp>
#include <libKDL/sema/analyser.hpp>

// MARK: - Construction

kdl::unit::file::file(const std::string &output_path)
    : m_output_path(output_path)
{}


// MARK: - File Import

auto kdl::unit::file::import_file(const std::string &path) -> void
{
    foundation::filesystem::path fs_path(path);
    if (!fs_path.exists()) {
        // TODO: Error Correctly here
        return;
    }

    auto file = std::make_shared<foundation::filesystem::file>(fs_path);
    if (!file->exists()) {
        // TODO: Error correctly here
        return;
    }
    m_imported_files.emplace_back(file);

    // We now have a successfully imported textual file. Perform lexical
    // analysis upon it, and then pass it to the tokenizer.
    auto lexical_result = lexer::lexer(file).analyze();
    auto token_stream = tokenizer::tokenizer(lexical_result).process();

    // Now that we have a token stream, we are ready to begin semantic analysis.
    sema::analyser(token_stream).process();
}
