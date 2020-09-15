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

#include "core/asset/image.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto asset::image::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<asset::image>("Image")
            .addConstructor<auto(*)(const math::size::lua_reference&, const graphics::color::lua_reference&)->void, asset::image::lua_reference>()
            .addProperty("size", &asset::image::size)
            .addProperty("numberOfSprites", &asset::image::sprite_count)
            .addFunction("spawnEntity", &asset::image::spawn_entity)
            .addFunction("setSpriteSize", &asset::image::layout_sprites)
        .endClass();
}

// MARK: - Construction

asset::image::image(const math::size& size, const graphics::color& color)
{
    auto data = std::vector<uint32_t>(static_cast<int>(size.area()), color.value());

    // Generate a texture and then a spritesheet.
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, data);
        m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
    }
}

asset::image::image(const int64_t& id, const std::string& name, const math::size &size, std::vector<uint32_t> data)
    : m_id(id), m_name(name)
{
    // Generate a texture and then a spritesheet.
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(size, std::move(data));
        m_sheet = std::make_shared<graphics::spritesheet>(tex, size);
    }
}

asset::image::image(const math::size::lua_reference &size, const graphics::color::lua_reference &color)
{
    auto data = std::vector<uint32_t>(static_cast<int>(size->area()), color->value());
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(*size.get(), std::move(data));
        m_sheet = std::make_shared<graphics::spritesheet>(tex, *size.get());
    }
}

// MARK: - Accessors

auto asset::image::size() const -> math::size
{
    return m_sheet->sprite_size();
}

auto asset::image::sprite_count() const -> int
{
    return m_sheet->sprite_count();
}

auto asset::image::spritesheet() const -> std::shared_ptr<graphics::spritesheet>
{
    return m_sheet;
}

// MARK: - Configuration

auto asset::image::configure(const int64_t &id, const std::string &name, const math::size &size,
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

auto asset::image::layout_sprites(const math::size& sprite_size) -> void
{
    m_sheet->layout_sprites(sprite_size);
}

// MARK: - Entity

auto asset::image::spawn_entity(const math::vector& position) const -> graphics::entity::lua_reference
{
    auto entity = graphics::entity::lua_reference(new graphics::entity(size()));
    entity->position = position;
    entity->set_spritesheet(spritesheet());
    return entity;
}

