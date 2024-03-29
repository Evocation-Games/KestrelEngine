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

#include <iostream>
#include <stdexcept>
#include <libKDL/unit/file.hpp>
#include <libLexer/lexer.hpp>
#include <libKDL/tokenizer/tokenizer.hpp>
#include <libKDL/sema/analyser.hpp>
#include <libKDL/assembler/encoder.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

// MARK: - Construction

kdl::unit::file::file(sema::context& ctx)
    : m_context(&ctx)
{
    find_config_files({});
}

kdl::unit::file::file(resource_core::file& output, sema::context& ctx)
    : m_output(&output), m_context(&ctx)
{
    find_config_files({});
}

// MARK: - Config Files

auto kdl::unit::file::find_config_files(const std::vector<std::string>& definitions) -> void
{
    // We need to import the config files in the correct order, so that
    // values are appropriately overridden as required.

    // 1. User Configuration File: ~/.kestrel/config.kdl
    auto user_configuration = foundation::filesystem::path::configuration_directory().appending_path_component("config.kdl");
    import_config_file(user_configuration, definitions);

    // 2. Homebrew Configuration Files
    auto homebrew_opt = foundation::filesystem::path("/opt/homebrew/var/kestrel");
    auto homebrew_usr = foundation::filesystem::path("/usr/local/var/kestrel");

    homebrew_opt.each_child([&] (const auto& child) {
        if (child.has_extension("kdl") || child.has_extension("kdmodule")) {
            import_config_file(child, definitions);
        }
    });

    homebrew_usr.each_child([&] (const auto& child) {
        if (child.has_extension("kdl") || child.has_extension("kdmodule")) {
            import_config_file(child, definitions);
        }
    });
}

auto kdl::unit::file::import_config_file(const foundation::filesystem::path& path, const std::vector<std::string>& definitions) -> void
{
    if (path.exists()) {
        import_file(path, definitions);
    }
}

// MARK: - Helpers

static inline auto setup_attributes(const resource::reference& reference) -> std::unordered_map<std::string, std::string>
{
    std::unordered_map<std::string, std::string> attributes;
    if (reference.has_container()) {
        attributes.emplace("container", reference.container_name());
    }
    return attributes;
}

// MARK: - File Import

auto kdl::unit::file::import_and_tokenize_file(const std::string &path, const std::vector<std::string> &definitions, sema::context& ctx) -> foundation::stream<kdl::tokenizer::token>
{
    foundation::filesystem::path fs_path(path);
    if (!fs_path.exists()) {
        return {};
    }

    auto file = std::make_shared<foundation::filesystem::file>(fs_path);
    if (!file->exists()) {
        throw diagnostic(diagnostic::reason::KDL047, { path });
    }
    ctx.files.emplace_back(file);

    // We now have a successfully imported textual file. Perform lexical
    // analysis upon it, and then pass it to the tokenizer.
    auto lexical_result = lexer::lexer(file).analyze();
    return tokenizer::tokenizer(lexical_result).process();
}

auto kdl::unit::file::import_file(const std::string &path, const std::vector<std::string>& definitions) -> void
{
    import_file(foundation::filesystem::path(path), definitions);
}

auto kdl::unit::file::import_file(const foundation::filesystem::path& path, const std::vector<std::string>& definitions) -> void
{
    auto file = std::make_shared<foundation::filesystem::file>(path);
    if (!file->exists()) {
        throw diagnostic(diagnostic::reason::KDL047, { path.string() });
    }
    m_context->files.emplace_back(file);

    // We now have a successfully imported textual file. Perform lexical
    // analysis upon it, and then pass it to the tokenizer.
    auto lexical_result = lexer::lexer(file).analyze();
    auto token_stream = tokenizer::tokenizer(lexical_result).process();

    // Now that we have a token stream, we are ready to begin semantic analysis
    sema::analyser(token_stream, definitions).process(*m_context);

    // Using the result of the analysis, we now need to encode each of the resources
    // that have been generated.
    if (!m_output) {
        // There is no output file, therefore we do not need to generate resources.
        return;
    }

    for (const auto& instance : m_context->resources) {
        // Make sure the resource reference has a type name. If there is no type name,
        // then this can not be considered valid.
        const auto& ref = instance.reference();
        if (!ref.has_type_name()) {
            continue;
        }

        const auto *type = m_context->type_named(ref.type_name());
        if (!type) {
            throw diagnostic(diagnostic::reason::KDL009, { ref.type_name() });
        }

        // Setup an encoder and generate the instance data and add it to the resource file.
        if (instance.values().empty() && instance.data().size() > 0) {
            m_output->add_resource(type->code(), ref.id(), instance.name(), instance.data(), setup_attributes(ref));
        }
        else {
            assembler::encoder encoder(instance, type, definitions);
            const auto& data = encoder.encode(m_context);
            m_output->add_resource(type->code(), ref.id(), instance.name(), data, setup_attributes(ref));
        }
    }
}
