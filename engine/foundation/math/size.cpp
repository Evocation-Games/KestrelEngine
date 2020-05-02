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

#include "foundation/math/size.hpp"

// MARK: - Lua Integration

auto kestrel::math::size::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<math::size>("Size")
            .addConstructor<auto(*)(double, double)->void, size::lua_reference>()
            .addProperty("width", &size::width, &size::set_width)
            .addProperty("height", &size::height, &size::set_height)
        .endClass();
}

// MARK: - Constructors

kestrel::math::size::size()
{

}

kestrel::math::size::size(double width, double height)
        : m_width(width), m_height(height)
{

}

kestrel::math::size::size(const math::size& size)
        : m_width(size.m_width), m_height(size.m_height)
{

}

kestrel::math::size::size(const diamane::size& size)
        : m_width(size.width()), m_height(size.height())
{

}

kestrel::math::size::size(const graphite::qd::size& size)
        : m_width(size.width()), m_height(size.height())
{

}

auto kestrel::math::size::zero() -> kestrel::math::size
{
    return size(0, 0);
}

// MARK: - Conversions

auto kestrel::math::size::diamane() const -> diamane::size
{
    return diamane::size(m_width, m_height);
}

auto kestrel::math::size::quickdraw() const -> graphite::qd::size
{
    return graphite::qd::size(static_cast<int16_t>(m_width), static_cast<int16_t>(m_height));
}

// MARK: - Accessors

auto kestrel::math::size::width() const -> double
{
    return m_width;
}

auto kestrel::math::size::height() const -> double
{
    return m_height;
}

auto kestrel::math::size::set_width(double width) -> void
{
    m_width = width;
}

auto kestrel::math::size::set_height(double height) -> void
{
    m_height = height;
}

// MARK: - Calculations

auto kestrel::math::size::area() const -> double
{
    return m_width * m_height;
}

// MARK: - Operators

auto kestrel::math::size::operator* (const math::size& size) -> kestrel::math::size
{
    return kestrel::math::size(m_width * size.m_width, m_height * size.m_height);
}