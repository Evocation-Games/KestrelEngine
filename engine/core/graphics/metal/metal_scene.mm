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

#include <memory>
#include "core/graphics/metal/metal_scene.h"
#include "core/graphics/metal/metal_session_window.h"

// MARK: - Construction

graphics::metal::scene::scene(const std::shared_ptr<graphics::session_window> &window, const scripting::lua::script &script)
    : graphics::scene(window, script)
{

}

// MARK: - Render/Physics

auto graphics::metal::scene::update() -> void
{

}

auto graphics::metal::scene::render() -> void
{
    invoke_render_blocks();
}

auto graphics::metal::scene::draw_entity(const graphics::entity::lua_reference& entity) const -> void
{
    if (auto owner = m_owner.lock()) {
        auto metal_window = std::static_pointer_cast<metal::session_window>(owner);
        metal_window->draw_entity(entity);
    }
}