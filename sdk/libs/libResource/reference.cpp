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

#include <libResource/reference.hpp>

// MARK: - Construction

resource::reference::reference(identifier id, const std::string& type_name, const std::string& type_code, const std::string& container)
    : m_id(id), m_type_name(type_name), m_type_code(type_code), m_container(container)
{}

auto resource::reference::decode_from(graphite::data::reader &reader) -> reference
{
    auto flags = static_cast<enum encoding_flags>(reader.read_byte());

    std::string container;
    if (flags & encoding_flags::container) {
        container = reader.read_pstr();
    }

    std::string code;
    if (flags & encoding_flags::typed) {
        code = reader.read_cstr(4);
    }

    auto id = reader.read_signed_quad();

    return reference(id, "", code, container);
}

auto resource::reference::auto_assigning() -> reference
{
    return reference(auto_value);
}

auto resource::reference::any() -> reference
{
    return reference(any_value);
}

// MARK: - Accessors

auto resource::reference::id() const -> identifier
{
    return m_id;
}

auto resource::reference::type_name() const -> std::string
{
    return m_type_name;
}

auto resource::reference::type_code() const -> std::string
{
    return m_type_code;
}

auto resource::reference::container_name() const -> std::string
{
    return m_container;
}


auto resource::reference::has_type_name() const -> bool
{
    return !m_type_name.empty();
}

auto resource::reference::has_type_code() const -> bool
{
    return !m_type_code.empty() && m_type_code != "????";
}

auto resource::reference::has_container() const -> bool
{
    return !m_container.empty();
}

auto resource::reference::is_auto() const -> bool
{
    return m_id == auto_value;
}

auto resource::reference::is_any() const -> bool
{
    return m_id == any_value;
}

auto resource::reference::is_type(const std::string& type) const -> bool
{
    return (type == m_type_name) || (type == m_type_code);
}

auto resource::reference::is_container(const std::string& container) const -> bool
{
    return container == m_container;
}


auto resource::reference::with_id(identifier id) const -> reference
{
    return reference(id, m_type_name, m_type_code, m_container);
}

auto resource::reference::with_type_name(const std::string& type_name, const std::string& type_code) const -> reference
{
    return reference(m_id, type_name, type_code, m_container);
}

auto resource::reference::with_container(const std::string& container) const -> reference
{
    return reference(m_id, m_type_name, m_type_code, container);
}

// MARK: - Encoding

auto resource::reference::encode_into(graphite::data::writer &writer) const -> void
{
    std::uint8_t flags = 0x00;
    flags |= (has_container() ? encoding_flags::container : 0);
    flags |= (has_type_code() ? encoding_flags::typed : 0);

    writer.write_byte(flags);
    if (flags & encoding_flags::container) {
        writer.write_pstr(container_name());
    }
    if (flags & encoding_flags::typed) {
        writer.write_cstr(type_code(), 4);
    }

    writer.write_signed_quad(id());
}