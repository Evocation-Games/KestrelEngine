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

#include "core/graphics/common/session_window.hpp"
#include "core/graphics/common/scene.hpp"
#include "core/clock/clock.hpp"
#include "core/audio/audio_manager.hpp"

// MARK: - Construction

graphics::session_window::session_window(std::shared_ptr<environment> env, const double& scale)
    : m_environment(env), m_scale(scale)
{
    m_cache_purge_time = session_clock::now();
}

// MARK: - Accessors

auto graphics::session_window::is_running() const -> bool
{
    return m_alive;
}

auto graphics::session_window::sprite_shader() -> std::shared_ptr<graphics::shader>
{
    return m_sprite_shader;
}

auto graphics::session_window::set_title(const std::string &title) -> void
{

}

auto graphics::session_window::set_size(const math::size &size) -> void
{
    m_size = size;
    m_console.set_size(size);
}

auto graphics::session_window::get_size() const -> math::size
{
    return m_size;
}

auto graphics::session_window::get_scale_factor() const -> double
{
    return m_scale;
}


// MARK: - Main Loop

auto graphics::session_window::tick() -> void
{
    rtc::clock::global().tick();
    update();
    render();
}

// MARK: - Render/Physics Base

auto graphics::session_window::update() -> void
{
    if (m_scenes.empty()) {
        return;
    }
    audio::manager::shared_manager().tick();
    current_scene()->check_timed_events();
    current_scene()->update();

    if (auto env = m_environment.lock()) {
        if ((session_clock::now() - m_cache_purge_time).count() >= 60) {
             env->cache()->purge_unused();
             env->gc_purge();
        }
    }
}

auto graphics::session_window::render() -> void
{
    if (m_scenes.empty()) {
        return;
    }

    auto base_scene = 0;
    for (auto i = m_scenes.size() - 1; i >= 0; --i) {
        base_scene = static_cast<int>(i);
        if (!m_scenes[i]->get_passthrough_render()) {
            break;
        }
    }

    for (auto i = base_scene; i < m_scenes.size(); ++i) {
        m_scenes[i]->render();
    }

    if (m_console.is_visible()) {
        m_console.update();
        m_console.entity()->draw();
    }
}

// MARK: - Scene Management

auto graphics::session_window::current_scene() const -> std::shared_ptr<graphics::scene>
{
    if (m_scenes.empty()) {
        return nullptr;
    }
    return m_scenes.back();
}

auto graphics::session_window::new_scene(const std::string& name, const scripting::lua::script &script) -> std::shared_ptr<graphics::scene>
{
    throw std::logic_error("session_window::new_scene must be overridden in a subclass.");
}

auto graphics::session_window::present_scene(std::shared_ptr<graphics::scene> scene) -> void
{
    m_scenes.emplace_back(scene);
    scene->start();
}

auto graphics::session_window::pop_scene() -> void
{
    if (m_scenes.size() > 1) {
        m_scenes.pop_back();
    }
}


// MARK: - Helpers

auto graphics::session_window::create_texture(const math::size &size,
                                              std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture>
{
    throw std::runtime_error("This method needs to be implemented in a subclass.");
}

auto graphics::session_window::create_texture(const math::size &size, const uint8_t *data) const -> std::shared_ptr<graphics::texture>
{
    throw std::runtime_error("This method needs to be implemented in a subclass.");
}

// MARK: - Console

auto graphics::session_window::console() -> dev::console&
{
    return m_console;
}
