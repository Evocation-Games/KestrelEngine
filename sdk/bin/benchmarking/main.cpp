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
#include <libFoundation/profile/profiler.hpp>
#include <libECS/world/world.hpp>

struct component_a
{
    std::int64_t foo { 0 };
};

struct component_b
{
    std::int64_t bar { 0 };
};

auto main(std::int32_t argc, const char **argv) -> std::int32_t
{
#pragma region ECS
    std::cout << "================== ECS ==================" << std::endl;
    ecs::world sut;

    foundation::profiler::measure("ECS Prepare (1M Entity Capacity)", [&sut] {
        sut.prepare();
    });

    foundation::profiler::measure("ECS Setup Components (2)", [&sut] {
        sut.register_component<component_a>();
        sut.register_component<component_b>();
    });

    foundation::profiler::measure("ECS Create Entities (100k)", [&sut] {
        for (auto i = 0; i < 100'000; ++i) {
            auto entity = sut.create_entity();

            struct component_a a;
            a.foo = i;
            struct component_b b;
            b.bar = i + i;

            sut.add_component(entity, a);
            sut.add_component(entity, b);
        }
    });


    foundation::profiler::measure("ECS Get Components (100k entities)", [&sut] {
        for (auto i = 0; i < 100'000; ++i) {
            auto component = sut.component<component_b>(i);
            component->bar = i;
        }
    });

    std::cout << "=========================================" << std::endl;
    std::cout << std::endl;
#pragma endregion


    return 0;
}