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

#include <libUI/format/interface/encoder.hpp>
#include <libGraphite/data/writer.hpp>

// MARK: - Header

auto encode_version(const ui::format::interface::format_version& version, graphite::data::writer& writer) -> void
{
    writer.write_byte(0x00);
    writer.write_byte(version.major);
    writer.write_byte(version.minor);
    writer.write_byte(version.fix);
}

auto encode_rect(const ui::rect& rect, graphite::data::writer& writer) -> void
{
    writer.write_signed_long(rect.origin.x);
    writer.write_signed_long(rect.origin.y);
    writer.write_signed_long(rect.size.width);
    writer.write_signed_long(rect.size.height);
}

auto encode_background(const ui::format::background& background, graphite::data::writer& writer) -> void
{
    writer.write_enum(background.type());
    background.fill().encode_into(writer);
    background.top().encode_into(writer);
    background.bottom().encode_into(writer);
}

auto encode_header(const ui::format::interface& interface, graphite::data::writer& writer) -> void
{
    writer.write_pstr(interface.name());
    encode_version(interface.version(), writer);
    writer.write_enum(interface.flags());
    encode_rect(interface.frame(), writer);
    encode_background(interface.background(), writer);
}

// MARK: - Action

auto encode_action(const ui::format::action& action, graphite::data::writer& writer) -> void
{
    writer.write_enum(action.type());
    switch (action.type()) {
        case ui::format::action::type::push_named_scene:
        case ui::format::action::type::named_function: {
            writer.write_pstr(action.value<std::string>());
            break;
        }
        case ui::format::action::type::push_identified_scene:
        case ui::format::action::type::lua_script_reference: {
            action.value<resource::reference>().encode_into(writer);
            break;
        }
        case ui::format::action::type::lua_script_source: {
            writer.write_cstr(action.value<std::string>());
            break;
        }
        case ui::format::action::type::lua_script_bytecode: {
            auto bytecode = action.value<graphite::data::block>();
            writer.write_long(bytecode.size());
            writer.write_data(&bytecode);
            break;
        }
        default: break;
    }
}

// MARK: - Values

auto encode_value(const ui::format::value& value, graphite::data::writer& writer) -> void
{
    writer.write_enum(value.type());
    switch (value.type()) {
        case ui::format::value::type::no_value: {
            writer.write_short(0);
            break;
        }
        case ui::format::value::type::string: {
            writer.write_short(1);
            writer.write_pstr(value.get<std::string>());
            break;
        }
        case ui::format::value::type::integer: {
            writer.write_short(1);
            writer.write_signed_quad(value.get<std::int64_t>());
            break;
        }
        case ui::format::value::type::unsigned_integer: {
            writer.write_short(1);
            writer.write_quad(value.get<std::uint64_t>());
            break;
        }
        case ui::format::value::type::color: {
            writer.write_short(1);
            writer.write_long(value.get<std::uint32_t>());
            break;
        }
        case ui::format::value::type::indexed_lookup: {
            auto lu = value.get<ui::format::indexed_lookup>();
            writer.write_short(1);
            lu.reference().encode_into(writer);
            writer.write_short(lu.index());
            writer.write_cstr(lu.type(), 4);
            break;
        }
        case ui::format::value::type::named_lookup: {
            auto lu = value.get<ui::format::named_lookup>();
            writer.write_short(1);
            lu.reference().encode_into(writer);
            writer.write_pstr(lu.field());
            writer.write_cstr(lu.type(), 4);
            break;
        }
        case ui::format::value::type::string_list: {
            auto list = value.get<std::vector<std::string>>();
            writer.write_short(static_cast<std::uint16_t>(list.size()));
            for (const auto& str : list) {
                writer.write_pstr(str);
            }
            break;
        }
        case ui::format::value::type::integer_list: {
            auto list = value.get<std::vector<std::int64_t>>();
            writer.write_short(static_cast<std::uint16_t>(list.size()));
            for (const auto& i : list) {
                writer.write_signed_quad(i);
            }
            break;
        }
        case ui::format::value::type::unsigned_integer_list: {
            auto list = value.get<std::vector<std::uint64_t>>();
            writer.write_short(static_cast<std::uint16_t>(list.size()));
            for (const auto& i : list) {
                writer.write_quad(i);
            }
            break;
        }
        case ui::format::value::type::lookup: {
            auto lu = value.get<ui::format::lookup>();
            writer.write_short(1);
            lu.reference().encode_into(writer);
            writer.write_cstr(lu.type(), 4);
            break;
        }
        case ui::format::value::type::indexed_list_lookup: {
            auto lu = value.get<ui::format::indexed_list_lookup>();
            writer.write_short(1);
            lu.reference().encode_into(writer);
            writer.write_short(lu.list().size());
            for (const auto& i : lu.list()) {
                writer.write_short(i);
            }
            writer.write_cstr(lu.type(), 4);
            break;
        }
        case ui::format::value::type::image: {
            writer.write_short(1);
            value.get<resource::reference>().encode_into(writer);
            break;
        }
        case ui::format::value::type::image_list: {
            auto list = value.get<std::vector<resource::reference>>();
            writer.write_short(static_cast<std::uint16_t>(list.size()));
            for (const auto& ref : list) {
                ref.encode_into(writer);
            }
            break;
        }
    }
}

