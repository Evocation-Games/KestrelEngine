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
#include <variant>
#include <libGraphite/data/data.hpp>
#include <libResource/reference.hpp>

namespace ui::format
{
    struct action
    {
    public:
        enum class type : std::uint16_t {
            no_action = 0x0000,
            push_named_scene = 0x0001,
            push_identified_scene = 0x0002,
            named_function = 0x0003,
            lua_script_reference = 0x0004,
            lua_script_source = 0x0005,
            lua_script_bytecode = 0x0006
        };

        action() = default;
        explicit action(const std::string& value, enum type type);
        explicit action(const resource::reference& value, enum type type);
        explicit action(const resource::reference& reference);
        explicit action(const graphite::data::block& bytecode);

        [[nodiscard]] auto type() const -> enum type;

        template<typename T>
        [[nodiscard]] auto value() const -> T
        {
            return std::get<T>(m_value);
        }

        auto set_type(const enum type& type) -> void;
        auto set_value(const std::string& value) -> void;
        auto set_value(const resource::reference& value) -> void;
        auto set_value(const graphite::data::block& value) -> void;

    private:
        enum type m_type { type::no_action };
        std::variant<std::string, resource::reference, graphite::data::block> m_value;
    };
}