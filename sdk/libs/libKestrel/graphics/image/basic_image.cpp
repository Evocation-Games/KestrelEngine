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

#include <utility>
#include <libKestrel/graphics/image/basic_image.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

// MARK: - Construction

kestrel::image::basic_image::basic_image(const math::size& size, const graphics::color& color)
{
    data::block data(size.area());
    data.set(color.color_value(), data.size());
    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::sprite_sheet>(tex, size);
}

kestrel::image::basic_image::basic_image(resource_core::identifier id, const std::string& name, const math::size &size, const data::block& data)
    : m_id(id), m_name(name)
{
    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::sprite_sheet>(tex, size);
}

kestrel::image::basic_image::basic_image(const math::size &size, const graphics::color::lua_reference &color)
{
    data::block data(size.area());
    data.set(color->color_value(), data.size());

    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::sprite_sheet>(tex, size);
}

// MARK: - Destruction

kestrel::image::basic_image::~basic_image()
{
    m_sheet = nullptr;
}

// MARK: - Accessors

auto kestrel::image::basic_image::id() const -> resource_core::identifier
{
    return m_id;
}

auto kestrel::image::basic_image::name() const -> std::string
{
    return m_name;
}

auto kestrel::image::basic_image::size() const -> math::size
{
    return m_sheet->sprite_size();
}

auto kestrel::image::basic_image::sprite_count() const -> std::size_t
{
    return m_sheet->sprite_count();
}

auto kestrel::image::basic_image::sprite_sheet() const -> std::shared_ptr<graphics::sprite_sheet>
{
    return m_sheet;
}

// MARK: - Configuration

auto kestrel::image::basic_image::configure(resource_core::identifier id, const std::string &name, const math::size &size, const data::block &data) -> void
{
    m_id = id;
    m_name = name;

    auto tex = renderer::create_texture(size, data);
    tex->set_data(data);
    m_sheet = std::make_shared<graphics::sprite_sheet>(tex, size);
}

auto kestrel::image::basic_image::configure(int64_t id, const std::string& name, const std::shared_ptr<graphics::sprite_sheet>& sheet) -> void
{
    m_id = id;
    m_name = name;
    m_sheet = sheet;
}


auto kestrel::image::basic_image::layout_sprites(const math::size& sprite_size) -> void
{
    m_sheet->layout_sprites(sprite_size);
}

// MARK: - Entity

auto kestrel::image::basic_image::spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity>
{
    auto entity = std::make_shared<ecs::entity>(size());
    entity->set_position(position);
    entity->set_name(m_name);
    entity->set_id(m_id);
    entity->set_sprite_sheet(sprite_sheet());
    return entity;
}

