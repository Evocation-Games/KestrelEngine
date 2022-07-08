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
#include "util/xxhash.hpp"

namespace util
{
    struct uuid
    {
    public:
        uuid();
        uuid(const uuid&) = default;
        explicit uuid(uint64_t v);
        explicit uuid(const std::string& name);

        uuid(const std::string& resource_type, int64_t resource_id, const std::string& ns = {});

        explicit operator uint64_t() const { return m_value; }

        explicit operator uint32_t() const {
            std::string str { std::to_string(m_value) };
            return xxh32((const uint8_t* )str.c_str(), str.size());
        }

        auto operator==(const uuid& k) const -> bool { return m_value == k.m_value; }
        auto operator!=(const uuid& k) const -> bool { return m_value != k.m_value; }

    private:
        uint64_t m_value;
    };
}

namespace std
{
    template<>
    struct hash<util::uuid>
    {
        auto operator()(const util::uuid& k) const -> size_t
        {
            return hash<uint64_t>()((uint64_t)k);
        }
    };
}