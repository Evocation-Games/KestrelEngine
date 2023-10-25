// Copyright (c) 2023 Tom Hancocks
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

#include <libRenderCore/texture/store.hpp>

// MARK: - Construction

renderer::texture::store::store(std::size_t max)
    : m_max_textures(max)
{}

// MARK: - Events

auto renderer::texture::store::when_texture_removed(std::function<auto(texture::device_id)->void> callback) -> void
{
    m_remove_event = std::move(callback);
}

// MARK: - Management

auto renderer::texture::store::create_texture(texture::id id) -> texture::info&
{
    if (!has_texture(id)) {
        // Create a new texture accordingly...
        if (is_full() && !purge_unused_textures()) {
            throw std::runtime_error("Unable to create new texture. Out of available slots.");
        }
        m_textures.insert({ id, texture::info() });
    }
    return get_texture(id);
}

auto renderer::texture::store::has_texture(texture::id id) const -> bool
{
    return m_textures.find(id) != m_textures.end();
}

auto renderer::texture::store::get_texture(texture::id id) -> texture::info &
{
    auto& info = m_textures.find(id)->second;
    if (++info.use_count) {
        m_unused.erase(id);
    }
    return info;
}

auto renderer::texture::store::get_texture_temporary(texture::id id) const -> const texture::info&
{
    auto& info = m_textures.find(id)->second;
    return info;
}

auto renderer::texture::store::drop_texture(texture::id id) -> void
{
    auto& info = m_textures.find(id)->second;
    if (--info.use_count == 0) {
        m_unused.insert(id);
    }
}

// MARK: - Purging

auto renderer::texture::store::purge_unused_textures() -> bool
{
    for (const auto& id : m_unused) {
        auto it = m_textures.find(id);
        if (it->second.gpu_live) {
            // Make sure we clean up live GPU textures.
            m_remove_event(it->second.handle);
        }
        m_textures.erase(id);
    }
    m_textures.clear();
    return true;
}