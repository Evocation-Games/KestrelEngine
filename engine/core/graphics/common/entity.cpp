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

#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/scene.hpp"

#include <utility>

// MARK: - Lua Constants

static auto blend_mode_normal() -> int
{
    return static_cast<int>(graphics::entity::blend::normal);
}

static auto blend_mode_light() -> int
{
    return static_cast<int>(graphics::entity::blend::light);
}

// MARK: - Lua

auto graphics::entity::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::entity>("Entity")
            .addProperty("frame", &entity::get_sprite_index, &entity::set_sprite_index)
            .addProperty("position", &entity::get_position, &entity::set_position)
            .addProperty("size", &entity::get_size, &entity::set_size)
            .addProperty("bounds", &entity::get_bounds)
            .addProperty("alpha", &entity::get_alpha, &entity::set_alpha)
            .addProperty("blend", &entity::get_blend_lua, &entity::set_blend_lua)
            .addFunction("draw", &entity::draw)
            .addFunction("intersects", &entity::is_intersecting)
            .addFunction("setClipSize", &entity::set_clip_size)
            .addFunction("removeClipSize", &entity::remove_clip_size)
            .addFunction("setSpriteOffset", &entity::set_sprite_offset)
        .endClass();

    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("BlendMode")
            .addProperty("NORMAL", &blend_mode_normal)
            .addProperty("LIGHT", &blend_mode_light)
        .endNamespace();
}

// MARK: - Construction

graphics::entity::entity(const math::size &size)
    : position(0, 0), size(size)
{

}

graphics::entity::entity(const math::vector& position, const math::size &size)
    : position(position), size(size)
{

}

// MARK: - Accessors

auto graphics::entity::move_to_scene(std::weak_ptr<graphics::scene> scene)
{
    m_scene = std::move(scene);
}

auto graphics::entity::scene() const -> std::weak_ptr<graphics::scene>
{
    return m_scene;
}

auto graphics::entity::set_spritesheet(std::shared_ptr<graphics::spritesheet> sheet, const int& index) -> void
{
    m_spritesheet = std::move(sheet);
    sprite_index = index;
}

auto graphics::entity::spritesheet() const -> std::shared_ptr<graphics::spritesheet>
{
    return m_spritesheet;
}

auto graphics::entity::texture() const -> std::shared_ptr<graphics::texture>
{
    return m_spritesheet->texture();
}

auto graphics::entity::get_sprite_index() const -> int
{
    return sprite_index;
}

auto graphics::entity::set_sprite_index(const int &index) -> void
{
    sprite_index = index;

    auto sprite_size = m_spritesheet->at(sprite_index).size();
    auto tex_size = m_spritesheet->texture()->size();
    size = math::size(sprite_size.width * tex_size.width, sprite_size.height * tex_size.height);
}

auto graphics::entity::get_position() const -> math::vector
{
    return position;
}

auto graphics::entity::set_position(const math::vector &position) -> void
{
    this->position = position;
}

auto graphics::entity::get_bounds() const -> math::rect
{
    auto half_size = this->size / 2.0;

    return math::rect({this->position.x - half_size.width,this->position.y - half_size.height},
                      this->size);
}

auto graphics::entity::get_size() const -> math::size
{
    return this->size;
}

auto graphics::entity::set_size(const math::size &sz) -> void
{
    this->size = sz;
}

auto graphics::entity::set_clip_size(const math::size& sz) -> void
{
    m_clip_size = math::size(sz.width / texture()->size().width, sz.height / texture()->size().height);
    m_has_texture_clip = true;
}

auto graphics::entity::remove_clip_size() -> void
{
    m_has_texture_clip = false;
}

auto graphics::entity::has_clip_size() const -> bool
{
    return m_has_texture_clip;
}

auto graphics::entity::clip_size() const -> math::size
{
    return m_clip_size;
}

auto graphics::entity::set_sprite_offset(const math::point &offset) -> void
{
    auto x_bound = 1.0 - m_clip_size.width;
    auto y_bound = 1.0 - m_clip_size.height;
    m_sprite_offset = math::point(
         std::max(0.0, std::min(x_bound, offset.x / texture()->size().width)),
         std::max(0.0, std::min(y_bound, offset.y / texture()->size().height))
    );

}

auto graphics::entity::get_sprite_offset() const -> math::point
{
    return m_sprite_offset;
}

auto graphics::entity::blend() const -> enum entity::blend
{
    return m_blend;
}

auto graphics::entity::get_blend_lua() const -> int
{
    return static_cast<int>(m_blend);
}

auto graphics::entity::set_blend_lua(const int& blend) -> void
{
    m_blend = static_cast<enum entity::blend>(blend);
}

auto graphics::entity::get_alpha() const -> double
{
    return m_alpha;
}

auto graphics::entity::set_alpha(const double& alpha) -> void
{
    m_alpha = alpha;
}

// MARK: - Rendering

auto graphics::entity::draw() -> void
{
    // Only attempt drawing the entity if it is in a scene.
    if (auto scene = m_scene.lock()) {
        scene->draw_entity(this);
    }
    else if (auto env = environment::active_environment().lock()) {
        auto scene = env->current_scene();
        if (scene) {
            scene->draw_entity(this);
        }
    }
}

// MARK: - Physics

auto graphics::entity::is_intersecting(const graphics::entity::lua_reference &subject) const -> bool
{
    return get_bounds().intersects(subject->get_bounds());
}