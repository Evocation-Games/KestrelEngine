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

// MARK: - Helpers

static inline auto make_hash(const std::string& type, const asset::resource::lua_reference& ref) -> std::size_t
{
    if (ref->id().has_value()) {
        return std::hash<std::string>{}("type=" + type + ":id=" + std::to_string(ref->id().value()));
    }
    else if (ref->name().has_value()) {
        return std::hash<std::string>{}("type=" + type + ":name=" + ref->name().value());
    }
    else {
        return std::hash<std::string>{}("type=" + type);
    }
}

// MARK: - Caching Functions

auto asset::cache::add(const std::string &type, const asset::resource::lua_reference &ref,
                       const std::any &asset) -> void
{
    m_assets[make_hash(type, ref)] = std::make_tuple(asset, rtc::clock::global().current());
}

auto asset::cache::fetch(const std::string &type,
                         const asset::resource::lua_reference &ref) -> std::optional<std::any>
{
    auto k = make_hash(type, ref);
    if (m_assets.find(k) == m_assets.end()) {
        return {};
    }

    auto asset = std::get<0>(m_assets.at(k));
    m_assets[k] = std::make_tuple(asset, rtc::clock::global().current());
    return asset;
}

// MARK: - Purging

auto asset::cache::purge_unused() -> void
{
    const auto death_interval = 10.0 * 60.0; // 10 Minutes

    for (const auto& asset_pair : m_assets) {
        if (rtc::clock::global().since(std::get<1>(asset_pair.second)).count() >= death_interval) {
            m_assets.erase(asset_pair.first);
        }
    }
}


