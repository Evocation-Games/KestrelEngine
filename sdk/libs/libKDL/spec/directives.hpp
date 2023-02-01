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

namespace kdl::spec::directives
{
    static constexpr const char *project_directive = "project";
    static constexpr const char *author_directive = "author";
    static constexpr const char *version_directive = "version";
    static constexpr const char *license_directive = "license";
    static constexpr const char *copyright_directive = "copyright";
    static constexpr const char *email_directive = "email";
    static constexpr const char *website_directive = "website";
    static constexpr const char *out_directive = "out";
    static constexpr const char *format_directive = "format";
    static constexpr const char *import_directive = "import";


    static auto vector() -> std::vector<std::string>
    {
        return {
            project_directive, author_directive, version_directive, license_directive,
            copyright_directive, email_directive, website_directive, out_directive,
            format_directive, import_directive
        };
    }
}