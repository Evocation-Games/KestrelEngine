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

#include <libKestrel/cache/cache.hpp>

// MARK: - Storage

static std::vector<std::tuple<kestrel::resource::key, kestrel::rtc::clock::time, std::any>> s_cache_assets;

// MARK: - Caching Functions

auto kestrel::cache::add(const resource::descriptor::lua_reference &ref, const std::any &asset) -> void
{
    resource::key key(*ref.get());
    for (auto& it : s_cache_assets) {
        if (std::get<0>(it) == key) {
            std::get<2>(it) = asset;
            std::get<1>(it) = rtc::clock::global().current();
            return;
        }
    }

    s_cache_assets.emplace(s_cache_assets.begin(), std::tuple(key, rtc::clock::global().current(), asset));
}

auto kestrel::cache::fetch(const resource::descriptor::lua_reference &ref) -> std::optional<std::any>
{
    resource::key key(*ref.get());
    for (auto& it : s_cache_assets) {
        if (std::get<0>(it) == key) {
            std::get<1>(it) = rtc::clock::global().current();
            return std::get<2>(it);
        }
    }
    return {};
}

// MARK: - Purging

auto kestrel::cache::purge_all() -> void
{
    s_cache_assets.clear();
}

auto kestrel::cache::purge_unused() -> void
{
    const auto death_interval = 2.0 * 60.0; // 2 Minutes
    for (auto it = s_cache_assets.begin(); it != s_cache_assets.end(); ++it) {
        if (rtc::clock::global().since(std::get<1>(*it)).count() >= death_interval) {
            s_cache_assets.erase(it);
        }
    }
}


