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

#include <random>
#include <libKestrel/util/uid.hpp>

// MARK: - Random

static std::random_device s_random_device;
static std::mt19937_64 s_engine(s_random_device());
static std::uniform_int_distribution<std::uint64_t> s_uniform_distribution;

// MARK: - Construction

kestrel::util::uid::uid()
    : m_value(s_uniform_distribution(s_engine))
{
}

kestrel::util::uid::uid(std::uint64_t v)
    : m_value(v)
{
}

kestrel::util::uid::uid(const std::string& name)
    : m_value(hashing::xxh64(reinterpret_cast<const std::uint8_t *>(name.c_str()), name.size()))
{
}

kestrel::util::uid::uid(const std::string &resource_type, std::int64_t resource_id, const std::string &container)
{
    std::string reference { container + "." + resource_type + "." + std::to_string(resource_id) };
    m_value = hashing::xxh64(reinterpret_cast<const std::uint8_t *>(reference.c_str()), reference.size());
}
