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
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/graphics/sprites/sprite_sheet.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>
#include <libKestrel/util/uid.hpp>
#include <libKestrel/physics/body.hpp>
#include <libKestrel/graphics/renderer/common/shader/source.hpp>
#include <libKestrel/math/vec4.hpp>

namespace kestrel::ui
{
    class scene;
}

namespace kestrel::ecs
{
    /**
     * The `kestrel::ecs::entity` class represents an entity that can be rendered. The entity itself
     * manages all the information required by the renderer to add the entity to the render pipeline.
     */
    class entity: public std::enable_shared_from_this<entity>
    {
    public:
        /**
         * Construct a new entity of the specified size.
         * @param size The size of the entity in points.
         */
        explicit entity(const math::size& size);

        /**
         * Construct a new entity of the specified size, and at the specified position.
         * @param position  The position of the entity in points.
         * @param size      The size of the entity in points.
         */
        entity(const math::point& position, const math::size& size);

        /**
         * Destroy the entity.
         */
        ~entity();

        /**
         * Move the entity into the specified scene's render pipeline. This changes the scene that the entity will
         * submit its draw command to.
         * @param scene The scene that the entity should be drawn into.
         */
        auto move_to_scene(std::shared_ptr<kestrel::ui::scene>& scene) -> void;

        /**
         * The scene that the entity currently renders into.
         * @return  A scene reference.
         */
        [[nodiscard]] auto scene() const -> std::shared_ptr<kestrel::ui::scene>;

        /**
         * The name of the scene that can be used for debugging.
         * @return  The name of the scene.
         */
        [[nodiscard]] auto name() const -> std::string;

        /**
         * Set the name of the scene to be used for debugging purposes.
         * @param name  A scene name
         */
        auto set_name(const std::string& name) -> void;

        /**
         * The identifier of the scene.
         * @return  A unique identifier representing the scene
         */
        [[nodiscard]] auto id() const -> util::uid::value;

        /**
         * Set a new identifier for the scene.
         * @warning     This should be used cautiously as it may cause internal tracking issues.
         * @param id    A new unique identifier for the scene.
         */
        auto set_id(util::uid::value id) -> void;

        /**
         * The identifier of the scene.
         * @return  A unique identifier representing the scene
         */
        [[nodiscard]] auto uid() const -> util::uid;

        /**
         * Set a new identifier for the scene.
         * @warning     This should be used cautiously as it may cause internal tracking issues.
         * @param id    A new unique identifier for the scene.
         */
        auto set_uid(util::uid id) -> void;

        /**
         * Set the sprite sheet in which sprite texture information should be taken from. An initial sprite index can
         * also be provided to set the default/initial sprite being rendered.
         * @param sheet             A reference to the sprite sheet to be used by the entity.
         * @param sprite_index      The index of the sprite to be used.
         */
        auto set_sprite_sheet(const std::shared_ptr<graphics::sprite_sheet>& sheet, std::uint32_t sprite_index = 0) -> void;

        /**
         * The current sprite sheet for the entity.
         * @return  A reference to the current sprite sheet used by the entity.
         */
        [[nodiscard]] auto sprite_sheet() const -> std::shared_ptr<graphics::sprite_sheet>;

        /**
         * The texture used to render the entity. The texture is managed by the sprite sheet. If not sprite sheet
         * is bound to the entity, then it will have no texture information.
         * @return  A reference to the texture of the current sprite sheet.
         */
        [[nodiscard]] auto texture() const -> std::shared_ptr<graphics::texture>;

        /**
         * The index of the entities current sprite.
         * @return  A sprite sheet index.
         */
        [[nodiscard]] auto get_sprite_index() const -> std::uint32_t;

        /**
         * Change the current sprite index.
         * @param index A valid sprite index for the current sprite sheet.
         */
        auto set_sprite_index(std::uint32_t index) -> void;

        /**
         * The current position of the entity within the renderer.
         * @return  A point structure denoting the position in points.
         */
        [[nodiscard]] auto get_position() const -> math::point;

