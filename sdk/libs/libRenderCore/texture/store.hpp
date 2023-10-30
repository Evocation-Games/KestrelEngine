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

#pragma once

#include <set>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <unordered_map>
#include <libRenderCore/texture/info.hpp>
#include <libRenderCore/texture/id.hpp>

#if !defined(RENDERER_MAX_CACHED_TEXTURES)
#   define RENDERER_MAX_CACHED_TEXTURES     1000
#endif

namespace renderer::texture
{
    using handle = std::uintptr_t;

    class store
    {
    public:
        explicit store(std::size_t max = RENDERER_MAX_CACHED_TEXTURES);

        auto when_texture_removed(std::function<auto(texture::device_id)->void> callback) -> void;

        [[nodiscard]] inline auto is_full() const -> bool { return m_textures.size() >= m_max_textures; }

        [[nodiscard]] auto create_texture(texture::id id) -> texture::info&;
        [[nodiscard]] auto has_texture(texture::id id) const -> bool;
        auto get_texture(texture::id id) -> texture::info&;
        auto get_texture_temporary(texture::id id) const -> const texture::info&;
        auto drop_texture(texture::id id) -> void;

        auto purge_unused_textures() -> bool;

    private:
        std::size_t m_max_textures;
        std::unordered_map<texture::id, texture::info> m_textures;
        std::set<texture::id> m_unused;
        std::function<auto(texture::device_id)->void> m_remove_event;
    };
}