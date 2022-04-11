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
#include "core/environment.hpp"
#include "renderer/common/renderer.hpp"
#include "core/graphics/common/entity.hpp"

// MARK: - Construction

graphics::entity::entity(const math::size &size)
    : m_position( 0 ), m_size(size)
{

}

graphics::entity::entity(const math::point& position, const math::size &size)
    : m_position(position), m_size(size)
{

}

// MARK: - Accessors

auto graphics::entity::move_to_scene(const std::shared_ptr<class scene>& scene)
{
    m_scene = scene;
}

auto graphics::entity::scene() const -> std::weak_ptr<class scene>
{
    return m_scene;
}

auto graphics::entity::name() const -> std::string
{
    return m_name;
}

auto graphics::entity::set_name(const std::string &name) -> void
{
    m_name = name;
}

auto graphics::entity::id() const -> int64_t
{
    return m_id;
}

auto graphics::entity::set_id(int64_t id) -> void
{
    m_id = id;
}

auto graphics::entity::set_sprite_sheet(const std::shared_ptr<graphics::spritesheet>& sheet, uint32_t sprite_index) -> void
{
    m_sprite_sheet = sheet;
    m_sprite_index = sprite_index;
    m_render_size = m_sprite_sheet->sprite_size();
}

auto graphics::entity::sprite_sheet() const -> std::shared_ptr<graphics::spritesheet>
{
    return m_sprite_sheet;
}

auto graphics::entity::texture() const -> std::shared_ptr<graphics::texture>
{
    return m_sprite_sheet->texture();
}

auto graphics::entity::get_sprite_index() const -> uint32_t
{
    return m_sprite_index;
}

auto graphics::entity::set_sprite_index(uint32_t sprite_index) -> void
{
    m_sprite_index = sprite_index;

    auto sprite_size = m_sprite_sheet->at(static_cast<int>(sprite_index)).size();
    auto tex_size = m_sprite_sheet->texture()->size();
    m_size = math::size(sprite_size.width * tex_size.width, sprite_size.height * tex_size.height);
}

auto graphics::entity::get_position() const -> math::point
{
    return m_position;
}

auto graphics::entity::set_position(const math::point &position) -> void
{
    m_position = position;
}

auto graphics::entity::get_bounds() const -> math::rect
{
    return { m_position, m_size };
}

auto graphics::entity::get_size() const -> math::size
{
    return m_size;
}

auto graphics::entity::set_size(const math::size &sz) -> void
{
    m_size = sz;
}

auto graphics::entity::get_render_size() const -> math::size
{
    return m_render_size;
}

auto graphics::entity::set_render_size(const math::size &sz) -> void
{
    m_render_size = sz;
}

auto graphics::entity::set_clipping_area(const math::size& sz) -> void
{
    auto clip_sz = sz * renderer::scale_factor();
    m_has_texture_clip = true;
    m_clipping_area_uv = math::size(clip_sz.width / texture()->size().width, clip_sz.height / texture()->size().height);
    m_clipping_area = sz;
}

auto graphics::entity::remove_clipping_area() -> void
{
    m_has_texture_clip = false;
}

auto graphics::entity::has_clipping_area() const -> bool
{
    return m_has_texture_clip;
}

auto graphics::entity::clipping_area() const -> math::size
{
    return m_clipping_area;
}

auto graphics::entity::clipping_area_uv() const -> math::size
{
    return m_clipping_area_uv;
}

auto graphics::entity::set_clipping_offset(const math::point& p) -> void
{
    m_clipping_offset = math::point(
       std::max(0.0, std::min(texture()->size().width, p.x)),
       std::max(0.0, std::min(texture()->size().height, p.y))
    ) * 2;

    m_clipping_offset_uv = math::point(m_clipping_offset.x / texture()->size().width, m_clipping_offset.y / texture()->size().height);
}

auto graphics::entity::clipping_offset() const -> math::point
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

auto graphics::entity::set_blend_lua(int blend) -> void
{
    m_blend = static_cast<enum entity::blend>(blend);
}

auto graphics::entity::get_alpha() const -> double
{
    return m_alpha;
}

auto graphics::entity::set_alpha(double alpha) -> void
{
    m_alpha = alpha;
}

// MARK: - Rendering

auto graphics::entity::draw() -> void
{
    auto env = environment::active_environment().lock();
    if (env) {
        auto session = env->session();
        if (session) {
            auto scene = session->current_scene();
            if (scene.get()) {
                scene->internal_scene()->draw_entity(shared_from_this());
            }
        }
    }
}

// MARK: - Physics

auto graphics::entity::is_intersecting(const graphics::entity& subject) const -> bool
{
    return get_bounds().intersects(subject.get_bounds());
}