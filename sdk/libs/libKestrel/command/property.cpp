// Copyright (c) 2022 Tom Hancocks
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

#include <libKestrel/command/property.hpp>
#include <libKestrel/device/console.hpp>

// MARK: - Construction

kestrel::command::property::property(const std::string &name, const std::int32_t expected_arguments)
    : m_name(name), m_name_hash(hash(name)), m_expected_arguments(expected_arguments)
{}

// MARK: - Hashing

auto kestrel::command::property::hash(const std::string &name) -> hashing::value64
{
    return hashing::xxh64(name.c_str(), name.size());
}

// MARK: - Return Values

auto kestrel::command::property::set_property_for_argument(const std::string &argument, const lua_reference &property) -> void
{
    const auto name = hash(argument);
    auto it = m_properties.find(name);
    if (it != m_properties.end()) {
        it->second = { property };
    }
    else {
        m_properties.insert({ name, property });
    }
}

auto kestrel::command::property::set_value_for_argument(const std::string &argument, const luabridge::LuaRef &value) -> void
{
    const auto name = hash(argument);
    auto it = m_properties.find(name);
    if (it != m_properties.end()) {
        if (value.state() && value.isBool()) {
            it->second = { value.cast<bool>() };
        }
        else if (value.state() && value.isNil()) {
            m_properties.erase(it);
        }
    }
    else if (value.state() && value.isBool()) {
        m_properties.insert(std::pair(name, value.cast<bool>()));
    }
}

auto kestrel::command::property::set_value(const luabridge::LuaRef &value) -> void
{
    if (m_expected_arguments > 0) {
        // TODO: Issue a warning here?
        kestrel::device::console::write(
            "Unable to apply return value to property with arguments; '" + m_name + "'",
            kestrel::device::console::status::warning
        );
        return;
    }

    if (value.state() && value.isBool()) {
        m_raw_value = value.cast<bool>();
    }
}

auto kestrel::command::property::boolean_value() const -> bool
{
    return (m_expected_arguments == 0 ? m_raw_value : false);
}

auto kestrel::command::property::boolean_value(hashing::value64 name_hash) const -> bool
{
    const auto it = m_properties.find(name_hash);
    if (it != m_properties.end()) {
        if (it->second.index() == boolean_index) {
            return std::get<bool>(it->second);
        }
    }
    return false;
}

auto kestrel::command::property::property_value(hashing::value64 name_hash) const -> lua_reference
{
    const auto it = m_properties.find(name_hash);
    if (it != m_properties.end()) {
        if (it->second.index() == property_index) {
            return std::get<lua_reference>(it->second);
        }
    }
    return { nullptr };
}

// MARK: - Name

auto kestrel::command::property::name() const -> std::string
{
    return m_name;
}

auto kestrel::command::property::name_hash() const -> hashing::value64
{
    return m_name_hash;
}

// MARK: - Argument Management

auto kestrel::command::property::expected_argument_count() const -> std::int32_t
{
    return m_expected_arguments;
}