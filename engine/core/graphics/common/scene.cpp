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

#include "core/graphics/common/scene.hpp"
#include "core/graphics/common/entity.hpp"

// MARK: - Construction

graphics::scene::scene(const std::shared_ptr<graphics::session_window>& window, const scripting::lua::script &script)
    : m_owner(window), m_script(script)
{

}

auto graphics::scene::start() -> void
{
    m_script.execute();
}


// MARK: - Render/Physics

auto graphics::scene::update() -> void
{
    // To be implemented in a subclass
}

auto graphics::scene::render() -> void
{
    // To be implemented in a subclass
}

auto graphics::scene::draw_entity(const std::shared_ptr<graphics::entity>& entity) const -> void
{
    // To be implemented in a subclass
}

auto graphics::scene::add_render_block(const luabridge::LuaRef &block) -> void
{
    m_render_blocks.emplace_back(block);
}

auto graphics::scene::invoke_render_blocks() -> void
{
    for (const auto& block : m_render_blocks) {
        block();
    }
}