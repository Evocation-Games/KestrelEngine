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

#include <libKestrel/resource/key.hpp>
#include <libGraphite/util/hashing.hpp>
#include <libKestrel/resource/container.hpp>

// MARK: - Construction

kestrel::resource::key::key(const descriptor &descriptor)
{
    m_id = descriptor.id;

    if (descriptor.has_type()) {
        m_type = descriptor.type;
    }

    if (descriptor.has_container()) {
        m_container = descriptor.containers[0];
    }
}

// MARK: - Accessors

auto kestrel::resource::key::str() const -> std::string
{
    std::string result;

    if (m_container.has_value() && !m_container.value().empty()) {
        result += m_container.value() + ".";
    }

    if (m_type.has_value()) {
        result += m_type.value() + ".";
    }

    result += "#" + std::to_string(m_id.value());
    return result;
}

auto kestrel::resource::key::hash() const -> std::string
{
    auto str = this->str();
    return std::to_string(graphite::hashing::xxh64(str.c_str(), str.size()));
}

// MARK: - Operators

auto kestrel::resource::key::operator==(const key &rhs) const -> bool
{
    // Deal with namespace first.
    if (m_container.has_value() && rhs.m_container.has_value()) {
        // Check for universal namespace, as this makes the namespace valid.
        if ((m_container == resource::container::universal_name || rhs.m_container == resource::container::universal_name) || m_container == rhs.m_container) {
            // Do nothing...
        }
        else {
            return false;
        }
    }
    else if ((m_container.has_value() && m_container.value().empty()) && !rhs.m_container.has_value()) {
        // Do nothing...
    }
    else if ((rhs.m_container.has_value() && rhs.m_container.value().empty()) && !m_container.has_value()) {
        // Do nothing...
    }
    else if (!m_container.has_value() && !rhs.m_container.has_value()) {
        // Do nothing...
    }
    else {
        return false;
    }

    // Deal with the type.
    if (m_type.has_value() || rhs.m_type.has_value()) {
        if (m_type == rhs.m_type) {
            // Do nothing...
        }
        else {
            return false;
        }
    }

    // Finally, match the ID
    return m_id == rhs.m_id;
}