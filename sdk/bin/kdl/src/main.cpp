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
#include <libLexer/exception/unrecognised_character_exception.hpp>
#include <libKDL/diagnostic/diagnostic.hpp>

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
    catch (kdl::diagnostic& diagnostic) {
        diagnostic.report();
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