        /**
         * Set the current position of the entity within the renderer.
         * @param position  A point structure denoting the position in points.
         */
        auto set_position(const math::point& position) -> void;

        /**
         * Get the bounds of the entity within the renderer's coordinate space. This value is a combination
         * of the size and the position.
         * @return  A rect structure representing the bounds of the entity in points.
         */
        [[nodiscard]] auto get_bounds() const -> math::rect;

        /**
         * The size of the entity within the renderer.
         * @return  A size structure denoting the size in points.
         */
        [[nodiscard]] auto get_size() const -> math::size;

        /**
         * Set the size of the entity within the renderer.
         * @param sz  A size structure denoting the size in points.
         */
        auto set_size(const math::size& sz) -> void;

        auto set_clipping_area(const math::size& sz) -> void;
        auto remove_clipping_area() -> void;
        [[nodiscard]] auto has_clipping_area() const -> bool;
        [[nodiscard]] auto clipping_area() const -> math::size;
        [[nodiscard]] auto clipping_area_uv() const -> math::size;

        auto set_clipping_offset(const math::point& p) -> void;
        [[nodiscard]] auto clipping_offset() const -> math::point;
        [[nodiscard]] auto clipping_offset_uv() const -> math::point;

        [[nodiscard]] auto has_scaled_texture() const -> bool;
        auto set_scaled_texture_area(const math::rect&) -> void;
        [[nodiscard]] auto scaled_texture_area() const -> math::rect;

        [[nodiscard]] auto ignores_scene_scaling_factor() const -> bool;
        auto set_ignores_scene_scaling_factor(bool f) -> void;

        [[nodiscard]] auto get_blend_lua() const -> int;
        auto set_blend_lua(int blend) -> void;
        [[nodiscard]] auto blend() const -> enum renderer::blending;

        [[nodiscard]] auto get_alpha() const -> double;
        auto set_alpha(double alpha) -> void;

        auto is_intersecting(const ecs::entity& subject) const -> bool;

        [[nodiscard]] auto hitbox_color() const -> graphics::color::lua_reference;
        auto set_hitbox_color(const graphics::color::lua_reference& color) -> void;

        [[nodiscard]] auto shader() const -> renderer::shader::source::lua_reference;
        auto set_shader(const renderer::shader::source::lua_reference& shader) -> void;
        auto bind_shader_attachment(std::int32_t idx, double v1) -> void;
        auto bind_shader_attachment2(std::int32_t idx, double v1, double v2) -> void;
        auto bind_shader_attachment3(std::int32_t idx, double v1, double v2, double v3) -> void;
        auto bind_shader_attachment4(std::int32_t idx, double v1, double v2, double v3, double v4) -> void;

        [[nodiscard]] auto shader_attachments() const -> std::array<math::vec4, 8>;

        auto draw() -> void;
        auto update() -> void;

        auto body() -> physics::body::lua_reference;

    private:
        std::weak_ptr<kestrel::ui::scene> m_scene;

        util::uid m_id;
        std::string m_name;
        math::point m_position { 0 };
        math::size m_size { 0 };
        std::uint32_t m_sprite_index { 0 };
        std::shared_ptr<graphics::sprite_sheet> m_sprite_sheet;
        enum renderer::blending m_blend { renderer::blending::normal };
        double m_alpha { 1.0 };
        bool m_has_texture_clip { false };
        bool m_has_scaled_texture { false };
        bool m_ignores_scene_scaling_factor { false };
        math::rect m_scaled_texture { math::point(0), math::size(0) };
        math::rect m_scaled_texture_uv { math::point(0), math::size(0) };
        math::size m_clipping_area { 0 };
        math::point m_clipping_offset { 0 };
        math::size m_clipping_area_uv { 0 };
        math::point m_clipping_offset_uv { 0 };
        graphics::color::lua_reference m_hitbox_color { graphics::color::green_color_ref() };
        physics::body::lua_reference m_body { new physics::body() };
        renderer::shader::source::lua_reference m_shader { nullptr };
        std::array<math::vec4, 8> m_shader_attachments;
    };

};
