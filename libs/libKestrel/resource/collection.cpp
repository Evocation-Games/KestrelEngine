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
#include <libKestrel/resource/collection.hpp>

// MARK: - Accesors

auto kestrel::resource::collection::size() const -> std::size_t
{
    return m_resources.size();
}

// MARK: - Item Management

auto kestrel::resource::collection::add_resource(const descriptor::lua_reference &descriptor, const luabridge::LuaRef &resource) -> void
{
    key key(*descriptor.get());

    // Find an existing entry and replace it...
    for (auto& it : m_resources) {
        if (it.first == key) {
            it.second = resource;
            return;
        }
    }

    // Not found, so add new entry.
    m_resources.emplace_back(std::pair(key, resource));
}

auto kestrel::resource::collection::random() const -> luabridge::LuaRef
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_resources.size() - 1); // distribution in range [1, 6]

    auto idx = dist(rng);
    auto resource = m_resources.at(dist(rng));
    return resource.second;
}

auto kestrel::resource::collection::get(const descriptor::lua_reference &descriptor) const -> luabridge::LuaRef
{
    key key(*descriptor.get());

    for (auto& it : m_resources) {
        if (it.first == key) {
            return it.second;
        }
    }

    return { nullptr };
}

auto kestrel::resource::collection::at(std::int32_t idx) const -> luabridge::LuaRef
{
    if (idx >= 1 && idx <= m_resources.size()) {
        return m_resources.at(idx - 1).second;
    }

    return { nullptr };
}

auto kestrel::resource::collection::has(const descriptor::lua_reference &descriptor) const -> bool
{
    key key(*descriptor.get());

    for (const auto& it : m_resources) {
        if (it.first == key) {
            return true;
        }
    }

    return false;
}

auto kestrel::resource::collection::each(const luabridge::LuaRef& block) const -> void
{
    if (block.state() && block.isFunction()) {
        for (const auto& it : m_resources) {
            block(it.second);
        }
    }
}