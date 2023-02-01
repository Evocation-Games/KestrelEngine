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

#include <stdexcept>
#include <libKDL/intermediate/unit/unit.hpp>

// MARK: - Construction

kdl::intermediate::unit::unit(const std::string &path)
    : m_destination_file_path(path)
{}

// MARK: - Hashing

auto kdl::intermediate::unit::hash() const -> hashing::value
{
    return hashing::string(m_destination_file_path);
}

// MARK: - Accessors

auto kdl::intermediate::unit::format() const -> enum graphite::rsrc::file::format
{
    return m_format;
}

// MARK: - Named Containers

auto kdl::intermediate::unit::container(const std::string &name, bool allow_creation) -> intermediate::named_container&
{
    const auto name_hash = hashing::string(name);
    auto it = m_containers.find(name_hash);
    if (it == m_containers.end()) {
        if (allow_creation) {
            // The container does not exist, so create it.
            intermediate::named_container container(name);
            m_containers.insert(std::pair(name_hash, std::move(container)));

            // Re-search for the container, but do not allow creation if it is still not found.
            return this->container(name, false);
        }
        else {
            throw std::logic_error("Failed to create a named container '" + name + "' in the intermediate unit.");
        }
    }
    else {
        return it->second;
    }
}

auto kdl::intermediate::unit::global_container() -> intermediate::named_container&
{
    return container(named_container::global_container_name);
}
