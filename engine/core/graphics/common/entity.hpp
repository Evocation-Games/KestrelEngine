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

#pragma once

#include <memory>
#include "math/size.hpp"
#include "math/point.hpp"
#include "core/graphics/common/spritesheet.hpp"
#include "renderer/common/blending.hpp"

class scene;

namespace graphics
{
    class entity: public std::enable_shared_from_this<graphics::entity>
    {
    public:
        explicit entity(const math::size& size);
        entity(const math::point& position, const math::size& size);

        ~entity();

        auto move_to_scene(const std::shared_ptr<class scene>& scene);
        auto scene() const -> std::weak_ptr<class scene>;

        [[nodiscard]] auto name() const -> std::string;
        auto set_name(const std::string& name) -> void;

        [[nodiscard]] auto id() const -> int64_t;
        auto set_id(int64_t id) -> void;

        auto set_sprite_sheet(const std::shared_ptr<graphics::spritesheet>& sheet, uint32_t sprite_index = 0) -> void;
        [[nodiscard]] auto sprite_sheet() const -> std::shared_ptr<graphics::spritesheet>;
        [[nodiscard]] auto texture() const -> std::shared_ptr<graphics::texture>;

        [[nodiscard]] auto get_sprite_index() const -> uint32_t;
        auto set_sprite_index(uint32_t index) -> void;

        [[nodiscard]] auto get_position() const -> math::point;
        auto set_position(const math::point& position) -> void;

        [[nodiscard]] auto get_bounds() const -> math::rect;

        [[nodiscard]] auto get_size() const -> math::size;
        auto set_size(const math::size& sz) -> void;

        [[nodiscard]] auto get_render_size() const -> math::size;
        auto set_render_size(const math::size& sz) -> void;

        auto set_clipping_area(const math::size& sz) -> void;
        auto remove_clipping_area() -> void;
        [[nodiscard]] auto has_clipping_area() const -> bool;
        [[nodiscard]] auto clipping_area() const -> math::size;
        [[nodiscard]] auto clipping_area_uv() const -> math::size;

        auto set_clipping_offset(const math::point& p) -> void;
        [[nodiscard]] auto clipping_offset() const -> math::point;
        [[nodiscard]] auto clipping_offset_uv() const -> math::point;

        [[nodiscard]] auto get_blend_lua() const -> int;
        auto set_blend_lua(int blend) -> void;
        [[nodiscard]] auto blend() const -> enum renderer::blending;

        [[nodiscard]] auto get_alpha() const -> double;
        auto set_alpha(double alpha) -> void;

        auto is_intersecting(const graphics::entity& subject) const -> bool;

        auto draw() -> void;

    private:
        std::weak_ptr<class scene> m_scene;

        int64_t m_id { INT64_MIN };
        std::string m_name;
        math::point m_position { 0 };
        math::size m_size { 0 };
        math::size m_render_size { 0 };
        uint32_t m_sprite_index { 0 };
        std::shared_ptr<graphics::spritesheet> m_sprite_sheet;
        enum renderer::blending m_blend { renderer::blending::normal };
        double m_alpha { 1.0 };

        bool m_has_texture_clip { false };
        math::size m_clipping_area { 0 };
        math::point m_clipping_offset { 0 };
        math::size m_clipping_area_uv { 0 };
        math::point m_clipping_offset_uv { 0 };
    };

};
