// Copyright (c) 2020 Tom Hancocks
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

#include <libKestrel/math/size.hpp>

// MARK: - Construction

kestrel::math::size::size(float v)
    : m_value(simd::float32::constant(v))
{}

kestrel::math::size::size(float w, float h)
    : m_value(w, h, w, h)
{}

kestrel::math::size::size(simd::float32 v)
    : m_value(v)
{}

kestrel::math::size::size(const ::ui::size& size)
    : m_value(
        static_cast<float>(size.width), static_cast<float>(size.height),
        static_cast<float>(size.width), static_cast<float>(size.height)
    )
{}

auto kestrel::math::size::ui_size() const -> ::ui::size
{
    return { static_cast<std::int32_t>(width()), static_cast<std::int32_t>(height()) };
}

// MARK: - Operators

auto kestrel::math::size::operator+(const size& s) const -> size
{
    return size(m_value + s.m_value);
}

auto kestrel::math::size::operator-(const size& s) const -> size
{
    return size(m_value - s.m_value);
}

auto kestrel::math::size::operator*(const size& s) const -> size
{
    return size(m_value * s.m_value);
}

auto kestrel::math::size::operator/(const size& s) const -> size
{
    return size(m_value / s.m_value);
}

auto kestrel::math::size::operator+(float f) const -> size
{
    return size(m_value + f);
}

auto kestrel::math::size::operator-(float f) const -> size
{
    return size(m_value - f);
}

auto kestrel::math::size::operator*(float f) const -> size
{
    return size(m_value * f);
}

auto kestrel::math::size::operator/(float f) const -> size
{
    return size(m_value / f);
}

auto kestrel::math::size::operator==(const size& s) const -> bool
{
    return (m_value[0] == s.m_value[0]) && (m_value[1] == s.m_value[1]);
}

auto kestrel::math::size::operator!=(const size& s) const -> bool
{
    return (m_value[0] != s.m_value[0]) || (m_value[1] != s.m_value[1]);
}

// MARK: - Operations

auto kestrel::math::size::add(const size& s) const -> size
{
    return operator+(s);
}

auto kestrel::math::size::subtract(const size& s) const -> size
{
    return operator-(s);
}

auto kestrel::math::size::multiply(float f) const -> size
{
    return operator*(f);
}

auto kestrel::math::size::divide(float f) const -> size
{
    return operator/(f);
}

auto kestrel::math::size::round() const -> size
{
    return size(m_value.round());
}

auto kestrel::math::size::floor() const -> size
{
    return size(m_value.floor());
}

auto kestrel::math::size::ceil() const -> size
{
    return size(m_value.ceil());
}

auto kestrel::math::size::area() const -> float
{
    return width() * height();
}

auto kestrel::math::size::aspect_ratio() const -> float
{
    return width() / height();
}

auto kestrel::math::size::is_portrait() const -> bool
{
    return width() <= height();
}

auto kestrel::math::size::is_landscape() const -> bool
{
    return width() >= height();
}

// MARK: - Accessors

auto kestrel::math::size::set_width(float width) -> void
{
    m_value.set(0, width);
    m_value.set(2, width);
}

auto kestrel::math::size::width() const -> float
{
    return m_value[0];
}

auto kestrel::math::size::set_height(float height) -> void
{
    m_value.set(1, height);
    m_value.set(3, height);
}

auto kestrel::math::size::height() const -> float
{
    return m_value[1];
}
