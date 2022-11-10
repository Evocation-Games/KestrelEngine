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
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/entity/entity.hpp>

// MARK: - Construction

kestrel::ecs::entity::entity(const math::size &size)
    : m_position( 0 ), m_size(size)
{
}

kestrel::ecs::entity::entity(const math::point& position, const math::size &size)
    : m_position(position), m_size(size)
{
}

// MARK: - Destruction

kestrel::ecs::entity::~entity()
{
    m_sprite_sheet = nullptr;
}

// MARK: - Accessors

auto kestrel::ecs::entity::move_to_scene(const std::shared_ptr<class scene>& scene)
{
    m_scene = scene;
}

auto kestrel::ecs::entity::scene() const -> std::weak_ptr<class scene>
{
    return m_scene;
}

auto kestrel::ecs::entity::name() const -> std::string
{
    return m_name;
}

auto kestrel::ecs::entity::set_name(const std::string &name) -> void
{
    m_name = name;
}

auto kestrel::ecs::entity::id() const -> util::uid::value
{
    return static_cast<util::uid::value>(uid());
}

auto kestrel::ecs::entity::set_id(util::uid::value id) -> void
{
    set_uid(util::uid(id));
}

auto kestrel::ecs::entity::uid() const -> util::uid
{
    return m_id;
}

auto kestrel::ecs::entity::set_uid(util::uid id) -> void
{
    m_id = id;
}

auto kestrel::ecs::entity::set_sprite_sheet(const std::shared_ptr<graphics::sprite_sheet>& sheet, std::uint32_t sprite_index) -> void
{
    m_sprite_sheet = sheet;
    m_sprite_index = sprite_index;
    m_render_size = m_sprite_sheet->sprite_size();
}

auto kestrel::ecs::entity::sprite_sheet() const -> std::shared_ptr<graphics::sprite_sheet>
{
    return m_sprite_sheet;
}

auto kestrel::ecs::entity::texture() const -> std::shared_ptr<graphics::texture>
{
    return m_sprite_sheet != nullptr ? m_sprite_sheet->texture() : nullptr;
}

auto kestrel::ecs::entity::get_sprite_index() const -> uint32_t
{
    return m_sprite_index;
}

auto kestrel::ecs::entity::set_sprite_index(uint32_t sprite_index) -> void
{
    m_sprite_index = sprite_index;

    auto sprite_size = m_sprite_sheet->at(static_cast<int>(sprite_index)).size();
    auto tex_size = m_sprite_sheet->texture()->size();
    m_size = math::size(sprite_size.width * tex_size.width, sprite_size.height * tex_size.height);
}

auto kestrel::ecs::entity::get_position() const -> math::point
{
    return m_position;
}

auto kestrel::ecs::entity::set_position(const math::point &position) -> void
{
    m_position = position;
}

auto kestrel::ecs::entity::get_bounds() const -> math::rect
{
    return { m_position, m_size };
}

auto kestrel::ecs::entity::get_size() const -> math::size
{
    return m_size;
}

auto kestrel::ecs::entity::set_size(const math::size &sz) -> void
{
    m_size = sz;
}

auto kestrel::ecs::entity::get_render_size() const -> math::size
{
    return m_render_size;
}

auto kestrel::ecs::entity::set_render_size(const math::size &sz) -> void
{
    m_render_size = sz;
}

auto kestrel::ecs::entity::get_draw_size() const -> math::size
{
    return m_draw_size;
}

auto kestrel::ecs::entity::set_draw_size(const math::size &sz) -> void
{
    m_draw_size = sz;
}

auto kestrel::ecs::entity::set_clipping_area(const math::size& sz) -> void
{
    auto clip_sz = sz * renderer::scale_factor();
    m_has_texture_clip = true;
    m_clipping_area_uv = math::size(clip_sz.width / texture()->size().width, clip_sz.height / texture()->size().height);
    m_clipping_area = sz;
}

auto kestrel::ecs::entity::remove_clipping_area() -> void
{
    m_has_texture_clip = false;
}

auto kestrel::ecs::entity::has_clipping_area() const -> bool
{
    return m_has_texture_clip;
}

auto kestrel::ecs::entity::clipping_area() const -> math::size
{
    return m_clipping_area;
}

auto kestrel::ecs::entity::clipping_area_uv() const -> math::size
{
    return m_clipping_area_uv;
}

auto kestrel::ecs::entity::set_clipping_offset(const math::point& p) -> void
{
    m_clipping_offset = math::point(
       std::max(0.0, std::min(texture()->size().width, p.x)),
       std::max(0.0, std::min(texture()->size().height, p.y))
    );

    m_clipping_offset_uv = math::point(m_clipping_offset.x / texture()->size().width, m_clipping_offset.y / texture()->size().height);
}

auto kestrel::ecs::entity::clipping_offset() const -> math::point
{
    return m_clipping_offset;
}

auto kestrel::ecs::entity::clipping_offset_uv() const -> math::point
{
    return m_clipping_offset_uv;
}

auto kestrel::ecs::entity::blend() const -> enum renderer::blending
{
    return m_blend;
}

auto kestrel::ecs::entity::get_blend_lua() const -> int
{
    return static_cast<int>(m_blend);
}

auto kestrel::ecs::entity::set_blend_lua(int blend) -> void
{
    m_blend = static_cast<enum renderer::blending>(blend);
}

auto kestrel::ecs::entity::get_alpha() const -> double
{
    return m_alpha;
}

auto kestrel::ecs::entity::set_alpha(double alpha) -> void
{
    m_alpha = alpha;
}

// MARK: - Rendering

auto kestrel::ecs::entity::draw() -> void
{
    auto scene = kestrel::session().current_scene();
    if (scene.get()) {
        auto internal_scene = scene->internal_scene();
        if (internal_scene.get()) {
            internal_scene->draw_entity(shared_from_this());
        }
    }
}

// MARK: - Physics

auto kestrel::ecs::entity::is_intersecting(const entity& subject) const -> bool
{
    return get_bounds().intersects(subject.get_bounds());
}