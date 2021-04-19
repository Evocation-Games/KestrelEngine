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
#include <memory>
#include "core/graphics/opengl/opengl_scene.hpp"
#include "core/graphics/opengl/opengl_session_window.hpp"

// MARK: - Construction

graphics::opengl::scene::scene(const std::shared_ptr<graphics::session_window>& window, const scripting::lua::script &script, const std::string& name)
    : graphics::scene(window, script, name)
{

}

// MARK: - Render/Physics

auto graphics::opengl::scene::update() -> void
{
    invoke_update_blocks();
}

auto graphics::opengl::scene::render() -> void
{
    invoke_render_blocks();
}

auto graphics::opengl::scene::draw_entity(const graphics::entity::lua_reference& entity) const -> void
{
    // Acquire a reference of the sprite renderer
    if (auto owner = m_owner.lock()) {
        auto opengl_window = std::static_pointer_cast<opengl::session_window>(owner);
        if (opengl_window->sprite_renderer().has_value()) {
            auto renderer = opengl_window->sprite_renderer();
            renderer->draw(entity);
        }
    }
}
