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

#include <type_traits>
#include "util/hint.hpp"
#include "core/graphics/common/texture.hpp"

#if TARGET_MACOS

#include <MetalKit/MetalKit.h>

namespace renderer::metal
{
    class texture : public graphics::texture
    {
    public:
        texture(uint32_t width, uint32_t height);
        explicit texture(const math::size& size);
        texture(uint32_t width, uint32_t height, const graphite::data::block& data);
        texture(const math::size& size, const graphite::data::block& data);
        texture(const math::size& size, const uint8_t *data);
        texture(id<MTLTexture> texture, const math::size& sz);

        ~texture();

        auto handle() const -> uint64_t override;
        auto handle_ptr() const -> id<MTLTexture>;

    private:
        id<MTLTexture> m_handle { 0 };
    };
}

#endif
