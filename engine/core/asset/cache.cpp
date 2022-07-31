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

#include "core/asset/cache.hpp"

// MARK: - Caching Functions

auto asset::cache::add(const asset::resource_descriptor::lua_reference &ref, const std::any &asset) -> void
{
    asset::resource_key key(*ref.get());
    for (auto& it : m_assets) {
        if (std::get<0>(it) == key) {
            std::get<2>(it) = asset;
            std::get<1>(it) = rtc::clock::global().current();
            return;
        }
    }

    m_assets.emplace(m_assets.begin(), std::tuple(key, rtc::clock::global().current(), asset));
}

auto asset::cache::fetch(const asset::resource_descriptor::lua_reference &ref) -> std::optional<std::any>
{
    asset::resource_key key(*ref.get());
    for (auto& it : m_assets) {
        if (std::get<0>(it) == key) {
            std::get<1>(it) = rtc::clock::global().current();
            return std::get<2>(it);
        }
    }
    return {};
}

// MARK: - Purging

auto asset::cache::purge_unused() -> void
{
    const auto death_interval = 2.0 * 60.0; // 2 Minutes

    for (auto it = m_assets.begin(); it != m_assets.end(); ++it) {
        if (rtc::clock::global().since(std::get<1>(*it)).count() >= death_interval) {
            m_assets.erase(it);
        }
    }
}


