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
            .addProperty("renderSize", &entity::get_render_size, &entity::set_render_size)
            .addProperty("bounds", &entity::get_bounds)
            .addProperty("alpha", &entity::get_alpha, &entity::set_alpha)
            .addProperty("blend", &entity::get_blend_lua, &entity::set_blend_lua)
            .addProperty("clippingArea", &entity::clipping_area, &entity::set_clipping_area)
            .addProperty("clippingOffset", &entity::clipping_offset, &entity::set_clipping_offset)
            .addFunction("draw", &entity::draw)
            .addFunction("intersects", &entity::is_intersecting)
            .addFunction("removeClipping", &entity::remove_clipping_area)
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
    render_size = m_spritesheet->sprite_size();
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
    return math::rect(this->position.point(), this->size);
}

auto graphics::entity::get_size() const -> math::size
{
    return this->size;
}

auto graphics::entity::set_size(const math::size &sz) -> void
{
    this->size = sz;
}

auto graphics::entity::get_render_size() const -> math::size
{
    return this->render_size;
}

auto graphics::entity::set_render_size(const math::size &sz) -> void
{
    this->render_size = sz;
}

lua_api auto graphics::entity::set_clipping_area(const math::size& sz) -> void
{
    auto clip_sz = sz * environment::active_environment().lock()->window()->get_scale_factor();
    m_has_texture_clip = true;
    m_clipping_area_uv = math::size(clip_sz.width / texture()->size().width, clip_sz.height / texture()->size().height);
    m_clipping_area = sz;
}

lua_api auto graphics::entity::remove_clipping_area() -> void
{
    m_has_texture_clip = false;
}

lua_api auto graphics::entity::has_clipping_area() const -> bool
{
    return m_has_texture_clip;
}

lua_api auto graphics::entity::clipping_area() const -> math::size
{
    return m_clipping_area;
}

auto graphics::entity::clipping_area_uv() const -> math::size
{
    return m_clipping_area_uv;
}

lua_api auto graphics::entity::set_clipping_offset(const math::point& p) -> void
{
    m_clipping_offset = math::point(
       std::max(0.0, std::min(texture()->size().width, p.x)),
       std::max(0.0, std::min(texture()->size().height, p.y))
    ) * 2;

    m_clipping_offset_uv = math::point(m_clipping_offset.x / texture()->size().width, m_clipping_offset.y / texture()->size().height);
}

lua_api auto graphics::entity::clipping_offset() const -> math::point
{
    return m_clipping_offset;
}

auto graphics::entity::clipping_offset_uv() const -> math::point
{
    return m_clipping_offset_uv;
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