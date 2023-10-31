// Copyright (c) 2023 Tom Hancocks
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

#include <thread>
#include <unordered_map>
#include <algorithm>
#include <libRenderCore/frame/frame.hpp>
#include <libRenderCore/components/drawable.hpp>
#include <libRenderCore/components/texturing.hpp>
#include <libRenderCore/driver/driver.hpp>
#include <libFoundation/profile/profiler.hpp>

// MARK: - Construction

renderer::frame::frame(const renderer::driver *driver)
{
    m_renderer.driver = driver;
}

auto renderer::frame::initialize() -> void
{
    m_buffer.initialize();
    m_unsorted.entities = std::vector<std::pair<ecs::entity, const ecs::world *>>( 500'000 );
}

// MARK: - Submission / Drawing

auto renderer::frame::draw(ecs::entity entity, const ecs::world *world) -> void
{
    // We only want to add drawable entities to the draw queue.
    if (auto drawable = world->component<component::drawable>(entity)) {
        // Before adding there are a couple of things that should be considered.
        // Does the entity lay within the cameras frustum? If it does not, then simply
        // ignore it.
        if (!m_renderer.bounds.intersects(drawable->frame)) {
            return;
        }

        // We have confirmed the entity has a chance of actually being drawn, so
        // add it to the list.
        auto& slot = m_unsorted.entities[m_unsorted.index++];
        slot.first = entity;
        slot.second = world;
        m_unsorted.count++;
    }
}

auto renderer::frame::sort_entities() -> void
{
    KESTREL_PROFILE_FUNCTION();
    // We have received a list of entities that are candidates for being drawn.
    for (auto slot_index = 0; slot_index < m_unsorted.count; ++slot_index) {
        const auto& slot = m_unsorted.entities[slot_index];
        auto new_buffer_required = m_buffer.is_full();

        const auto *drawable = slot.second->component<component::drawable>(slot.first);
        const auto *texturing = slot.second->component<component::texturing>(slot.first);
        new_buffer_required |= !m_buffer.can_accept_texture(texturing);

        if (new_buffer_required) {
            submit_buffer();
        }

        auto quad = get_quad(m_buffer);
        layout_quad(quad, drawable);
        color_quad(quad, drawable);

        if (texturing) {
            auto device_texture = m_renderer.driver->device_texture_id(texturing->texture);
            texture_quad(quad, m_buffer.bind_texture(texturing->texture, device_texture), texturing);
        }
    }
}

auto renderer::frame::submit_buffer() ->  void
{
    KESTREL_PROFILE_FUNCTION();
    m_renderer.driver->draw(m_buffer);
    m_buffer.reset();
    m_renderer.tester.reset();
}

// MARK: - Lifetime

auto renderer::frame::begin(math::vec2 bounds) -> frame&
{
    m_renderer.bounds = { math::vec2(0), bounds };
    m_task.state = PREPARING;
    m_buffer.reset();
    m_unsorted.index = 0;
    m_unsorted.count = 0;
    return *this;
}

auto renderer::frame::finalize(std::function<auto(renderer::callback)->void> completion) -> void
{
    sort_entities();
    submit_buffer();
    m_task.completion = std::move(completion);
    m_task.state = SUBMITTED;
    m_task.completion([&] {
        // We have finished rendering the frame!
        m_task.state = READY;
    });
}

// MARK: - Draw Calls

auto renderer::frame::get_quad(renderer::buffer &buffer) -> quad
{
    frame::quad q;
    q.vertices[0] = &buffer.next_vertex();
    q.vertices[1] = &buffer.next_vertex();
    q.vertices[2] = &buffer.next_vertex();
    q.vertices[3] = &buffer.next_vertex();
    q.vertices[4] = &buffer.next_vertex();
    q.vertices[5] = &buffer.next_vertex();
    return q;
}

auto renderer::frame::layout_quad(renderer::frame::quad &q, const component::drawable *drawable) -> void
{
    auto px = drawable->frame.x();
    auto py = drawable->frame.y();
    auto sw = drawable->frame.width();
    auto sh = drawable->frame.height();

    // Set the coordinates for each of the vertexes in the quad.
    q.vertices[0]->position = renderer::vec4(px, py + sh, 0, 0);
    q.vertices[1]->position = renderer::vec4(px + sw, py + sh, 0, 0);
    q.vertices[2]->position = renderer::vec4(px + sw, py, 0, 0);
    q.vertices[3]->position = renderer::vec4(px, py + sh, 0, 0);
    q.vertices[4]->position = renderer::vec4(px, py, 0, 0);
    q.vertices[5]->position = renderer::vec4(px + sw, py, 0, 0);
}

auto renderer::frame::color_quad(renderer::frame::quad& q, const component::drawable *drawable) -> void
{
    auto color = renderer::vec4(drawable->color.x(), drawable->color.y(), drawable->color.z(), drawable->color.w());

    q.vertices[0]->color = color;
    q.vertices[1]->color = color;
    q.vertices[2]->color = color;
    q.vertices[3]->color = color;
    q.vertices[4]->color = color;
    q.vertices[5]->color = color;

    q.vertices[0]->texture = -1;
    q.vertices[1]->texture = -1;
    q.vertices[2]->texture = -1;
    q.vertices[3]->texture = -1;
    q.vertices[4]->texture = -1;
    q.vertices[5]->texture = -1;
}

auto renderer::frame::texture_quad(renderer::frame::quad& q,buffer::texture_slot slot, const component::texturing *texturing) -> void
{
    auto uv_x = texturing->uv_origin.x();
    auto uv_y = texturing->uv_origin.y();
    auto uv_w = texturing->uv_size.x();
    auto uv_h = texturing->uv_size.y();

    q.vertices[0]->texture = static_cast<float>(slot);
    q.vertices[1]->texture = static_cast<float>(slot);
    q.vertices[2]->texture = static_cast<float>(slot);
    q.vertices[3]->texture = static_cast<float>(slot);
    q.vertices[4]->texture = static_cast<float>(slot);
    q.vertices[5]->texture = static_cast<float>(slot);

    q.vertices[0]->tex_coord = renderer::vec2(uv_x, uv_y + uv_h);
    q.vertices[1]->tex_coord = renderer::vec2(uv_x + uv_w, uv_y + uv_h);
    q.vertices[2]->tex_coord = renderer::vec2(uv_x + uv_w, uv_y);
    q.vertices[3]->tex_coord = renderer::vec2(uv_x, uv_y + uv_h);
    q.vertices[4]->tex_coord = renderer::vec2(uv_x, uv_y);
    q.vertices[5]->tex_coord = renderer::vec2(uv_x + uv_w, uv_y);
}
