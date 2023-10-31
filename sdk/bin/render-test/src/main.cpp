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

#include <iostream>
#include <chrono>
#include <libRenderCore/driver/driver.hpp>
#include <libECS/world/world.hpp>
#include <libRenderCore/components/drawable.hpp>
#include <libFoundation/profile/profiler.hpp>
#include <libFoundation/system/filesystem/path.hpp>
#include <libImage/codecs/png/png.hpp>

struct render_test: public event::receiver
{
    render_test() = default;

    auto start() -> void
    {
        setup_ecs();
        setup_renderer();
        std::cout << "Using driver: " << m_driver.driver_name() << std::endl;
        std::cout << "GPU: " << m_driver.gpu_name() << std::endl;
    }

    auto setup_renderer() -> void
    {
        m_driver.set_event_receiver(this);
        m_driver.start([&] (auto& frame) {
            generate_frame(frame);
        });
    }

    auto setup_ecs() -> void
    {
       m_world.prepare();
       m_world.register_component<renderer::component::drawable>();
       m_world.register_component<renderer::component::texturing>();

        // Create 10 entities (identical squares)
        for (auto i = 0; i < m_entity_count; ++i) {
            auto entity = m_world.create_entity();
            renderer::component::drawable drawable;
            drawable.color = math::vec4(1.0, 0, 0, 1.0);
            drawable.visible = true;
            drawable.frame = math::geometry::rect({ 100, 100 }, { 100, 100 });
            m_world.add_component<renderer::component::drawable>(entity, drawable);
        }

        // Create 10 identical pentagons
        auto texture = m_driver.create_texture(load_image_asset("pentagon"), { 100, 100 });
        for (auto i = 0; i < m_entity_count; ++i) {
            auto entity = m_world.create_entity();
            renderer::component::drawable drawable;
            drawable.color = math::vec4(1.0, 1.0, 1.0, 1.0);
            drawable.visible = true;
            drawable.frame = math::geometry::rect({ 300, 100 }, { 100, 100 });

            renderer::component::texturing texturing;
            texturing.texture = texture;

            m_world.add_component<renderer::component::drawable>(entity, drawable);
            m_world.add_component<renderer::component::texturing>(entity, texturing);
        }
    }

    auto receive(event::instance raw) -> void override
    {

    }

    auto generate_frame(renderer::frame& frame) -> void
    {
        KESTREL_PROFILE_FUNCTION();
        for (auto i = 20; i > 0; --i) {
            frame.draw(i - 1, &m_world);
        }
    }

    auto load_image_asset(const std::string& name) -> data::block
    {
        foundation::filesystem::path path("../sdk/bin/render-test/asset/pentagon.png");
        image::codec::png png(path);
        auto& surface = png.surface();
        return surface.raw();
    }

private:
    renderer::display_configuration m_cfg;
    renderer::driver m_driver;
    ecs::world m_world;
    std::uint64_t m_entity_count { 10 };
};

auto main(std::int32_t argc, const char **argv) -> std::int32_t
{
    render_test app;
    app.start();
    return 0;
}