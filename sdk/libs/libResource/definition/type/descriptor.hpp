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

namespace resource::definition::type
{
    struct descriptor
    {
    public:
        descriptor() = default;

        explicit descriptor(bool is_reference);
        descriptor(bool is_reference, const std::string& name);
        descriptor(bool is_reference, const std::string& name, const std::vector<std::string>& hints);

        [[nodiscard]] auto is_reference() const -> bool;
        auto set_reference(bool ref) -> void;

        [[nodiscard]] auto has_name() const -> bool;
        [[nodiscard]] auto name() const -> std::string;
        auto set_name(const std::string& name) -> void;

        [[nodiscard]] auto has_hints() const -> bool;
        [[nodiscard]] auto hints() const -> std::vector<std::string>;

        [[nodiscard]] auto spelling() const -> std::string;

    private:
        bool m_reference { false };
        std::string m_type;
        std::vector<std::string> m_hints;
    };
}