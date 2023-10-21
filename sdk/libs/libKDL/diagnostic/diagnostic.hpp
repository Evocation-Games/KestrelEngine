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
#include <stdexcept>
#include <libLexer/lexer.hpp>
#include <libKDL/tokenizer/token.hpp>
#include <libInterpreter/token/token.hpp>

namespace kdl
{
    /**
     * A diagnostic is responsible for encapsulating information about an error/issue that
     * has occurred, and producing a report for it.
     */
    struct diagnostic : public std::exception
    {
    public:
        struct reason
        {
            static auto constexpr KDL000 = "[KDL000] Fatal error occurred at '%CXX'";
            static auto constexpr KDL001 = "[KDL001] Expected an identifier for command request/attribute name";
            static auto constexpr KDL002 = "[KDL002] First argument of a component should be a type name identifier";
            static auto constexpr KDL003 = "[KDL003] Second argument of a component should be an initial resource reference";
            static auto constexpr KDL004 = "[KDL004] Unable to interpret component type";
            static auto constexpr KDL005 = "[KDL005] Expected an identifier for component name";
            static auto constexpr KDL006 = "[KDL006] Unexpected token encountered";
            static auto constexpr KDL007 = "[KDL007] Invalid token sequence encountered";
            static auto constexpr KDL008 = "[KDL008] Illegal file reference provided";
            static auto constexpr KDL009 = "[KDL009] Unrecognized type name";
            static auto constexpr KDL010 = "[KDL010] Expected an identifier for type name";
            static auto constexpr KDL011 = "[KDL011] Missing both a `source` and `result` format";
            static auto constexpr KDL012 = "[KDL012] Unrecognised value type";
            static auto constexpr KDL013 = "[KDL013] Unknown field name";
            static auto constexpr KDL014 = "[KDL014] Field repeated too many times";
            static auto constexpr KDL015 = "[KDL015] Field value not found in type template";
            static auto constexpr KDL016 = "[KDL016] Too many values provided for field";
            static auto constexpr KDL017 = "[KDL017] Illegal value encountered";
            static auto constexpr KDL018 = "[KDL018] Bad value expression";
            static auto constexpr KDL019 = "[KDL019] Import file not found";
            static auto constexpr KDL020 = "[KDL020] Inline resource declarations must not be `override` or `duplicate`";
            static auto constexpr KDL021 = "[KDL021] Value expression results in illegal type";
            static auto constexpr KDL022 = "[KDL022] Unrecognized module name";
            static auto constexpr KDL023 = "[KDL023] Unrecognized directive name";
            static auto constexpr KDL024 = "[KDL024] Bad directive format";
            static auto constexpr KDL025 = "[KDL025] Expected an identifier for @function name";
            static auto constexpr KDL026 = "[KDL026] @out directive only accepts strings";
            static auto constexpr KDL027 = "[KDL027] Expected an identifier or variable for @var or @const";
            static auto constexpr KDL028 = "[KDL028] Unrecognized scene/dialog member encountered";
            static auto constexpr KDL029 = "[KDL029] Unrecognized scene element member encountered";
            static auto constexpr KDL030 = "[KDL030] Value expression failed";
            static auto constexpr KDL031 = "[KDL031] Unrecognized scene action";
            static auto constexpr KDL032 = "[KDL032] Scene value `Get` function expects a resource reference as argument #1";
            static auto constexpr KDL033_1 = "[KDL033:1] Unexpected token encountered in expression";
            static auto constexpr KDL033_2 = "[KDL033:2] Unexpected token encountered in expression";
            static auto constexpr KDL035_1 = "[KDL035:1] Unrecognized variable";
            static auto constexpr KDL035_2 = "[KDL035:2] Unrecognized variable";
            static auto constexpr KDL035_3 = "[KDL035:3] Unrecognized variable";
            static auto constexpr KDL035_4 = "[KDL035:4] Unrecognized variable";
            static auto constexpr KDL034 = "[KDL034] @MergeBitmask must only be used when all values are of type `Bitmask`";
            static auto constexpr KDL036 = "[KDL036] Auto field synthesis requires field name to match field in template";
            static auto constexpr KDL037 = "[KDL037] Expected variable name";
            static auto constexpr KDL038 = "[KDL038] `OCNT` field must be followed by an `LSTC` field";
            static auto constexpr KDL039 = "[KDL039] Unrecognized binary field type";
            static auto constexpr KDL040 = "[KDL040] Failed to create resource type";
            static auto constexpr KDL041 = "[KDL041] Failed to define module";
            static auto constexpr KDL042 = "[KDL042] Unrecognized keyword";
            static auto constexpr KDL043 = "[KDL043] Illegal arguments specified for decorator";
            static auto constexpr KDL044 = "[KDL044] Arguments must be delimited by a comma (,)";
            static auto constexpr KDL045 = "[KDL045] Missing nested resource type name";
            static auto constexpr KDL046 = "[KDL046] Missing ByteCode language identifier";
            static auto constexpr KDL047 = "[KDL047] Could not open source file";
            static auto constexpr KDL048 = "[KDL048] Lua compilation error";

            static auto constexpr KDL900 = "[KDL900] Feature not implemented yet...";
        };

    public:
        diagnostic() = default;
        explicit diagnostic(std::string reason, const std::vector<std::string>& info = {});
        diagnostic(const lexer::lexeme& lexeme, std::string reason, const std::vector<std::string>& info = {});
        diagnostic(const tokenizer::token& tk, std::string reason, const std::vector<std::string>& info = {});
        diagnostic(const interpreter::token& tk, std::string reason, const std::vector<std::string>& info = {});

        auto report() -> void;

    private:
        [[nodiscard]] auto location() const -> std::string;
        [[nodiscard]] auto reason() const -> std::string;
        [[nodiscard]] auto source_code() const -> std::string;
        [[nodiscard]] auto additional_info() const -> std::string;

    private:
        std::string m_file {};
        std::size_t m_pos {0};
        std::size_t m_offset {0};
        std::size_t m_line {0};
        std::string m_reason {};
        std::string m_source_line {};
        std::vector<std::string> m_info {};
    };
}