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

#pragma once

#include <cstdint>
#include <string>

#include <libHashing/xxhash/xxhash.hpp>
#include <libResourceCore/structure/instance.hpp>

namespace kestrel::util
{
    /**
     * A basic unique form of unique identifier for uniquely identifying objects within the game.
     */
    struct uid
    {
    public:
        typedef std::uint64_t value;

        uid();
        uid(const uid&) = default;
        explicit uid(value v);
        explicit uid(const std::string& name);

        uid(const std::string& resource_type, resource_core::identifier resource_id, const std::string& ns = {});

        explicit operator value() const { return m_value; }

        explicit operator std::uint32_t() const {
            std::string str { std::to_string(m_value) };
            return hashing::xxh32((const std::uint8_t* )str.c_str(), str.size());
        }

        auto operator==(const uid& k) const -> bool { return m_value == k.m_value; }
        auto operator!=(const uid& k) const -> bool { return m_value != k.m_value; }

    private:
        value m_value;
    };
}

namespace std
{
    template<>
    struct hash<kestrel::util::uid>
    {
        auto operator()(const kestrel::util::uid& k) const -> size_t
        {
            return hash<std::uint64_t>()((std::uint64_t)k);
        }
    };
}