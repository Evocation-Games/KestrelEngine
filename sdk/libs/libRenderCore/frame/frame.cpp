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
{}

// MARK: - Submission / Drawing

auto renderer::frame::draw(ecs::entity entity, const ecs::world *world) -> void
{
    auto drawable = world->component<component::drawable>(entity);
    auto texturing = world->component<component::texturing>(entity);
    auto texture = (texturing != nullptr) ? texturing->texture : -1;
    if (!drawable || !m_renderer.tester.test(drawable->frame, texture, false)) {
        return;
    }

    // Determine if a new buffer is needed. This is if the buffer is full, or can not accept any
    // more textures.
    auto new_buffer_required = m_buffer.is_full();
    if (texturing) {
        new_buffer_required |= m_buffer.can_accept_texture(texturing->texture);
    }

    if (new_buffer_required) {
        // Submit the buffer and get a new one.
        submit_buffer();
    }

    auto entity_quad = get_quad(m_buffer);

    // Handle the drawable component first... this includes position and size.
     layout_quad(entity_quad, drawable);
     color_quad(entity_quad, drawable);

     if (texturing) {
         // We need to apply texture information to the quad.
         auto device_texture = m_renderer.driver->device_texture_id(texturing->texture);
         texture_quad(entity_quad, m_buffer.bind_texture(texturing->texture, device_texture), texturing);
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

auto renderer::frame::begin() -> frame&
{
    m_task.state = PREPARING;
    m_buffer.reset();
    return *this;
}

auto renderer::frame::finalize(std::function<auto(renderer::callback)->void> completion) -> void
{
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
