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
#include <libResourceCore/file.hpp>

#include <iostream>
#include <libKDL/exception/duplicate_type_definition_exception.hpp>
#include <libKDL/exception/expression_runtime_exception.hpp>
#include <libKDL/exception/incompatible_conversion_exception.hpp>
#include <libKDL/exception/incorrect_arguments_exception.hpp>
#include <libKDL/exception/invalid_attribute_exception.hpp>
#include <libKDL/exception/module_exception.hpp>
#include <libKDL/exception/resource_already_exists_exception.hpp>
#include <libKDL/exception/type_mismatch_exception.hpp>
#include <libKDL/exception/unexpected_token_exception.hpp>
#include <libKDL/exception/unknown_binary_conversion_format_exception.hpp>
#include <libKDL/exception/unknown_field_exception.hpp>
#include <libKDL/exception/unknown_value_exception.hpp>
#include <libKDL/exception/unrecognised_binary_template_field_exception.hpp>
#include <libKDL/exception/unrecognised_function_exception.hpp>
#include <libKDL/exception/unrecognised_module_format_exception.hpp>
#include <libKDL/exception/unrecognised_type_definition_exception.hpp>
#include <libKDL/exception/unrecognised_variable_exception.hpp>
#include <libLexer/exception/unrecognised_character_exception.hpp>

auto report_exception(const std::string& reason, const lexer::lexeme& lx) -> void;
auto report_exception(const std::string& reason) -> void;

auto main(std::int32_t argc, const char **argv) -> std::int32_t
{
    std::string output_name = "result.kdat";
    resource_core::file output;
    kdl::sema::context context;
    kdl::unit::file session(output, context);
    std::vector<std::string> definitions;
    auto format = resource_core::file::format::extended;

    // Parse out the command that was used to invoke the assembler.
    try {
        for (std::int32_t i = 1; i < argc; ++i) {
            std::string option(argv[i]);
            if (option == "-f" || option == "--format") {
                std::string f(argv[++i]);
                if (f == "extended" || f == "kestrel") {
                    format = resource_core::file::format::extended;
                }
                else if (f == "classic" || f == "rsrc") {
                    format = resource_core::file::format::classic;
                }
                else if (f == "rez") {
                    format = resource_core::file::format::rez;
                }
                else {
                    throw std::runtime_error("");
                }
            }
            else if (option == "-D") {
                // Create a definition/flag for use in condition attributes.
                definitions.emplace_back(argv[++i]);
            }
            else if (option == "-o" || option == "--output") {
                output_name = argv[++i];
            }
            else {
                std::vector<std::string> file_definitions = definitions;
                switch (format) {
                    case resource_core::file::format::extended: {
                        file_definitions.emplace_back("extended");
                        break;
                    }
                    case resource_core::file::format::classic: {
                        file_definitions.emplace_back("classic");
                        break;
                    }
                    case resource_core::file::format::rez: {
                        file_definitions.emplace_back("rez");
                        break;
                    }
                }
                session.import_file(argv[i], file_definitions);
            }
        }

        output.write(output_name, format);
    }
    catch (kdl::duplicate_type_definition_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::expression_runtime_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::incompatible_conversion_exception& e) {
        report_exception(e.reason());
    }
    catch (kdl::incorrect_arguments_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::invalid_attribute_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::module_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::resource_already_exists_exception& e) {
        report_exception(e.reason());
    }
    catch (kdl::type_mismatch_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unexpected_token_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unknown_binary_conversion_format_exception& e) {
        report_exception(e.reason());
    }
    catch (kdl::unknown_field_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unknown_value_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unrecognised_binary_template_field_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unrecognised_function_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unrecognised_module_format_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unrecognised_type_definition_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (kdl::unrecognised_variable_exception& e) {
        report_exception(e.reason(), e.lexeme());
    }
    catch (lexer::unrecognised_character_exception& e) {
        report_exception("Unexpected character exception.", e.lexeme());
    }

    return 0;
}

// MARK: - Exception Reporting

auto report_exception(const std::string& reason, const lexer::lexeme& lx) -> void
{
    std::cerr << lx.location() << " - " << lx.source_directory().string() << std::endl;
    std::cerr << "    " << reason << std::endl;
    std::cerr << "    '" << lx.text() << "'" << std::endl;
}

auto report_exception(const std::string& reason) -> void
{
    std::cerr << reason << std::endl;
}