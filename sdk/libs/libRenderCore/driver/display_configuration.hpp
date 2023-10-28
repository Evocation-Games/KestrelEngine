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

namespace renderer
{
    struct display_configuration
    {
        enum graphics_layer_preference { DEFAULT = 0, OPENGL = 1, METAL = 2, VULKAN = 3, DIRECTX = 4 };

        [[nodiscard]] inline auto output_width() const -> std::uint32_t { return m_resolution.width; }
        [[nodiscard]] inline auto output_height() const -> std::uint32_t { return m_resolution.height; }

        [[nodiscard]] inline auto swap_chain_count() const -> std::size_t { return m_gl.swap_chain_count; }
        [[nodiscard]] inline auto vsync() const -> bool { return m_gl.vsync; }
        [[nodiscard]] inline auto refresh_rate() const -> std::uint64_t { return m_screen.refresh_rate; }

        [[nodiscard]] inline auto gl_preference() const -> graphics_layer_preference { return m_gl.preference; };

    private:
        struct {
            std::uint32_t width { 1280 };
            std::uint32_t height { 720 };
            float scale { 1.f };
        } m_resolution;

        struct {
            std::uint32_t width { 1280 };
            std::uint32_t height { 720 };
            float dpi { 1.f };
            std::uint64_t refresh_rate { 60 };
        } m_screen;

        struct {
            enum graphics_layer_preference preference { OPENGL };
            bool vsync { false };
            std::size_t swap_chain_count { 3 };
        } m_gl;
    };
}