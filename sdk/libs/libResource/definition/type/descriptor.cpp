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

#include <libResource/definition/type/descriptor.hpp>

// MARK: - Construction

resource::definition::type::descriptor::descriptor(bool is_reference)
    : m_reference(is_reference)
{}

resource::definition::type::descriptor::descriptor(bool is_reference, const std::string &name)
    : m_reference(is_reference), m_type(name)
{}

resource::definition::type::descriptor::descriptor(bool is_reference, const std::string &name, const std::vector<std::string> &hints)
    : m_reference(is_reference), m_type(name), m_hints(hints)
{}

// MARK: - Accessors

auto resource::definition::type::descriptor::is_reference() const -> bool
{
    return m_reference;
}

auto resource::definition::type::descriptor::set_reference(bool ref) -> void
{
    m_reference = ref;
}

auto resource::definition::type::descriptor::name() const -> std::string
{
    return m_type;
}

auto resource::definition::type::descriptor::has_name() const -> bool
{
    return !m_type.empty();
}

auto resource::definition::type::descriptor::set_name(const std::string &name) -> void
{
    m_type = name;
}

auto resource::definition::type::descriptor::has_hints() const -> bool
{
    return !m_hints.empty();
}

auto resource::definition::type::descriptor::hints() const -> std::vector<std::string>
{
    return m_hints;
}