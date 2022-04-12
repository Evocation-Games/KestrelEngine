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

#include "core/ui/session.hpp"
#include "core/ui/scene.hpp"
#include "core/ui/game_scene.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Construction

ui::session::session()
{
    m_cache_purge_time = session_clock::now();
}

// MARK: - Scene Management

auto ui::session::current_scene() const -> game_scene::lua_reference
{
    if (m_scenes.empty()) {
        return nullptr;
    }
    return m_scenes.back();
}

auto ui::session::new_scene(const std::string &name, const scripting::lua::script &script) -> std::shared_ptr<scene>
{
    auto ptr = shared_from_this();
    return std::make_shared<scene>(ptr, script);
}

auto ui::session::present_scene(const game_scene::lua_reference& scene) -> void
{
    m_scenes.emplace_back(scene);
    scene->start();
}

auto ui::session::pop_scene() -> void
{
    if (m_scenes.size() > 1) {
        m_scenes.pop_back();
    }
}

// MARK: - Properties

auto ui::session::set_title(const std::string &title) -> void
{
    renderer::set_window_title(title);
}

auto ui::session::set_size(const math::size &size) -> void
{
    renderer::set_window_size(size);
}

auto ui::session::size() const -> math::size
{
    return renderer::window_size();
}

// MARK: - Updates / Events

auto ui::session::tick() -> void
{
    auto scene = this->current_scene();
    if (scene.get()) {
        scene->internal_scene()->update();
        scene->internal_scene()->render();
    }

    if (m_console.is_visible()) {
        m_console.update();
        m_console.entity()->draw();
    }
}

auto ui::session::receive_event(const event &e) -> void
{
    if (e.is_key_event()) {
        if (e.is_released() && e.key() == hid::f1) {
            m_console.set_size({ size().width, size().height / 2 });
            m_console.toggle_visibility();
            return;
        }
        if (m_console.is_visible()) {
            m_console.receive(e);
            return;
        }
    }

    auto scene = this->current_scene();
    if (scene.get()) {
        scene->internal_scene()->receive_event(e);
    }
}
