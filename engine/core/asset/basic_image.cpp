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

#include "core/asset/basic_image.hpp"
#include "core/environment.hpp"

// MARK: - Construction

asset::basic_image::basic_image(const math::size& size, const graphics::color& color)
{
    auto data = std::vector<uint32_t>(static_cast<int>(size.area()), color.value());

    // Generate a texture and then a spritesheet.
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, data);
        m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
    }
}

asset::basic_image::basic_image(const int64_t& id, const std::string& name, const math::size &size, std::vector<uint32_t> data)
    : m_id(id), m_name(name)
{
    // Generate a texture and then a spritesheet.
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, std::move(data));
        m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
    }
}

asset::basic_image::basic_image(const math::size &size, const graphics::color::lua_reference &color)
{
    auto data = std::vector<uint32_t>(static_cast<int>(size.area()), color->value());
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, std::move(data));
        m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
    }
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

auto asset::basic_image::spritesheet() const -> std::shared_ptr<graphics::spritesheet>
{
    return m_sheet;
}

// MARK: - Configuration

auto asset::basic_image::configure(const int64_t &id, const std::string &name, const math::size &size,
                             std::vector<uint32_t> data) -> void
{
    m_id = id;
    m_name = name;

    // Generate a texture and then a spritesheet.
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, std::move(data));
        m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
    }
}

auto asset::basic_image::configure(const int64_t &id, const std::string &name,
                                   std::shared_ptr<graphics::spritesheet> sheet) -> void
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

auto asset::basic_image::spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference
{
    auto entity = graphics::entity::lua_reference(new graphics::entity(size()));
    entity->position = position;
    entity->name = m_name;
    entity->id = m_id;
    entity->set_spritesheet(spritesheet());
    return entity;
}
