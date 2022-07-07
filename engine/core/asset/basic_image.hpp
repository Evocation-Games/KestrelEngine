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
#include <libGraphite/data/data.hpp>
#include "scripting/state.hpp"
#include "core/graphics/common/color.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/spritesheet.hpp"

namespace asset
{

    class basic_image
    {
    public:
        basic_image() = default;
        explicit basic_image(const math::size& size, const graphics::color& color = graphics::color::white_color());
        basic_image(int64_t id, const std::string& name, const math::size& size, const graphite::data::block& data);
        lua_api basic_image(const math::size& size, const graphics::color::lua_reference& color);

        ~basic_image();

        [[nodiscard]] virtual auto id() const -> int64_t;
        [[nodiscard]] virtual auto name() const -> std::string;

        lua_api [[nodiscard]] virtual auto size() const -> math::size;
        lua_api [[nodiscard]] virtual auto sprite_count() const -> int;

        lua_api virtual auto layout_sprites(const math::size& sprite_size) -> void;

        [[nodiscard]] auto sprite_sheet() const -> std::shared_ptr<graphics::spritesheet>;

        lua_api [[nodiscard]] virtual auto spawn_entity(const math::point& position) const -> std::shared_ptr<graphics::entity>;

    protected:
        int64_t m_id { INT64_MIN };
        std::string m_name;
        std::shared_ptr<graphics::spritesheet> m_sheet;

        auto configure(int64_t id, const std::string& name, const math::size& size, const graphite::data::block& data) -> void;
        auto configure(int64_t id, const std::string& name, const std::shared_ptr<graphics::spritesheet>& sheet) -> void;
    };

}
