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

#include <libUI/format/interface/value.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/graphics/types/color.hpp>

namespace kestrel::ui
{
    struct value
    {
    public:
        value() = default;
        explicit value(const ::ui::format::value& value);
        explicit value(const std::string& value);

        [[nodiscard]] auto count() const -> std::size_t;
        [[nodiscard]] auto string(std::int64_t idx) const -> std::string;
        [[nodiscard]] auto integer(std::int64_t idx) const -> std::int64_t;
        [[nodiscard]] auto unsigned_integer(std::int64_t idx) const -> std::uint64_t;
        [[nodiscard]] auto boolean(std::int64_t idx) const -> bool;
        [[nodiscard]] auto descriptor(std::int64_t idx) const -> resource::descriptor::lua_reference;
        [[nodiscard]] auto color(std::int64_t idx) const -> graphics::color::lua_reference;

    private:
        struct boxed
        {
            enum type : std::uint8_t {
                string, integer, unsigned_integer, boolean, descriptor, color
            };
            std::variant<std::string, std::int64_t, std::uint64_t, bool, resource::descriptor::lua_reference, graphics::color::lua_reference> stored_value;

            boxed() : stored_value(0LL) {}
            explicit boxed(const std::string& str) : stored_value(str) {}
            explicit boxed(const std::int64_t value) : stored_value(value) {}
            explicit boxed(const std::uint64_t value) : stored_value(value) {}
            explicit boxed(bool value) : stored_value(value) {}
            explicit boxed(const resource::descriptor::lua_reference& value) : stored_value(value) {}
            explicit boxed(const graphics::color::lua_reference& value) : stored_value(value) {}
        };

        std::vector<boxed> m_values;
        bool m_list { false };

        auto load_values(const ::ui::format::value& value) -> void;
        auto add_value(const std::string& value) -> void;
        auto add_value(std::int64_t value) -> void;
        auto add_value(std::uint64_t value) -> void;
        auto add_value(bool value) -> void;
        auto add_value(const resource::descriptor& value) -> void;
        auto add_value(const graphics::color& color) -> void;


    };
}