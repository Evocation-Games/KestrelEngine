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
#include <libKDL/intermediate/project/project.hpp>

// MARK: - Unit Management

auto kdl::intermediate::project::unit(const std::string &path, bool allow_creation) -> intermediate::unit&
{
    const auto path_hash = hashing::string(path);
    auto it = m_units.find(path_hash);
    if (it == m_units.end()) {
        if (allow_creation) {
            // The unit does not exist, so create it.
            intermediate::unit unit(path);
            m_units.insert(std::pair(path_hash, std::move(unit)));

            // Re-search for the unit, but do not allow creation if it does not exist.
            return this->unit(path, false);
        }
        else {
            throw std::logic_error("Failed to create the intermediate unit '" + path + "' in the project.");
        }
    }
    else {
        return it->second;
    }
}

// MARK: - Metadata Management

auto kdl::intermediate::project::write_metadata(const metadata::key &key, const std::string &value) -> void
{
    auto it = m_metadata.find(key);
    if (it == m_metadata.end()) {
        m_metadata.insert(std::pair(key, value));
    }
    else {
        it->second = value;
    }
}

auto kdl::intermediate::project::read_metadata(const metadata::key &key) const -> std::string
{
    auto it = m_metadata.find(key);
    if (it == m_metadata.end()) {
        return "";
    }
    else {
        return it->second;
    }
}
