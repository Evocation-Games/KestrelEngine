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

#include <libResource/definition/template/type.hpp>
#include <libResource/definition/template/instance.hpp>

// MARK: - Construction

resource::definition::binary_template::type::type(enum $type type)
    : m_base(type), m_nested({ .definition = nullptr })
{
    calculate_size();
}

resource::definition::binary_template::type::type(enum $type type, std::size_t size)
    : m_base(type), m_nested({ .definition = nullptr }), m_size(size)
{}

resource::definition::binary_template::type::type(const instance *nested, const std::string& name)
    : m_base($type::NESTED), m_nested({ .definition = nested, .name = name })
{}

// MARK: - Accessors

auto resource::definition::binary_template::type::name() const -> std::string
{
    switch (m_base) {
        case DBYT:      return "DBYT";
        case DWRD:      return "DWRD";
        case DLNG:      return "DLNG";
        case DQWD:      return "DQWD";
        case HBYT:      return "HBYT";
        case HWRD:      return "HWRD";
        case HLNG:      return "HLNG";
        case HQWD:      return "HQWD";
        case RECT:      return "RECT";
        case PSTR:      return "PSTR";
        case CSTR:      return "CSTR";
        case LSTR:      return "LSTR";
        case OSTR:      return "OSTR";
        case CHAR:      return "CHAR";
        case OCNT:      return "OCNT";
        case LSTC:      return "LSTC";
        case LSTE:      return "LSTE";
        case BBIT:      return "BBIT";
        case BOOL:      return "BOOL";
        case RSRC:      return "RSRC";
        case HEXD:      return "HEXD";
        case Cnnn: {
            return "Cnnn";
        }
        case NESTED: {
            return "Nested<" + m_nested.name + ">";
        }
        case LUA_BYTE_CODE: {
            return "ByteCode<Lua>";
        }
    }
}

auto resource::definition::binary_template::type::value() const -> enum $type
{
    return m_base;
}

auto resource::definition::binary_template::type::nested_type() const -> const instance *
{
    return m_nested.definition;
}

auto resource::definition::binary_template::type::nested_type_name() const -> std::string
{
    return m_nested.name;
}

auto resource::definition::binary_template::type::size() const -> std::size_t
{
    return m_size;
}

auto resource::definition::binary_template::type::calculate_size() -> void
{
    switch (m_base) {
        case $type::CHAR:
        case $type::HBYT:
        case $type::DBYT:
        case $type::BBIT:
            m_size = 1;
            break;
        case $type::DWRD:
        case $type::HWRD:
        case $type::OCNT:
            m_size = 2;
            break;
        case $type::DLNG:
        case $type::HLNG:
            m_size = 4;
            break;
        case $type::HQWD:
        case $type::DQWD:
        case $type::RECT:
            m_size = 8;
            break;
        case $type::LUA_BYTE_CODE:
        case $type::BOOL:
        case $type::CSTR:
        case PSTR:
            m_size = 1;
            break;
        case Cnnn:
        case LSTR:
        case OSTR:
        case LSTC:
        case LSTE:
        case HEXD:
            m_size = 0;
            break;
        case NESTED:
            m_size = m_nested.definition->minimum_size();
            break;
        case RSRC:
            m_size = 9;
            break;
    }
}