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

#include <libHashing/hashing.hpp>
#include <libKDL/session/containers/container.hpp>

// MARK: - Construction

kdl::session::namespace_container::namespace_container(const std::string &name)
    : m_name(name), m_name_hash(hashing::string(name))
{}

// MARK: - Type Containers

auto kdl::session::namespace_container::type_container(const type_definition *type) -> resources::type_container&
{
    const auto& name_hash = hashing::string(type->name());
    auto it = m_types.find(name_hash);
    if (it != m_types.end()) {
        return it->second;
    }

    // Create a new type container
    resources::type_container container(type);
    m_types.insert(std::pair(name_hash, container));
    it = m_types.find(name_hash);
    if (it == m_types.end()) {
        // TODO: Something seriously wrong here... throw a custom exception
        throw std::runtime_error("");
    }
    return it->second;
}