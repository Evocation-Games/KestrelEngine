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

struct event_reciever: public event::receiver
{
    auto receive(event::instance raw) -> void override
    {

    }
};

auto main(std::int32_t argc, const char **argv) -> std::int32_t
{
    renderer::display_configuration cfg;
    renderer::driver driver;

    std::cout << "Using driver: " << driver.driver_name() << std::endl;
    std::cout << "GPU: " << driver.gpu_name() << std::endl;

    ecs::world world;
    world.prepare();
    world.register_component<renderer::component::drawable>();
    world.register_component<renderer::component::texturing>();

    driver.set_event_receiver(new event_reciever());

    // Create 100 entities
    auto entity_count = 100;
    for (auto i = 0; i < entity_count; ++i) {
        auto entity = world.create_entity();
        renderer::component::drawable drawable;
        drawable.color = math::vec4(1.0, 0, 0, 1.0);
        drawable.visible = true;
        drawable.frame = math::geometry::rect({ 100, 100 }, { 100, 100 });
        world.add_component<renderer::component::drawable>(entity, drawable);
    }

    driver.start([&world, entity_count] (auto& frame) {
        KESTREL_PROFILE_FUNCTION();
        for (auto i = entity_count; i > 0; --i) {
            frame.draw(i - 1, &world);
        }
    });

    return 0;
}