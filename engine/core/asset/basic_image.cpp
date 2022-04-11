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

#include "core/environment.hpp"
#include "core/asset/basic_image.hpp"

#include <utility>
#include "renderer/common/renderer.hpp"

// MARK: - Construction

asset::basic_image::basic_image(const math::size& size, const graphics::color& color)
{
    auto data = std::vector<uint32_t>(static_cast<int>(size.area()), color.value());
    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
}

asset::basic_image::basic_image(int64_t id, const std::string& name, const math::size &size, const std::vector<uint32_t>& data)
    : m_id(id), m_name(std::move(name))
{
    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
}

asset::basic_image::basic_image(const math::size &size, const graphics::color::lua_reference &color)
{
    auto data = std::vector<uint32_t>(static_cast<int>(size.area()), color->value());
    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
}

// MARK: - Accessors

auto asset::basic_image::id() const -> int64_t
{
    return m_id;
}

auto asset::basic_image::name() const -> std::string
{
    return m_name;
}

auto asset::basic_image::size() const -> math::size
{
    return m_sheet->sprite_size();
}

auto asset::basic_image::sprite_count() const -> int
{
    return m_sheet->sprite_count();
}

auto asset::basic_image::sprite_sheet() const -> std::shared_ptr<graphics::spritesheet>
{
    return m_sheet;
}

// MARK: - Configuration

auto asset::basic_image::configure(int64_t id, const std::string& name, const math::size& size, const std::vector<uint32_t>& data) -> void
{
    m_id = id;
    m_name = name;

    auto tex = renderer::create_texture(size, data);
    m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
}

auto asset::basic_image::configure(int64_t id, const std::string& name, const std::shared_ptr<graphics::spritesheet>& sheet) -> void
{
    m_id = id;
    m_name = name;
    m_sheet = sheet;
}


auto asset::basic_image::layout_sprites(const math::size& sprite_size) -> void
{
    m_sheet->layout_sprites(sprite_size);
}

// MARK: - Entity

auto asset::basic_image::spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>
{
    auto entity = std::make_shared<graphics::entity>(size());
    entity->set_position(position);
    entity->set_name(m_name);
    entity->set_id(m_id);
    entity->set_sprite_sheet(sprite_sheet());
    return entity;
}
