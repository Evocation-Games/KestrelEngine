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

#include <libUI/format/interface/interface.hpp>

// MARK: - Construction

ui::format::interface::interface(const std::string& name)
    : m_name(name)
{}

// MARK: - Accessors

auto ui::format::interface::name() const -> std::string
{
    return m_name;
}

auto ui::format::interface::version() const -> struct format_version
{
    return m_version;
}

auto ui::format::interface::flags() const -> enum flags
{
    return static_cast<enum flags>(m_flags);
};

auto ui::format::interface::frame() const -> struct rect
{
   return m_frame;
}

auto ui::format::interface::background() const -> struct background
{
    return m_background;
}

auto ui::format::interface::element_count() const -> std::size_t
{
    return m_elements.size();
}

auto ui::format::interface::elements() const -> const std::vector<element>&
{
    return m_elements;
}

auto ui::format::interface::element_at(std::int32_t idx) const -> element
{
    return m_elements.at(idx);
}

auto ui::format::interface::set_name(const std::string& name) -> void
{
    m_name = name;
}

auto ui::format::interface::set_version(std::uint8_t major, std::uint8_t minor, std::uint8_t fix) -> void
{
    m_version.major = major;
    m_version.minor = minor;
    m_version.fix = fix;
}

auto ui::format::interface::set_flags(std::uint16_t flags) -> void
{
    m_flags = flags;
}

auto ui::format::interface::set_flags(enum flags flags) -> void
{
    m_flags = flags;
}

auto ui::format::interface::add_flag(enum flags flag) -> void
{
    m_flags |= flag;
}

auto ui::format::interface::set_frame(const struct rect& frame) -> void
{
    m_frame = frame;
}

auto ui::format::interface::set_frame(std::int16_t x, std::int16_t y, std::int16_t width, std::int16_t height) -> void
{
    m_frame = { x, y, width, height };
}

auto ui::format::interface::set_frame_size(std::int16_t width, std::int16_t height) -> void
{
    m_frame = { 0, 0, width, height };
    m_flags |= flags::centered;
}

auto ui::format::interface::set_background(const resource::reference& fill) -> void
{
    m_background = format::background(fill);
}

auto ui::format::interface::set_background(const resource::reference& top, const resource::reference& fill, const resource::reference& bottom) -> void
{
    m_background = format::background(top, fill, bottom);
}

auto ui::format::interface::set_background(const struct background& background) -> void
{
    m_background = background;
}


// MARK: - Elements

auto ui::format::interface::add_element(const struct element& element) -> void
{
    m_elements.emplace_back(element);
}