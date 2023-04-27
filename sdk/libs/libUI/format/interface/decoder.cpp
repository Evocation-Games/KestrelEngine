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

#include <libUI/format/interface/decoder.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Header

auto decode_version(ui::format::interface& an_interface, graphite::data::reader& reader) -> void
{
    reader.move(1);
    an_interface.set_version(reader.read_byte(), reader.read_byte(), reader.read_byte());
}

auto decode_rect(graphite::data::reader& reader) -> ui::rect
{
    return {
        reader.read_signed_long(), reader.read_signed_long(),
        reader.read_signed_long(), reader.read_signed_long()
    };
}

auto decode_background(graphite::data::reader& reader) -> ui::format::background
{
    auto type = reader.read_enum<enum ui::format::background::type>();
    auto fill = resource::reference::decode_from(reader);
    auto top = resource::reference::decode_from(reader);
    auto bottom = resource::reference::decode_from(reader);

    switch (type) {
        case ui::format::background::type::is_static: {
            return ui::format::background(fill);
        }
        case ui::format::background::type::is_dynamic: {
            return ui::format::background(top, fill, bottom);
        }
        default: {
            return {};
        }
    }
}

auto decode_header(ui::format::interface& an_interface, graphite::data::reader& reader) -> void
{
    an_interface.set_name(reader.read_pstr());
    decode_version(an_interface, reader);
    an_interface.set_flags(reader.read_enum<enum ui::format::interface::flags>());
    an_interface.set_frame(decode_rect(reader));
    an_interface.set_background(decode_background(reader));
}

// MARK: - Action

auto decode_action(graphite::data::reader& reader) -> ui::format::action
{
    auto type = reader.read_enum<enum ui::format::action::type>();
    switch (type) {
        case ui::format::action::type::push_named_scene:
        case ui::format::action::type::named_function: {
            return ui::format::action(reader.read_pstr(), type);
        }
        case ui::format::action::type::push_identified_scene:
        case ui::format::action::type::lua_script_reference: {
            return ui::format::action(resource::reference::decode_from(reader), type);
        }
        case ui::format::action::type::lua_script_source: {
            return ui::format::action(reader.read_cstr(), type);
        }
        case ui::format::action::type::lua_script_bytecode: {
            auto size = reader.read_long();
            auto bytecode = reader.read_data(size);
            return ui::format::action(bytecode);
        }
        default:
            return {};
    }
}

// MARK: - Values

auto decode_value(graphite::data::reader& reader) -> ui::format::value
{
    auto type = reader.read_enum<enum ui::format::value::type>();
    auto count = reader.read_short();
    switch (type) {
        case ui::format::value::type::no_value: {
            return {};
        }
        case ui::format::value::type::string: {
            return ui::format::value(reader.read_pstr());
        }
        case ui::format::value::type::integer: {
            return ui::format::value(reader.read_signed_quad());
        }
        case ui::format::value::type::unsigned_integer: {
            return ui::format::value(reader.read_quad());
        }
        case ui::format::value::type::color: {
            return ui::format::value(reader.read_long());
        }
        case ui::format::value::type::indexed_lookup: {
            auto lu = resource::reference::decode_from(reader);
            auto index = reader.read_short();
            auto type = reader.read_cstr(4);
            return ui::format::value(ui::format::indexed_lookup(lu, index, type));
        }
        case ui::format::value::type::named_lookup: {
            auto lu = resource::reference::decode_from(reader);
            auto field = reader.read_pstr();
            auto type = reader.read_cstr(4);
            return ui::format::value(ui::format::named_lookup(lu, field, type));
        }
        case ui::format::value::type::string_list: {
            std::vector<std::string> list;
            for (auto i = 0; i < count; ++i) {
                list.emplace_back(reader.read_pstr());
            }
            return ui::format::value(list);
        }
        case ui::format::value::type::integer_list: {
            std::vector<std::int64_t> list;
            for (auto i = 0; i < count; ++i) {
                list.emplace_back(reader.read_signed_quad());
            }
            return ui::format::value(list);
        }
        case ui::format::value::type::unsigned_integer_list: {
            std::vector<std::uint64_t> list;
            for (auto i = 0; i < count; ++i) {
                list.emplace_back(reader.read_quad());
            }
            return ui::format::value(list);
        }
        case ui::format::value::type::lookup: {
            auto lu = resource::reference::decode_from(reader);
            auto type = reader.read_cstr(4);
            return ui::format::value(ui::format::lookup(lu, type));
        }
        case ui::format::value::type::indexed_list_lookup: {
            auto lu = resource::reference::decode_from(reader);
            auto list_count = reader.read_short();
            std::vector<std::uint16_t> list;
            for (auto i = 0; i < list_count; ++i) {
                list.emplace_back(reader.read_short());
            }
            auto type = reader.read_cstr(4);
            return ui::format::value(ui::format::indexed_list_lookup(lu, list, type));
        }
        case ui::format::value::type::image: {
            auto lu = resource::reference::decode_from(reader);
            return ui::format::value(lu);
        }
        case ui::format::value::type::image_list: {
            std::vector<resource::reference> list;
            for (auto i = 0; i < count; ++i) {
                list.emplace_back(resource::reference::decode_from(reader));
            }
            return ui::format::value(list);
        }
        default: {
            return {};
        }
    }
}

