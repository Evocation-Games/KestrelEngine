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

#include <libKestrel/ui/types/value/value.hpp>
#include <libGraphite/toolbox/string_list.hpp>

// MARK: - Construction

kestrel::ui::value::value(const ::ui::format::value &value)
{
    load_values(value);
}

kestrel::ui::value::value(const std::string &value)
    : m_values({ boxed(value) }), m_list(false)
{}

// MARK: - Loading / Parsing

auto kestrel::ui::value::load_values(const ::ui::format::value &value) -> void
{
    switch (value.type()) {
        case ::ui::format::value::type::string: {
            add_value(value.get<std::string>());
            break;
        }
        case ::ui::format::value::type::integer: {
            add_value(value.get<std::int64_t>());
            break;
        }
        case ::ui::format::value::type::unsigned_integer: {
            add_value(value.get<std::uint64_t>());
            break;
        }
        case ::ui::format::value::type::color: {
            add_value(graphics::color(value.get<std::uint32_t>()));
            break;
        }
        case ::ui::format::value::type::indexed_lookup: {
            auto lookup = value.get<::ui::format::indexed_lookup>();
            if (lookup.reference().is_type("STR#") && lookup.type() == "PSTR") {
                resource::descriptor ref(lookup.reference());
                if (auto resource = ref.load()) {
                    graphite::toolbox::string_list list(resource->data());
                    add_value(list.at(lookup.index() - 1));
                }
            }
            break;
        }
        case ::ui::format::value::type::named_lookup:
            break;
        case ::ui::format::value::type::string_list: {
            for (const auto& str : value.get<std::vector<std::string>>()) {
                add_value(str);
            }
            break;
        }
        case ::ui::format::value::type::integer_list: {
            for (const auto& integer : value.get<std::vector<std::int64_t>>()) {
                add_value(integer);
            }
            break;
        }
        case ::ui::format::value::type::unsigned_integer_list: {
            for (const auto& integer : value.get<std::vector<std::uint64_t>>()) {
                add_value(integer);
            }
            break;
        }
        case ::ui::format::value::type::lookup:
            break;
        case ::ui::format::value::type::indexed_list_lookup:
            break;
        case ::ui::format::value::type::image: {
            add_value(resource::descriptor(value.get<::resource::reference>()));
            break;
        }
        case ::ui::format::value::type::image_list: {
            for (const auto& image : value.get<std::vector<::resource::reference>>()) {
                add_value(resource::descriptor(image));
            }
            break;
        }
        default:
            break;
    }
}

auto kestrel::ui::value::add_value(std::int64_t value) -> void
{
    m_values.emplace_back(value);
}

auto kestrel::ui::value::add_value(std::uint64_t value) -> void
{
    m_values.emplace_back(value);
}

auto kestrel::ui::value::add_value(const std::string &value) -> void
{
    m_values.emplace_back(value);
}

auto kestrel::ui::value::add_value(bool value) -> void
{
    m_values.emplace_back(value);
}

auto kestrel::ui::value::add_value(const resource::descriptor &value) -> void
{
    m_values.emplace_back(resource::descriptor::lua_reference(new resource::descriptor(value)));
}

auto kestrel::ui::value::add_value(const graphics::color &color) -> void
{
    m_values.emplace_back(graphics::color::lua_reference(new graphics::color(color)));
}

// MARK: - Boxed Values

auto kestrel::ui::value::count() const -> std::size_t
{
    return m_values.size();
}

auto kestrel::ui::value::string(std::int64_t idx) const -> std::string
{
    if (idx < 0 || idx >= m_values.size()) {
        return "";
    }

    const auto &boxed = m_values.at(idx);
    switch (boxed.stored_value.index()) {
        case boxed::type::string: {
            return std::get<std::string>(boxed.stored_value);
        }
        case boxed::type::integer: {
            return std::to_string(std::get<std::int64_t>(boxed.stored_value));
        }
        case boxed::type::unsigned_integer: {
            return std::to_string(std::get<std::uint64_t>(boxed.stored_value));
        }
        case boxed::type::boolean: {
            return std::get<bool>(boxed.stored_value) ? "true" : "false";
        }
        case boxed::type::descriptor: {
            return std::get<resource::descriptor::lua_reference>(boxed.stored_value)->description();
        }
        case boxed::type::color: {
            return "<COLOR-VALUE>";
        }
        default:
            return "";
    }
}

