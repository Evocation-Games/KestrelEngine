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


#include "foundation/assets/cache.hpp"

// MARK: - Helpers

static inline auto make_hash(std::string type, kestrel::assets::resource_reference::lua_reference ref) -> std::size_t
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

// MARK: - Constructor

kestrel::assets::cache::cache()
{

}

// MARK: - Caching

auto kestrel::assets::cache::add(std::string type, resource_reference::lua_reference ref, std::any asset) -> void
{
    m_assets.emplace(make_hash(type, ref), asset);
}


auto kestrel::assets::cache::fetch(std::string type, resource_reference::lua_reference ref) const -> std::optional<std::any>
{
    auto k = make_hash(type, ref);
    if (m_assets.find(k) == m_assets.end()) {
        return {};
    }
    return m_assets.at(k);
}
