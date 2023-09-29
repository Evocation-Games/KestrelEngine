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
#include <vector>
#include <variant>

#include <libUI/format/interface/background.hpp>
#include <libUI/format/interface/element.hpp>
#include <libUI/types/rect.hpp>
#include <libResource/reference.hpp>

namespace ui::format
{
    struct interface
    {
    public:
        enum flags : std::uint16_t {
            dialog = 0x0001,
            imgui = 0x0002,
            shows_title_bar = 0x0004,
            shows_close_button = 0x0008,
            passthrough = 0x0010,
            centered = 0x0020,
            windowed_scene = 0x0040,
            vertical_flow_layout = 0x0080,
        };

        struct format_version {
            std::uint8_t major { 1 };
            std::uint8_t minor { 0 };
            std::uint8_t fix { 0 };
        };

        interface() = default;
        explicit interface(const std::string& name);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto version() const -> struct format_version;
        [[nodiscard]] auto flags() const -> enum flags;
        [[nodiscard]] auto frame() const -> struct rect;
        [[nodiscard]] auto background() const -> struct background;
        [[nodiscard]] auto element_count() const -> std::size_t;
        [[nodiscard]] auto elements() const -> const std::vector<element>&;
        [[nodiscard]] auto element_at(std::int32_t idx) const -> element;

        auto set_name(const std::string& name) -> void;
        auto set_version(std::uint8_t major, std::uint8_t minor, std::uint8_t fix) -> void;
        auto set_flags(std::uint16_t flags) -> void;
        auto set_flags(enum flags flags) -> void;
        auto add_flag(enum flags flag) -> void;
        auto set_frame(const struct rect& frame) -> void;
        auto set_frame(std::int16_t x, std::int16_t y, std::int16_t width, std::int16_t height) -> void;
        auto set_frame_size(std::int16_t width, std::int16_t height) -> void;
        auto set_background(const resource::reference& fill) -> void;
        auto set_background(const resource::reference& top, const resource::reference& fill, const resource::reference& bottom) -> void;
        auto set_background(const struct background& background) -> void;
        auto add_element(const struct element& element) -> void;

    private:
        std::string m_name;
        struct format_version m_version;
        std::uint16_t m_flags { 0 };
        struct rect m_frame { 0, 0, 0, 0 };
        struct background m_background;
        std::vector<element> m_elements;
    };

}