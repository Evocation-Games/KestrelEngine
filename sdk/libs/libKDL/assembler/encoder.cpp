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

#include <iostream>
#include <libKDL/assembler/encoder.hpp>

// MARK: - Construction

kdl::assembler::encoder::encoder(const resource::instance &instance, const resource::definition::type::instance *type)
    : m_instance_ref(&instance), m_type(type)
{}

// MARK: - Encoding

static auto inline extend_prefix(const std::string& prefix, const std::string& addition) -> std::string
{
    if (prefix.empty()) {
        return addition;
    }
    else {
        return prefix + "." + addition;
    }
}

auto kdl::assembler::encoder::encode() -> const graphite::data::block&
{
    encode_binary_template("", m_type->binary_template());
    return *m_writer.data();
}

auto kdl::assembler::encoder::encode_binary_template(const std::string& prefix, const resource::definition::binary_template::instance *binary_template) -> void
{
    const auto& values = m_instance_ref->values();
    for (const auto& field : binary_template->all_fields()) {
        encode_binary_field(prefix, field);
    }
}

auto kdl::assembler::encoder::encode_binary_field(const std::string& prefix, const resource::definition::binary_template::field& field) -> void
{
    auto value_name = extend_prefix(prefix, field.label());
    if (field.has_nested_type()) {
        // Nested type, which means we need to look at the nested binary template and handle that.
        encode_binary_template(value_name, field.nested_type());
    }
    else {
        // Singular value.
        auto it = m_instance_ref->values().find(value_name);
        if (it != m_instance_ref->values().end()) {
            encode_value(it->second, field.type());
        }
        else {
            // Populate a zero value?
            encode_value(resource::value_container(), field.type());
        }
    }
}

auto kdl::assembler::encoder::encode_value(const resource::value_container &value, const resource::definition::binary_template::type& type) -> void
{
    switch (type.value()) {
        case resource::definition::binary_template::type::CHAR:
        case resource::definition::binary_template::type::DBYT: {
            m_writer.write_signed_byte(value.integer_value<std::int8_t>());
            break;
        }
        case resource::definition::binary_template::type::DWRD: {
            m_writer.write_signed_short(value.integer_value<std::int16_t>());
            break;
        }
        case resource::definition::binary_template::type::DLNG: {
            m_writer.write_signed_long(value.integer_value<std::int32_t>());
            break;
        }
        case resource::definition::binary_template::type::DQWD: {
            m_writer.write_signed_quad(value.integer_value<std::int64_t>());
            break;
        }
        case resource::definition::binary_template::type::HBYT: {
            m_writer.write_byte(value.integer_value<std::uint8_t>());
            break;
        }
        case resource::definition::binary_template::type::HWRD: {
            m_writer.write_short(value.integer_value<std::uint16_t>());
            break;
        }
        case resource::definition::binary_template::type::HLNG: {
            m_writer.write_long(value.integer_value<std::uint32_t>());
            break;
        }
        case resource::definition::binary_template::type::HQWD: {
            m_writer.write_quad(value.integer_value<std::uint64_t>());
            break;
        }
        case resource::definition::binary_template::type::RECT: {
            break;
        }
        case resource::definition::binary_template::type::PSTR: {
            m_writer.write_pstr(value.string_value());
            break;
        }
        case resource::definition::binary_template::type::CSTR: {
            m_writer.write_cstr(value.string_value());
            break;
        }
        case resource::definition::binary_template::type::Cnnn: {
            // TODO: Fix the encoding of the size information.
            m_writer.write_cstr(value.string_value());
            break;
        }
        case resource::definition::binary_template::type::LSTR:
            break;
        case resource::definition::binary_template::type::OSTR:
            break;
        case resource::definition::binary_template::type::OCNT:
            break;
        case resource::definition::binary_template::type::LSTC:
            break;
        case resource::definition::binary_template::type::LSTE:
            break;
        case resource::definition::binary_template::type::BBIT:
            break;
        case resource::definition::binary_template::type::BOOL:
            break;
        case resource::definition::binary_template::type::HEXD: {
            m_writer.write_bytes(value.data_value());
            break;
        }
        case resource::definition::binary_template::type::RSRC: {
            if (m_writer.data()->is_extended_format()) {
                std::uint8_t flags = 0;
                std::string container;
                std::string type_name;

                if (value.reference_value().has_container()) {
                    container = value.reference_value().container_name();
                    flags |= 0x01;
                }

                if (value.reference_value().has_type_name()) {
                    type_name = value.reference_value().type_name();
                    flags |= 0x02;
                }

                m_writer.write_integer<std::uint8_t>(flags);
                if (container.empty()) {
                    m_writer.write_pstr(container);
                }
                if (type_name.empty()) {
                    m_writer.write_cstr(type_name, 4);
                }
                m_writer.write_integer<std::int64_t>(value.reference_value().id());
            }
            else {
                const auto& ref = value.reference_value();
                m_writer.write_integer<std::int16_t>(static_cast<std::int16_t>(ref.id()));
            }
            break;
        }
        case resource::definition::binary_template::type::NESTED:
            break;
    }
}