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

// MARK: - Lua

auto graphics::entity::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::entity>("Entity")
            .addFunction("draw", &entity::draw)
        .endClass();
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
