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

#include <libKestrel/session/session.hpp>
#include <libKestrel/ui/scene/scene.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>
#include <libKestrel/lua/script.hpp>
#include <libKestrel/device/console.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/exceptions/lua_runtime_exception.hpp>

// MARK: - Construction

kestrel::ui::session::session()
{
    m_cache.purge_time = session_clock::now();

    device::console::pipe_output([&] (const struct device::console::record& record) {
        if (record.status == device::console::status::input) {
            lua::script command(kestrel::lua_runtime(), record.text);

            try {
                command.execute();
            }
            catch (lua_runtime_exception& e) {
                device::console::write(e.what(), device::console::status::error);
            }
        }
    });
}

// MARK: - Destruction

kestrel::ui::session::~session()
{
    // Do nothing. Engine tearing down.
}

// MARK: - Scene Management

auto kestrel::ui::session::current_scene() const -> game_scene::lua_reference
{
    if (m_scenes.empty()) {
        return nullptr;
    }
    return m_scenes.back();
}

auto kestrel::ui::session::new_scene(const std::string &name, const lua::script &script) -> std::shared_ptr<scene>
{
    auto scene = std::make_shared<class scene>(script);
    return scene;
}

auto kestrel::ui::session::present_scene(const game_scene::lua_reference& scene) -> void
{
    m_scenes.emplace_back(scene);
    scene->start();
}

auto kestrel::ui::session::pop_scene() -> void
{
    if (m_scenes.size() > 1) {
        m_scenes.pop_back();
    }
}

// MARK: - Properties

auto kestrel::ui::session::set_title(const std::string &title) -> void
{
    renderer::set_window_title(title);
}

auto kestrel::ui::session::set_size(const math::size &size) -> void
{
    renderer::set_window_size(size);
}

auto kestrel::ui::session::size() const -> math::size
{
    return renderer::window_size();
}

// MARK: - Updates / Events

auto kestrel::ui::session::tick(bool render, bool update) -> void
{
    auto scene = this->current_scene();

    if (update && scene.get()) {
        m_update.last_time = rtc::clock::global().since(m_update.start_time).count();
        m_update.start_time = rtc::clock::global().current();
        scene->internal_scene()->update();
    }

    if (render) {
        auto base_scene = 0;
        for (auto i = m_scenes.size() - 1; i >= 0; --i) {
            base_scene = static_cast<int>(i);
            if (!m_scenes[i]->passthrough_render()) {
                break;
            }
        }

        for (auto i = base_scene; i < m_scenes.size() - 1; ++i) {
            m_scenes[i]->internal_scene()->render();
        }

        if (scene.get()) {
            scene->internal_scene()->render();

            // TODO: Fix console renderering
//            if (m_console.console.is_visible() ) {
//                m_console.console.entity()->set_render_size({
//                    scene->size().width,
//                    scene->size().height / 2
//                });
//
//                m_console.console.entity()->set_draw_size({
//                    scene->size().width,
//                    scene->size().height / 2
//                });
//
//                m_console.console.update();
//                m_console.console.entity()->draw();
//            }
        }

    }
}

auto kestrel::ui::session::receive_event(const event &e) -> void
{
    if (e.is_key_event()) {
        if (e.has(event_type::key_up) && e.is(hid::f1)) {
//            m_console.console.set_size({ size().width, size().height / 2 });
//            m_console.console.toggle_visibility();
            return;
        }
//        if (m_console.console.is_visible()) {
//            m_console.console.receive(e);
//            return;
//        }
    }

    auto scene = this->current_scene();
    if (scene.get()) {
        scene->internal_scene()->receive_event(e);
    }
}
