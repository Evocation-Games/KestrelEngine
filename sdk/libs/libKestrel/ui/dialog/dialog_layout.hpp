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
#include <optional>
#include <libKestrel/lua/script.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/ui/scene/interface.hpp>
#include <libKestrel/ui/alignment.hpp>
#include <libToolbox/ui/dialog_item_list.hpp>
#include <libToolbox/ui/dialog.hpp>
#include <libKestrel/ui/dialog/dialog_render_mode.hpp>
#include <libKestrel/ui/types/action/action.hpp>

namespace kestrel::ui
{
    struct dialog_layout
    {
    public:
        struct element
        {
            std::string name;
            std::uint8_t type { 0 };
            math::rect frame;
            ui::value value;
            ui::action action;
            graphics::color::lua_reference background_color { graphics::color::black_color_ref() };
            graphics::color::lua_reference border_color { graphics::color::white_color_ref() };
            graphics::color::lua_reference text_color { graphics::color::white_color_ref() };
            graphics::color::lua_reference secondary_color { graphics::color::white_color_ref() };
            graphics::color::lua_reference selection_color  { graphics::color::grey_color_ref() };
            std::string font_name;
            std::int16_t font_size { 12 };
            ui::horizontal_alignment alignment { ui::horizontal_alignment::left };
        };

    public:
        dialog_layout() = default;
        dialog_layout(const luabridge::LuaRef& ref, const math::size& size);

        [[nodiscard]] auto mode() const -> enum dialog_render_mode;
        [[nodiscard]] auto element_count() const -> std::size_t;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto frame() const -> math::rect;
        [[nodiscard]] auto size() const -> math::size;
        [[nodiscard]] auto flags() const -> enum scene_interface_flags;
        [[nodiscard]] auto is_legacy_dialog() const -> bool;

        [[nodiscard]] auto has_stretched_background() const -> bool;
        [[nodiscard]] auto background() const -> resource::descriptor::lua_reference;
        [[nodiscard]] auto background_top() const -> resource::descriptor::lua_reference;
        [[nodiscard]] auto background_bottom() const -> resource::descriptor::lua_reference;

        auto set_frame(const math::rect& frame) -> void;
        auto set_size(const math::size& size) -> void;

        [[nodiscard]] auto element_at(std::int16_t idx) const -> const struct element *;

    private:
        bool m_is_dlog { false };
        enum dialog_render_mode m_mode { dialog_render_mode::scene };
        enum scene_interface_flags m_flags { scene_interface_flags::scene_passthrough };
        math::rect m_frame;
        std::string m_name;
        std::vector<struct element> m_elements;
        bool m_stretched_background { false };
        resource::descriptor::lua_reference m_background { nullptr };
        resource::descriptor::lua_reference m_background_top { nullptr };
        resource::descriptor::lua_reference m_background_bottom { nullptr };

        auto build_scene_interface_layout(const scene_interface *scin) -> void;
        auto build_dialog_layout(const toolbox::dialog *dlog) -> void;
    };
}