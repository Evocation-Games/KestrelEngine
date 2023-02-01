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

#include <libKestrel/command/node.hpp>
#include <libGraphite/util/hashing.hpp>

// MARK: - Construction

kestrel::command::node::node(graphite::hashing::value64 name, node *parent)
    : m_parent(parent), m_name(name)
{
}

// MARK: - Naming

auto kestrel::command::node::name_hash() const -> graphite::hashing::value64
{
    return m_name;
}

auto kestrel::command::node::is_named(const std::string &name) const -> bool
{
    return is_named(graphite::hashing::xxh64(name.c_str(), name.size()));
}

auto kestrel::command::node::is_named(graphite::hashing::value64 value) const -> bool
{
    return value == m_name;
}

// MARK: - Arguments

auto kestrel::command::node::add_argument(argument&& arg) -> void
{
    m_arguments.emplace_back(std::move(arg));
}

auto kestrel::command::node::argument_at(std::int32_t idx) const -> const argument&
{
    return m_arguments.at(idx);
}

auto kestrel::command::node::has_argument(graphite::hashing::value64 value, std::int32_t idx) const -> bool
{
    if (idx < 0) {
        for (const auto& arg : m_arguments) {
            if (arg.keypath_hash_value() == value) {
                return true;
            }
        }
        return false;
    }
    else {
        return argument_at(idx).keypath_hash_value() == value;
    }
}

auto kestrel::command::node::argument_count() const -> std::int32_t
{
    return m_arguments.size();
}

// MARK: - Children

auto kestrel::command::node::children() const -> std::vector<node::lua_reference>
{
    return m_children;
}

auto kestrel::command::node::has_children() const -> bool
{
    return !m_children.empty();
}