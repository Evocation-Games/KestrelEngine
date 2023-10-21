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

#include <string>
#include <libData/block.hpp>
#include <libResourceCore/structure/instance.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/graphics/sprites/sprite_sheet.hpp>
#include <libKestrel/entity/entity.hpp>

namespace kestrel::image
{
    class basic_image
    {
    public:
        basic_image() = default;
        explicit basic_image(const math::size& size, const graphics::color& color = graphics::color::white_color());
        basic_image(resource_core::identifier id, const std::string& name, const math::size& size, const data::block& data);
        basic_image(const math::size& size, const graphics::color::lua_reference& color);

        ~basic_image();

        [[nodiscard]] virtual auto id() const -> resource_core::identifier;
        [[nodiscard]] virtual auto name() const -> std::string;

        [[nodiscard]] virtual auto size() const -> math::size;
        [[nodiscard]] virtual auto sprite_count() const -> std::size_t;

        virtual auto layout_sprites(const math::size& sprite_size) -> void;

        [[nodiscard]] auto sprite_sheet() const -> std::shared_ptr<graphics::sprite_sheet>;

        [[nodiscard]] virtual auto spawn_entity(const math::point& position) const -> std::shared_ptr<ecs::entity>;

    protected:
        resource_core::identifier m_id { resource_core::auto_resource_id };
        std::string m_name;
        std::shared_ptr<graphics::sprite_sheet> m_sheet;

        auto configure(resource_core::identifier id, const std::string& name, const math::size& size, const data::block& data) -> void;
        auto configure(resource_core::identifier id, const std::string& name, const std::shared_ptr<graphics::sprite_sheet>& sheet) -> void;
    };
}