// MARK: - Attributes

auto encode_attribute_value(const ui::format::attribute_value& value, graphite::data::writer& writer) -> void
{
    writer.write_enum(value.type());
    switch (value.type()) {
        case ui::format::attribute_value::type::string: {
            writer.write_cstr(value.value<std::string>());
            break;
        }
        case ui::format::attribute_value::type::integer: {
            writer.write_signed_quad(value.value<std::int64_t>());
            break;
        }
        case ui::format::attribute_value::type::unsigned_integer: {
            writer.write_quad(value.value<std::uint64_t>());
            break;
        }
        case ui::format::attribute_value::type::boolean: {
            writer.write_byte(value.value<bool>() ? 0x01 : 0x00);
            break;
        }
        case ui::format::attribute_value::type::rect: {
            encode_rect(value.value<ui::rect>(), writer);
            break;
        }
        case ui::format::attribute_value::type::action: {
            encode_action(value.value<ui::format::action>(), writer);
            break;
        }
        case ui::format::attribute_value::type::value: {
            encode_value(value.value<ui::format::value>(), writer);
            break;
        }
        case ui::format::attribute_value::type::color_code: {
            writer.write_long(value.value<std::uint32_t>());
            break;;
        }
    }
}

auto encode_attribute(const ui::format::attribute& attribute, graphite::data::writer& writer) -> void
{
    writer.write_enum(attribute.code());
    writer.write_pstr(attribute.name());
    writer.write_short(static_cast<std::uint16_t>(attribute.value_count()));
    for (const auto& value : attribute.values()) {
        encode_attribute_value(value, writer);
    }
}

auto encode_attributes(const ui::format::element& element, graphite::data::writer& writer) -> void
{
    writer.write_short(static_cast<std::uint16_t>(element.attribute_count()));
    for (const auto& attribute : element.attributes()) {
        encode_attribute(attribute, writer);
    }
}

// MARK: - Elements

auto encode_element(const ui::format::element& element, graphite::data::writer& writer) -> void
{
    writer.write_enum(element.type());
    encode_attributes(element, writer);
}

auto encode_elements(const ui::format::interface& interface, graphite::data::writer& writer) -> void
{
    writer.write_short(static_cast<std::uint16_t>(interface.element_count()));
    for (const auto& element : interface.elements()) {
        encode_element(element, writer);
    }
}

// MARK: - Encoder

auto ui::format::encode(const ui::format::interface &interface) -> graphite::data::block
{
    graphite::data::writer writer(graphite::data::byte_order::msb);
    encode_header(interface, writer);
    encode_elements(interface, writer);
    return std::move(*const_cast<graphite::data::block *>(writer.data()));
}
