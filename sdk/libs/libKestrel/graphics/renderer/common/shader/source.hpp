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

#pragma once

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/macro.hpp>
#include <libKestrel/graphics/renderer/common/shader/program.hpp>

namespace kestrel::renderer::shader
{
    struct lua_api(Renderer.Shader, Available_0_8) source
    {
        is_resource_type("shdr");
        has_constructable_lua_api(source);

        lua_constructor(Available_0_8) explicit source(const resource::descriptor::lua_reference& descriptor);

        lua_getter(isLoaded, Avaialble_0_8) [[nodiscard]] auto is_loaded() const -> bool;
        [[nodiscard]] auto program() const -> std::shared_ptr<shader::program>;

    private:
        static constexpr const char *glsl = "glsl";
        static constexpr const char *mlsl = "mlsl";

    private:
        bool m_valid { false };
        std::shared_ptr<shader::program> m_program;
    };
}