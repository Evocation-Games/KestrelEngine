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

#include <libFoundation/string/split.hpp>
#include <libFoundation/string/trim.hpp>
#include <libScripting/annotations/parser.hpp>
#include <libScripting/annotations/macro.hpp>

auto test() -> void;

static inline auto sanitize_raw_annotation(const std::string& raw) -> std::string
{
    std::string out;
    for (const auto c : raw) {
        if (c == '"') {
            continue;
        }
        out += c;
    }
    return out;
}

auto scripting::annotation::parse(const std::string &annotation) -> set
{
    auto str = sanitize_raw_annotation(annotation);
    std::vector<struct attachment> out;

    auto attachments = foundation::string::split(str, "/");

    // First component by spec is the language, so we can drop it.
    attachments.erase(attachments.begin(), attachments.begin() + 1);

    for (const auto& attachment_string : attachments) {
        if (attachment_string.empty()) {
            // There is nothing here, so continue past.
            continue;
        }

        const auto components = foundation::string::split(attachment_string, "=");
        if (components.empty()) {
            continue;
        }

        std::string value(attachment_string);
        value.erase(0, components.at(0).size() + 1);

        out.emplace_back(tag_name::from(foundation::string::trim(components.at(0), true)), foundation::string::trim(value, true));
    }

    return set(out);
}
