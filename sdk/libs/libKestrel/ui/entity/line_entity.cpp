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

#include <libKestrel/ui/entity/line_entity.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

// MARK: - Construction

kestrel::ui::line_entity::line_entity(const math::point &start, const math::point &end, double weight)
    : m_start(start), m_end(end), m_weight(weight)
{}

kestrel::ui::line_entity::line_entity(const std::shared_ptr<line_entity> &entity)
    : m_weight(entity->m_weight),
      m_start(entity->m_start),
      m_draw_start(entity->m_draw_start),
      m_end(entity->m_end),
      m_draw_end(entity->m_draw_end),
      m_color(entity->m_color),
      m_blend_mode(entity->m_blend_mode),
      m_alpha(entity->m_alpha)
{
    if (entity->m_on_layout.state() && entity->m_on_layout.isFunction()) {
        m_on_layout = entity->m_on_layout;
    }
}

// MARK: - Accessors

auto kestrel::ui::line_entity::start() const -> math::point
{
    return m_start;
}

auto kestrel::ui::line_entity::set_start(const math::point &start) -> void
{
    m_start = start;
}

auto kestrel::ui::line_entity::end() const -> math::point
{
    return m_end;
}

auto kestrel::ui::line_entity::set_end(const math::point &end) -> void
{
    m_end = end;
}

auto kestrel::ui::line_entity::draw_start() const -> math::point
{
    return m_draw_start;
}

auto kestrel::ui::line_entity::set_draw_start(const math::point &start) -> void
{
    m_draw_start = start;
}

auto kestrel::ui::line_entity::draw_end() const -> math::point
{
    return m_draw_end;
}

auto kestrel::ui::line_entity::set_draw_end(const math::point &end) -> void
{
    m_draw_end = end;
}

auto kestrel::ui::line_entity::weight() const -> double
{
    return m_weight;
}

auto kestrel::ui::line_entity::set_weight(double weight) -> void
{
    m_weight = weight;
}

auto kestrel::ui::line_entity::color() const -> graphics::color::lua_reference
{
    return m_color;
}

auto kestrel::ui::line_entity::set_color(const graphics::color::lua_reference &color) -> void
{
    m_color = color;
}

auto kestrel::ui::line_entity::blend_mode() const -> std::int32_t
{
    return static_cast<std::int32_t>(m_blend_mode);
}

auto kestrel::ui::line_entity::set_blend_mode(std::int32_t mode) -> void
{
    m_blend_mode = static_cast<enum renderer::blending>(mode);
}

auto kestrel::ui::line_entity::alpha() const -> double
{
    return m_alpha;
}

auto kestrel::ui::line_entity::set_alpha(double alpha) -> void
{
    m_alpha = alpha;
}

// MARK: - Layout

auto kestrel::ui::line_entity::layout() -> void
{
    if (m_on_layout.state() && m_on_layout.isFunction()) {
        m_on_layout(this);
    }
}

auto kestrel::ui::line_entity::on_layout(const luabridge::LuaRef &callback) -> void
{
    if (callback.state() && callback.isFunction()) {
        m_on_layout = callback;
    }
    else {
        m_on_layout = { nullptr };
    }
}

auto kestrel::ui::line_entity::shader() const -> renderer::shader::source::lua_reference
{
    return m_shader;
}

auto kestrel::ui::line_entity::set_shader(const renderer::shader::source::lua_reference &shader) -> void
{
    m_shader = shader;
}

auto kestrel::ui::line_entity::bind_shader_attachment(std::int32_t idx, double v1) -> void
{
    m_shader_attachments[idx] = math::vec4(static_cast<float>(v1), 0.f, 0.f, 0.f);
}

auto kestrel::ui::line_entity::bind_shader_attachment2(std::int32_t idx, double v1, double v2) -> void
{
    m_shader_attachments[idx] = math::vec4(static_cast<float>(v1), static_cast<float>(v2), 0.f, 0.f);
}

auto kestrel::ui::line_entity::bind_shader_attachment3(std::int32_t idx, double v1, double v2, double v3) -> void
{
    m_shader_attachments[idx] = math::vec4(static_cast<float>(v1), static_cast<float>(v2), static_cast<float>(v3), 0.f);
}

auto kestrel::ui::line_entity::bind_shader_attachment4(std::int32_t idx, double v1, double v2, double v3, double v4) -> void
{
    m_shader_attachments[idx] = math::vec4(static_cast<float>(v1), static_cast<float>(v2), static_cast<float>(v3), static_cast<float>(v4));
}

// MARK: - Drawing

auto kestrel::ui::line_entity::draw() -> void
{
    renderer::draw_line(m_draw_start, m_draw_end, m_blend_mode, *m_color.get(), m_weight, m_shader.get() ? m_shader->program() : nullptr, m_shader_attachments);
}
