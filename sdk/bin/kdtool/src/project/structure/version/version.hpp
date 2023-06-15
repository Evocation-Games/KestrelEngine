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

#include <cstdint>
#include <string>
#include <libFoundation/string/split.hpp>
#include <libFoundation/string/joined.hpp>

namespace kdtool::project::structure
{
    struct version
    {
        explicit version(const std::string& str)
        {
            const auto components = foundation::string::split(str);
            if (components.size() >= 1) {
                m_major = std::stoi(components[0]);

                if (components.size() >= 2) {
                    m_minor = std::stoi(components[1]);

                    if (components.size() >= 3) {
                        m_fix = std::stoi(components[2]);
                    }
                }
            }
        }

        [[nodiscard]] auto string() const -> std::string
        {
            return foundation::string::joined<std::uint16_t, std::uint16_t, std::uint16_t>(".", m_major, m_minor, m_fix);
        }

    private:
        std::uint16_t m_major;
        std::uint16_t m_minor;
        std::uint16_t m_fix;
    };
}