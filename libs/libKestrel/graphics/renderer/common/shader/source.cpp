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

#include <stdexcept>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/graphics/renderer/common/context.hpp>
#include <libKestrel/graphics/renderer/common/shader/source.hpp>
#include <libKestrel/resource/reader.hpp>

// MARK: - Construction

kestrel::renderer::shader::source::source(const resource::descriptor::lua_reference &descriptor)
{
    // Read the basic shader first, and then each of the specific implementations
    auto reader = resource::reader(descriptor->with_type(resource_type::code));
    if (!reader.valid() || reader.name().empty()) {
        return;
    }

    auto glsl = reader.read_resource_reference();
    auto mlsl = reader.read_resource_reference();

    if (kestrel::renderer_api() == static_cast<std::int32_t>(renderer::api::opengl)) {
        auto glsl_shader = resource::reader(glsl->with_type(source::glsl));
        if (glsl_shader.valid()) {
            auto glsl_vertex = glsl_shader.read_cstr();
            auto glsl_fragment = glsl_shader.read_cstr();
        }
    }
    #if TARGET_MACOS
    else if (kestrel::renderer_api() == static_cast<std::int32_t>(renderer::api::metal)) {
        auto mlsl_shader = resource::reader(mlsl->with_type(source::mlsl));
        if (mlsl_shader.valid()) {
            auto mlsl_vertex = mlsl_shader.read_cstr();
            auto mlsl_fragment = mlsl_shader.read_cstr();

            m_program = renderer::current_context()->create_shader_library(reader.name(), mlsl_vertex, mlsl_fragment);
            m_valid = true;
        }
    }
    #endif
}

// MARK: - Accessors

auto kestrel::renderer::shader::source::is_loaded() const -> bool
{
    return m_valid && (m_program != nullptr);
}

auto kestrel::renderer::shader::source::program() const -> std::shared_ptr<shader::program>
{
    return m_program;
}