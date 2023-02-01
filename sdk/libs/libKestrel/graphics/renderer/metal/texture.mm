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

#include <utility>
#include <libKestrel/graphics/renderer/metal/texture.h>
#include <libKestrel/graphics/renderer/metal/context.h>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

static std::uint32_t s_texture_id = 0;

// MARK: - Construction

kestrel::renderer::metal::texture::texture(std::uint32_t width, std::uint32_t height)
    : graphics::texture(width, height, true)
{
}

kestrel::renderer::metal::texture::texture(const math::size& size)
    : graphics::texture(size, true)
{
}

kestrel::renderer::metal::texture::texture(uint32_t width, uint32_t height, const graphite::data::block& data)
    : graphics::texture(width, height, data)
{
}

kestrel::renderer::metal::texture::texture(const math::size& sz, const graphite::data::block& data)
    : graphics::texture(sz, data)
{
}

kestrel::renderer::metal::texture::texture(const math::size& sz, const std::uint8_t *data)
    : graphics::texture(sz, data)
{
}

kestrel::renderer::metal::texture::texture(id<MTLTexture> texture, const math::size& sz)
    : graphics::texture(sz, false), m_handle(texture)
{

}

// MARK: - Destruction

kestrel::renderer::metal::texture::~texture()
{
    m_handle = nil;
}

// MARK: - Accessors

auto kestrel::renderer::metal::texture::set_data(const graphite::data::block &data) -> void
{
    graphics::texture::set_data(data);

    // If we're already uploaded, then adjust the contents of the texture?
    if (uploaded()) {
        MTLRegion region = MTLRegionMake2D(0, 0, m_descriptor.width, m_descriptor.height);
        region.origin.z = 0;
        region.size.depth = 1;

        NSUInteger bytes_per_row = m_handle.width << 2;
        [m_handle replaceRegion:region mipmapLevel:0 withBytes:m_data.get<void *>() bytesPerRow:bytes_per_row];
    }
}

auto kestrel::renderer::metal::texture::handle() const -> std::uint64_t
{
    return reinterpret_cast<std::uint64_t>(m_handle);
}

auto kestrel::renderer::metal::texture::handle_ptr() const -> id<MTLTexture>
{
    return m_handle;
}

// MARK: - Upload

auto kestrel::renderer::metal::texture::upload_to_gpu() -> void
{
    m_id = s_texture_id++;

    auto ctx = reinterpret_cast<metal::context *>(renderer::current_context());
    auto device = ctx->device();

    m_descriptor = [MTLTextureDescriptor new];
    m_descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    m_descriptor.mipmapLevelCount = 1;
    m_descriptor.width = static_cast<NSUInteger>(m_size.width());
    m_descriptor.height = static_cast<NSUInteger>(m_size.height());

    m_handle = [device newTextureWithDescriptor:m_descriptor];
    MTLRegion region = MTLRegionMake2D(0, 0, m_descriptor.width, m_descriptor.height);
    region.origin.z = 0;
    region.size.depth = 1;

    NSUInteger bytes_per_row = m_handle.width << 2;
    [m_handle replaceRegion:region mipmapLevel:0 withBytes:m_data.get<void *>() bytesPerRow:bytes_per_row];

    graphics::texture::upload_to_gpu();
}

auto kestrel::renderer::metal::texture::destroy() -> void
{
    m_handle = nullptr;
    m_descriptor = nullptr;
}