// MARK: - Attributes

auto decode_attribute_value(graphite::data::reader& reader) -> ui::format::attribute_value
{
    auto type = reader.read_enum<enum ui::format::attribute_value::type>();
    switch (type) {
        case ui::format::attribute_value::type::string: {
            return ui::format::attribute_value(reader.read_cstr());
        }
        case ui::format::attribute_value::type::integer: {
            return ui::format::attribute_value(reader.read_signed_quad());
        }
        case ui::format::attribute_value::type::unsigned_integer: {
            return ui::format::attribute_value(reader.read_quad());
        }
        case ui::format::attribute_value::type::boolean: {
            return ui::format::attribute_value(reader.read_byte() != 0);
        }
        case ui::format::attribute_value::type::rect: {
            return ui::format::attribute_value(decode_rect(reader));
        }
        case ui::format::attribute_value::type::action: {
            return ui::format::attribute_value(decode_action(reader));
        }
        case ui::format::attribute_value::type::value: {
            return ui::format::attribute_value(decode_value(reader));
        }
        case ui::format::attribute_value::type::color_code: {
            return ui::format::attribute_value(reader.read_long());
        }
    }
}

auto decode_attribute(graphite::data::reader& reader) -> ui::format::attribute
{
    auto code = reader.read_enum<enum ui::format::attribute::code>();
    auto name = reader.read_pstr();
    std::vector<ui::format::attribute_value> values;

    auto count = reader.read_short();
    for (auto i = 0; i < count; ++i) {
        values.emplace_back(decode_attribute_value(reader));
    }

    return ui::format::attribute(code, name, values);
}

auto decode_attributes(ui::format::element& element, graphite::data::reader& reader) -> void
{
    auto count = reader.read_short();
    for (auto i = 0; i < count; ++i) {
        element.add_attribute(decode_attribute(reader));
    }
}

// MARK: - Elements

auto decode_element(ui::format::interface& an_interface, graphite::data::reader& reader) -> void
{
    auto type = reader.read_enum<enum ui::format::element::type>();
    ui::format::element element(type);
    decode_attributes(element, reader);
    an_interface.add_element(element);
}

auto decode_elements(ui::format::interface& an_interface, graphite::data::reader& reader) -> void
{
    auto count = reader.read_short();
    for (auto i = 0; i < count; ++i) {
        decode_element(an_interface, reader);
    }
}

// MARK: - Decoder

auto ui::format::decode(const graphite::data::block &data) -> interface
{
    struct interface an_interface;
    graphite::data::reader reader(&data);

    decode_header(an_interface, reader);
    decode_elements(an_interface, reader);

    return std::move(an_interface);
}
