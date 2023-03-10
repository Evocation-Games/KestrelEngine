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

namespace kdl::tokenizer
{
    enum token_type
    {
        none, error,

        // Keywords
        type_keyword, declare_keyword, new_keyword, override_keyword, duplicate_keyword,
        field_keyword, template_keyword, assert_keyword, component_keyword, files_keyword,
        types_keyword, repeatable_keyword, as_keyword, constructor_keyword, import_keyword,
        scene_keyword,

        // Directives
        project_directive, author_directive, version_directive, license_directive,
        copyright_directive, email_directive, website_directive, out_directive,
        format_directive, import_directive,

        // Decorators
        builtin_decorator, no_declaration_decorator, synthesize_decorator, constexpression_decorator,
        api_decorator, example_decorator, deprecated_decorator, condition_decorator,

        // Binary Types
        DBYT, DWRD, DLNG, DQWD, HBYT, HWRD, HLNG, HQWD,
        RECT, PSTR, CSTR, Cnnn, LSTR, OSTR, CHAR, HEXD,
        OCNT, LSTC, LSTE, BBIT, BOOL, RSRC, NESTED,

        // High Level Types
        integer_type, string_type, bitmask_type, file_type, image_type, named_reference_type,
        unnamed_reference_type, data_type, image_set_type, sound_type,

        // Data Values
        string, integer, percentage, reference, data,

        // Fundamental Types
        identifier, directive, decorator, variable, expression, structure, identifier_path,

        // Operators
        l_paren, r_paren, l_angle, r_angle, l_brace, r_brace, l_bracket, r_bracket,
        comma, dot, pipe, slash, amp, colon, semi, plus, minus, star, equals, carat,
        left_shift, right_shift, tilde, at, hash, percent, exclaim, question, dollar
    };
}