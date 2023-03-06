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
#include <libKDL/assembler/encoder.hpp>

// MARK: - Construction

kdl::unit::file::file(graphite::rsrc::file& output)
    : m_output(&output)
{}

// MARK: - Helpers

static inline auto setup_attributes(const resource::reference& reference) -> std::unordered_map<std::string, std::string>
{
    std::unordered_map<std::string, std::string> attributes;
    if (reference.has_container()) {
        attributes.emplace("ns", reference.container_name());
    }
    return attributes;
}

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

    // Now that we have a token stream, we are ready to begin semantic analysis
    auto analysis_context = sema::analyser(token_stream).process();

    // Using the result of the analysis, we now need to encode each of the resources
    // that have been generated.
    if (!m_output) {
        // TODO: Record an error that there is no output file for the resources to be written to.
        return;
    }

    for (const auto& instance : analysis_context.resources) {
        // Make sure the resource reference has a type name. If there is no type name,
        // then this can not be considered valid.
        const auto& ref = instance.reference();
        if (!ref.has_type_name()) {
            continue;
        }

        const auto *type = analysis_context.type_named(ref.type_name());
        if (!type) {
            // TODO: Report an error regarding the missing type.
            throw std::runtime_error("");
        }

        // Setup an encoder and generate the instance data and add it to the resource file.
        assembler::encoder encoder(instance, type);
        m_output->add_resource(type->code(), ref.id(), instance.name(), encoder.encode(), setup_attributes(ref));
    }
}
