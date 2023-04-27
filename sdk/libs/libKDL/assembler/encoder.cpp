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
#include <numeric>
#include <limits>
#include <libKDL/assembler/encoder.hpp>

// MARK: - Construction

kdl::assembler::encoder::encoder(const resource::instance &instance, const resource::definition::type::instance *type, const std::vector<std::string>& definitions)
    : m_instance_ref(&instance), m_type(type), m_writer(graphite::data::byte_order::msb)
{
    if (std::find(definitions.begin(), definitions.end(), "FORCE_UNSAFE_IDS") != definitions.end()) {
        m_use_extended_resource_id = false;
    }
}

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

auto kdl::assembler::encoder::encode(const sema::context *ctx) -> const graphite::data::block&
{
    m_context = ctx;
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

auto kdl::assembler::encoder::encode_binary_field(const std::string& prefix, const resource::definition::binary_template::field& field, std::optional<std::uint16_t> index) -> void
{
    auto value_name = extend_prefix(prefix, field.label());
    if (index.has_value()) {
        value_name += std::to_string(index.value());
    }

    if (field.has_nested_type()) {
        // Nested type, which means we need to look at the nested binary template and handle that.
        encode_binary_template(value_name, field.nested_type());
    }
    else if (field.is_list()) {
        std::vector<std::uint16_t> list_indicies;
        for (auto n = 0; n <= std::numeric_limits<std::uint16_t>::max(); ++n) {
            auto it = m_instance_ref->values().find(value_name + std::to_string(n));
            if (it != m_instance_ref->values().end()) {
                list_indicies.emplace_back(n);
            }
        }
        encode_value(resource::value_container(list_indicies.size()), field.type());
        for (const auto idx : list_indicies) {
            for (const auto& element_field : field.list_fields()) {
                encode_binary_field(prefix, element_field, idx);
            }
        }
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
        case resource::definition::binary_template::type::OCNT:
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
            m_writer.write_quad(value.integer_value<std::uint64_t>());
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
            m_writer.write_cstr(value.string_value(), type.size());
            break;
        }
        case resource::definition::binary_template::type::HEXD: {
            m_writer.write_bytes(value.data_value());
            break;
        }
        case resource::definition::binary_template::type::RSRC: {
            if (m_use_extended_resource_id) {
                value.reference_value().encode_into(m_writer);
            }
            else {
                const auto& ref = value.reference_value();
                m_writer.write_integer<std::int16_t>(static_cast<std::int16_t>(ref.id()));
            }
            break;
        }
        case resource::definition::binary_template::type::NESTED: {
            break;
        }

        case resource::definition::binary_template::type::LUA_BYTE_CODE: {
            break;
        }

        default:
            break;
    }
}