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
#include <string>
#include "scripting/state.hpp"
#include "scripting/script.hpp"
#include "util/hint.hpp"
#include "math/rect.hpp"
#include "core/asset/scene_interface.hpp"
#include <libGraphite/toolbox/dialog_item_list.hpp>
#include <libGraphite/toolbox/dialog.hpp>

namespace ui
{
    struct dialog_layout
    {
    public:
        enum class mode : std::uint8_t { scene = 0, imgui = 1 };

        struct element
        {
            std::uint8_t type { 0 };
            math::rect frame;
            std::string value;
            scripting::lua::script script;
        };

    public:
        dialog_layout() = default;
        dialog_layout(const luabridge::LuaRef& ref, const math::size& size);

        [[nodiscard]] auto mode() const -> enum mode;
        [[nodiscard]] auto element_count() const -> std::size_t;
        [[nodiscard]] auto frame() const -> math::rect;
        [[nodiscard]] auto size() const -> math::size;
        [[nodiscard]] auto flags() const -> enum asset::scene_interface::flags;

        auto set_frame(const math::rect& frame) -> void;
        auto set_size(const math::size& size) -> void;

        [[nodiscard]] auto element_at(std::int16_t idx) const -> const struct element *;

    private:
        enum mode m_mode { mode::scene };
        enum asset::scene_interface::flags m_flags { asset::scene_interface::flags::scene_passthrough };
        math::rect m_frame;
        std::vector<struct element> m_elements;

        auto build_scene_interface_layout(const asset::scene_interface *scin) -> void;
        auto build_dialog_layout(const graphite::toolbox::dialog *dlog) -> void;
    };
}