auto kestrel::ui::value::integer(std::int64_t idx) const -> std::int64_t
{
    if (idx < 0 || idx >= m_values.size()) {
        return 0;
    }

    const auto& boxed = m_values.at(idx);
    switch (boxed.stored_value.index()) {
        case boxed::type::integer: {
            return std::get<std::int64_t>(boxed.stored_value);
        }
        case boxed::type::unsigned_integer: {
            return static_cast<std::int64_t>(std::get<std::uint64_t>(boxed.stored_value));
        }
        case boxed::type::boolean: {
            return std::get<bool>(boxed.stored_value) ? 1 : 0;
        }
        case boxed::type::descriptor: {
            return std::get<resource::descriptor::lua_reference>(boxed.stored_value)->id;
        }
        default:
            return 0;
    }
}

auto kestrel::ui::value::unsigned_integer(std::int64_t idx) const -> std::uint64_t
{
    if (idx < 0 || idx >= m_values.size()) {
        return 0;
    }

    const auto& boxed = m_values.at(idx);
    switch (boxed.stored_value.index()) {
        case boxed::type::integer: {
            return static_cast<std::uint64_t>(std::get<std::int64_t>(boxed.stored_value));
        }
        case boxed::type::unsigned_integer: {
            return std::get<std::uint64_t>(boxed.stored_value);
        }
        case boxed::type::boolean: {
            return std::get<bool>(boxed.stored_value) ? 1 : 0;
        }
        case boxed::type::descriptor: {
            return static_cast<std::uint64_t>(std::get<resource::descriptor::lua_reference>(boxed.stored_value)->id);
        }
        default:
            return 0;
    }
}

auto kestrel::ui::value::boolean(std::int64_t idx) const -> bool
{
    if (idx < 0 || idx >= m_values.size()) {
        return false;
    }

    const auto& boxed = m_values.at(idx);
    switch (boxed.stored_value.index()) {
        case boxed::type::integer: {
            return (std::get<std::int64_t>(boxed.stored_value) != 0);
        }
        case boxed::type::unsigned_integer: {
            return (std::get<std::uint64_t>(boxed.stored_value) != 0);
        }
        case boxed::type::boolean: {
            return std::get<bool>(boxed.stored_value);
        }
        default:
            return false;
    }
}

auto kestrel::ui::value::descriptor(std::int64_t idx) const -> resource::descriptor::lua_reference
{
    if (idx < 0 || idx >= m_values.size()) {
        return { nullptr };
    }

    const auto& boxed = m_values.at(idx);
    switch (boxed.stored_value.index()) {
        case boxed::type::integer: {
            return resource::descriptor::identified(std::get<std::int64_t>(boxed.stored_value));
        }
        case boxed::type::unsigned_integer: {
            return resource::descriptor::identified(static_cast<std::int64_t>(std::get<std::uint64_t>(boxed.stored_value)));
        }
        case boxed::type::descriptor: {
            return std::get<resource::descriptor::lua_reference>(boxed.stored_value);
        }
        default:
            return { nullptr };
    }
}

auto kestrel::ui::value::color(std::int64_t idx) const -> graphics::color::lua_reference
{
    if (idx < 0 || idx >= m_values.size()) {
        return { nullptr };
    }

    const auto& boxed = m_values.at(idx);
    switch (boxed.stored_value.index()) {
        case boxed::type::color: {
            return std::get<graphics::color::lua_reference>(boxed.stored_value);
        }
        default:
            return graphics::color::black_color_ref();
    }
}