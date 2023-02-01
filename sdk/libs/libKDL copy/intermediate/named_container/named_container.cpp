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
#include <libHashing/hashing.hpp>
#include <libKDL/intermediate/named_container/named_container.hpp>

// MARK: - Construction

kdl::intermediate::named_container::named_container(const std::string& name)
    : m_name(name)
{}

// MARK: - Default Containers

auto kdl::intermediate::named_container::global() -> named_container
{
    return named_container(global_container_name);
}

// MARK: - Accessors

auto kdl::intermediate::named_container::name() const -> std::string
{
    return m_name;
}

// MARK: - Merging

auto kdl::intermediate::named_container::merge(const named_container& container) -> void
{

}

// MARK: - Resource Containers

auto kdl::intermediate::named_container::resource_container(const std::string &code, bool allow_creation) -> intermediate::resource_container &
{
    const auto code_hash = hashing::string(code);
    auto it = m_types.find(code_hash);
    if (it == m_types.end()) {
        if (allow_creation) {
            intermediate::resource_container type(code);
            m_types.insert(std::pair(code_hash, std::move(type)));
            return resource_container(code, false);
        }
        else {
            throw std::logic_error("Failed to create the intermediate resource_container '" + code + "' in unit.");
        }
    }
    else {
        return it->second;
    }
}