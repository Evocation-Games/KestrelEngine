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
#include <cstdint>

namespace renderer
{
    struct api_info
    {
        api_info() = default;
        api_info(std::string name, std::string gpu)
            : m_name(std::move(name)), m_gpu_name(std::move(gpu)), m_supported(true)
        {}

        static auto unsupported() -> api_info { return api_info(); }

        [[nodiscard]] inline auto supported() const -> bool { return m_supported; }
        [[nodiscard]] inline auto name() const -> std::string { return m_name; }
        [[nodiscard]] inline auto gpu_name() const -> std::string { return m_gpu_name; }

    private:
        bool m_supported { false };
        std::string m_name;
        std::string m_gpu_name;
    };
}