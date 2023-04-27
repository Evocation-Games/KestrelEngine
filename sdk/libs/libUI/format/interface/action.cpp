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

#include <libUI/format/interface/action.hpp>

// MARK: - Construction

ui::format::action::action(const graphite::data::block &bytecode)
    : m_type(type::lua_script_bytecode), m_value(bytecode)
{}

ui::format::action::action(const resource::reference& reference)
    : m_type(type::lua_script_reference), m_value(reference)
{}

ui::format::action::action(const std::string &value, enum action::type type)
    : m_type(type), m_value(value)
{}

ui::format::action::action(const resource::reference& reference, enum action::type type)
    : m_type(type), m_value(reference)
{}

// MARK: - Accessors

auto ui::format::action::type() const -> enum type
{
    return m_type;
}

auto ui::format::action::set_type(const enum ui::format::action::type &type) -> void
{
    m_type = type;
}

auto ui::format::action::set_value(const std::string &value) -> void
{
    m_value = value;
}

auto ui::format::action::set_value(const resource::reference &value) -> void
{
    m_value = value;
}

auto ui::format::action::set_value(const graphite::data::block &value) -> void
{
    m_value = value;
}