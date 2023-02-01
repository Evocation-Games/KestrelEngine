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

#include <vector>
#include <libKDL/lexer/lexeme.hpp>

namespace kdl::session
{
    struct type_definition;

    struct binary_template
    {
    public:

        /**
         * The binary Data Types that KDL recognises and can encode to.
         */
        enum data_type
        {
            invalid = 0,

            // Basic Integer Types
            DBYT = 0x00010000U,
            DWRD = 0x00020000U,
            DLNG = 0x00030000U,
            DQAD = 0x00040000U,
            HBYT = 0x00050000U,
            HWRD = 0x00060000U,
            HLNG = 0x00070000U,
            HQAD = 0x00080000U,

            // Aligned Integer Types
//          AWRD = 0x00090000,
//          ALNG = 0x000A0000,
//          AQAD = 0x000B0000,

            // Fill Integer Types
//          FBYT = 0x000C0000,
//          FWRD = 0x000D0000,
//          FLNG = 0x000E0000,
//          FQAD = 0x000F0000,

            // Data
            HEXD = 0x00100000U,

            // Strings
            PSTR = 0x00110000U,
//          LSTR = 0x00120000,
//          WSTR = 0x00130000,
//          ESTR = 0x00140000,
//          OSTR = 0x00150000,
            CSTR = 0x00160000U,
            Cnnn = 0x00170000U,
//          ECST = 0x00180000,
//          OCST = 0x00190000,

            // Misc
//          BOOL = 0x001A0000,
//          BBIT = 0x001B0000,
//          TNAM = 0x001C0000,
//          CHAR = 0x001D0000,
            RECT = 0x001E0000U,
//          Hnnn = 0x001F0000,
//          P0nn = 0x00200000,

            // List support
            OCNT = 0x00210000U,
//          LSTZ = 0x00220000,
            LSTE = 0x00230000U,
//          ZCNT = 0x00240000,
            LSTC = 0x00250000U,
//          LSTB = 0x00260000U,

            // Complex Custom Types
            RSRC = 0x00270000U,
            NESTED = 0x00280000U,
        };

        /**
         *
         */
        struct field
        {
            lexer::lexeme label;
            enum data_type type;
            std::vector<field> fields;
            struct {
                lexer::lexeme name;
                const type_definition *definition { nullptr };
            } nested_type;

            field(
                const lexer::lexeme& label,
                const data_type& type,
                const lexer::lexeme& nested_type = lexer::lexeme(0),
                const type_definition *nested_type_definition = nullptr
            );
        };

    public:
        explicit binary_template(const std::string& name);

        [[nodiscard]] auto name() const -> std::string;

        auto add_field(const field& field) -> void;
        auto add_field(const lexer::lexeme& label, enum data_type type, const lexer::lexeme& nested_type = lexer::lexeme(0)) -> void;

        [[nodiscard]] auto field_count() const -> std::size_t;
        [[nodiscard]] auto at(std::int32_t idx) const -> const field&;
        auto mutable_at(std::int32_t idx) -> field&;
        [[nodiscard]] auto named(const std::string& name) const -> const field&;
        [[nodiscard]] auto named(const lexer::lexeme& name) const -> const field&;
        [[nodiscard]] auto index(const std::string& name) const -> std::int32_t;
        [[nodiscard]] auto index(const lexer::lexeme& name) const -> std::int32_t;

        [[nodiscard]] auto has_field(const std::string& name) const -> bool;
        [[nodiscard]] auto has_field(const lexer::lexeme& name) const -> bool;

        [[nodiscard]] auto fields() const -> const std::vector<field>&;

        static auto data_type_for_name(const std::string& name) -> enum data_type;
        static auto data_type_base_size(enum data_type type) -> std::size_t;

    private:
        std::string m_name;
        std::vector<field> m_fields;
    };
}