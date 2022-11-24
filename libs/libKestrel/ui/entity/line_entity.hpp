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

#include <vector>
#include <variant>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/vec4.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>
#include <libKestrel/graphics/renderer/common/shader/source.hpp>

namespace kestrel::ui
{
    struct lua_api(LineEntity, Available_0_8) line_entity
    {
    public:
        has_constructable_lua_api(line_entity);

        line_entity() = default;

        lua_constructor(Avaiable_0_8) line_entity(const math::point& start, const math::point& end, double width);
        explicit line_entity(const std::shared_ptr<line_entity>& entity);

        lua_getter(startPosition, Available_0_8) [[nodiscard]] auto start() const -> math::point;
        lua_setter(startPosition, Available_0_8) auto set_start(const math::point& start) -> void;
        lua_getter(endPosition, Available_0_8) [[nodiscard]] auto end() const -> math::point;
        lua_setter(endPosition, Available_0_8) auto set_end(const math::point& start) -> void;

        lua_getter(drawingStartPosition, Available_0_8) [[nodiscard]] auto draw_start() const -> math::point;
        lua_setter(drawingStartPosition, Available_0_8) auto set_draw_start(const math::point& start) -> void;
        lua_getter(drawingEndPosition, Available_0_8) [[nodiscard]] auto draw_end() const -> math::point;
        lua_setter(drawingEndPosition, Available_0_8) auto set_draw_end(const math::point& start) -> void;

        lua_getter(weight, Available_0_8) [[nodiscard]] auto weight() const -> double;
        lua_setter(weight, Available_0_8) auto set_weight(double weight) -> void;

        lua_getter(color, Available_0_8) [[nodiscard]] auto color() const -> graphics::color::lua_reference;
        lua_setter(color, Available_0_8) auto set_color(const graphics::color::lua_reference& color) -> void;

        lua_getter(blend, Available_0_8) [[nodiscard]] auto blend_mode() const -> std::int32_t;
        lua_setter(blend, Available_0_8) auto set_blend_mode(std::int32_t mode) -> void;

        lua_getter(alpha, Available_0_8) [[nodiscard]] auto alpha() const -> double;
        lua_setter(alpha, Available_0_8) auto set_alpha(double alpha) -> void;

        lua_getter(shader, Available_0_8) [[nodiscard]] auto shader() const -> renderer::shader::source::lua_reference;
        lua_setter(shader, Available_0_8) auto set_shader(const renderer::shader::source::lua_reference& shader) -> void;
        lua_function(bindShaderAttachment, Available_0_8) auto bind_shader_attachment(std::int32_t idx, double v1) -> void;
        lua_function(bindShaderAttachment2, Available_0_8) auto bind_shader_attachment2(std::int32_t idx, double v1, double v2) -> void;
        lua_function(bindShaderAttachment3, Available_0_8) auto bind_shader_attachment3(std::int32_t idx, double v1, double v2, double v3) -> void;
        lua_function(bindShaderAttachment4, Available_0_8) auto bind_shader_attachment4(std::int32_t idx, double v1, double v2, double v3, double v4) -> void;

        lua_function(layout, Available_0_8) auto layout() -> void;
        lua_function(onLayout, Available_0_8) auto on_layout(const luabridge::LuaRef& callback) -> void;
        lua_function(draw, Available_0_8) auto draw() -> void;

    private:
        math::point m_start;
        math::point m_draw_start;
        math::point m_end;
        math::point m_draw_end;
        double m_weight { 1.0 };
        graphics::color::lua_reference m_color { graphics::color::white_color_ref() };
        enum renderer::blending m_blend_mode { renderer::blending::normal };
        double m_alpha { 1.0 };
        luabridge::LuaRef m_on_layout { nullptr };
        renderer::shader::source::lua_reference m_shader { nullptr };
        std::array<math::vec4, 13> m_shader_attachments;
    };
}