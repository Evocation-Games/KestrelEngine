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

#include <libKDL/session/types/binary_template/binary_template.hpp>

// MARK: - Construction

kdl::session::binary_template::binary_template(const std::string& name)
    : m_name(name)
{}

kdl::session::binary_template::field::field(
    const lexer::lexeme& label,
    const data_type &type,
    const lexer::lexeme& nested_type,
    const type_definition *nested_type_definition
)
    : label(label), type(type), nested_type({ .name = nested_type, .definition = nested_type_definition })
{}

// MARK: - Accessor

auto kdl::session::binary_template::name() const -> std::string
{
    return m_name;
}


// MARK: - Field Management

auto kdl::session::binary_template::add_field(const lexer::lexeme &label, enum data_type type, const lexer::lexeme& nested_type) -> void
{
    add_field(field(label, type, nested_type));
}

auto kdl::session::binary_template::add_field(const field &field) -> void
{
    m_fields.emplace_back(field);
}

auto kdl::session::binary_template::field_count() const -> std::size_t
{
    return m_fields.size();
}

auto kdl::session::binary_template::at(std::int32_t idx) const -> const field&
{
    return m_fields.at(idx);
}

auto kdl::session::binary_template::mutable_at(std::int32_t idx) -> field&
{
    return m_fields.at(idx);
}

auto kdl::session::binary_template::named(const lexer::lexeme &name) const -> const field&
{
    return named(name.text());
}

auto kdl::session::binary_template::named(const std::string &name) const -> const field&
{
    for (const auto& it : m_fields) {
        if (it.label.is(name)) {
            return it;
        }
    }
    throw std::logic_error("Could not find the field '" + name + "' in the template.");
}

auto kdl::session::binary_template::index(const lexer::lexeme &name) const -> std::int32_t
{
    return index(name.text());
}

auto kdl::session::binary_template::index(const std::string &name) const -> std::int32_t
{
    for (std::int32_t idx = 0; idx < m_fields.size(); ++idx) {
        const auto it = m_fields.at(idx);
        if (it.label.is(name)) {
            return idx;
        }
    }
    throw std::logic_error("Could not find the field '" + name + "' in the template.");
}

auto kdl::session::binary_template::has_field(const lexer::lexeme &name) const -> bool
{
    return has_field(name.text());
}

auto kdl::session::binary_template::has_field(const std::string &name) const -> bool
{
    for (const auto& it : m_fields) {
        if (it.label.is(name)) {
            return true;
        }
    }
    return false;
}

auto kdl::session::binary_template::fields() const -> const std::vector<field>&
{
    return m_fields;
}

// MARK: - Data Types

auto kdl::session::binary_template::data_type_for_name(const std::string &name) -> enum data_type
{
    if (name == "HBYT") {
        return data_type::HBYT;
    }
    else if (name == "DBYT") {
        return data_type::DBYT;
    }
//  else if (name == "FBYT") {
//      return data_type::FBYT;
//  }
    else if (name == "HWRD") {
        return data_type::HWRD;
    }
    else if (name == "DWRD") {
        return data_type::DWRD;
    }
//  else if (name == "FWRD") {
//      return data_type::FWRD;
//  }
    else if (name == "HLNG") {
        return data_type::HLNG;
    }
    else if (name == "DLNG") {
        return data_type::DLNG;
    }
//  else if (name == "FLNG") {
//      return data_type::FLNG;
//  }
    else if (name == "HQAD") {
        return data_type::HQAD;
    }
    else if (name == "DQAD") {
        return data_type::DQAD;
    }
//  else if (name == "FQAD") {
//      return data_type::FQAD;
//  }
//  else if (name == "AWRD") {
//      return data_type::AWRD;
//  }
//  else if (name == "ALNG") {
//      return data_type::ALNG;
//  }
//  else if (name == "AQAD") {
//      return data_type::AQAD;
//  }
    else if (name == "HEXD") {
        return data_type::HEXD;
    }
    else if (name == "PSTR") {
        return data_type::PSTR;
    }
//  else if (name == "LSTR") {
//      return data_type::LSTR;
//  }
//  else if (name == "WSTR") {
//      return data_type::WSTR;
//  }
//  else if (name == "ESTR") {
//      return data_type::ESTR;
//  }
//  else if (name == "OSTR") {
//      return data_type::OSTR;
//  }
    else if (name == "CSTR") {
        return data_type::CSTR;
    }
//  else if (name == "ECST") {
//      return data_type::ECST;
//  }
//  else if (name == "OCST") {
//      return data_type::OCST;
//  }
//  else if (name == "BOOL") {
//      return data_type::BOOL;
//  }
//  else if (name == "BBIT") {
//      return data_type::BBIT;
//  }
//  else if (name == "TNAM") {
//      return data_type::TNAM;
//  }
//  else if (name == "CHAR") {
//      return data_type::CHAR;
//  }
    else if (name == "RECT") {
        // We're going to transmute to Nested<Rect>.
        return data_type::NESTED;
    }
    else if (name[0] == 'C') {
        auto width = static_cast<std::uint32_t>(std::stoul(name.substr(1), nullptr, 16));
        return static_cast<data_type>(static_cast<std::uint32_t>(Cnnn) | width);
    }
//  else if (name[0] == 'H') {
//      auto width = static_cast<std::uint32_t>(std::stoul(name.substr(1), nullptr, 16));
//      return static_cast<data_type>(static_cast<std::uint32_t>(Hnnn) | width);
//  }
//  else if (name[0] == 'P' && name[1] == '0') {
//      auto width = static_cast<std::uint32_t>(std::stoul(name.substr(1), nullptr, 16));
//      return static_cast<data_type>(static_cast<std::uint32_t>(P0nn) | width);
//  }
    else if (name == "OCNT") {
        return data_type::OCNT;
    }
//  else if (name == "LSTZ") {
//      return data_type::LSTZ;
//  }
    else if (name == "LSTE") {
        return data_type::LSTE;
    }
//    else if (name == "ZCNT") {
//        return data_type::ZCNT;
//    }
    else if (name == "LSTC") {
        return data_type::LSTC;
    }
//  else if (name == "LSTB") {
//      return data_type::LSTB;
//  }
    else if (name == "RSRC") {
        return data_type::RSRC;
    }
    else if (name == "Nested") {
        return data_type::NESTED;
    }

    return data_type::invalid;
}

auto kdl::session::binary_template::data_type_base_size(enum data_type type) -> std::size_t
{
    switch (type & ~0xFFF) {
        case data_type::CSTR:
        case data_type::PSTR:
        case data_type::HBYT:
        case data_type::DBYT: {
            return 1;
        }
        case data_type::HWRD:
        case data_type::DWRD:
        case data_type::OCNT: {
            return 2;
        }
        case data_type::HLNG:
        case data_type::DLNG: {
            return 4;
        }
        case data_type::RECT:
        case data_type::HQAD:
        case data_type::DQAD: {
            return 8;
        }
        case data_type::RSRC: {
            return 9;
        }
        case data_type::Cnnn: {
            return type & 0xFFF;
        }
        case data_type::HEXD: {
            return 0;
        }
        case data_type::NESTED: {
            return 0;
        }
        default:
            return 0;
    }
}