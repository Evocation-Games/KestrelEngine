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

#include "core/ui/imgui/container.hpp"

#include "core/ui/imgui/separator.hpp"
#include "core/ui/imgui/label.hpp"
#include "core/ui/imgui/button.hpp"
#include "core/ui/imgui/textfield.hpp"
#include "core/ui/imgui/image.hpp"
#include "core/ui/imgui/slider.hpp"
#include "core/ui/imgui/same_line.hpp"
#include "core/ui/imgui/combo.hpp"
#include "core/ui/imgui/tabbar.hpp"
#include "core/ui/imgui/checkbox.hpp"
#include "core/ui/imgui/table.hpp"
#include "core/ui/imgui/box.hpp"

// MARK: - Drawing

auto ui::imgui::widget_container::add_widget(luabridge::LuaRef widget) -> void
{
    m_contents.emplace_back(widget);
}

// MARK: - Draw

auto ui::imgui::widget_container::draw() -> void
{
    for (auto widget : m_contents) {
        if (scripting::lua::ref_isa<label>(widget)) {
            widget.cast<label::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<button>(widget)) {
            widget.cast<button::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<textfield>(widget)) {
            widget.cast<textfield::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<image>(widget)) {
            widget.cast<image::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<slider>(widget)) {
            widget.cast<slider::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<same_line>(widget)) {
            widget.cast<same_line::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<combo>(widget)) {
            widget.cast<combo::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<tabbar>(widget)) {
            widget.cast<tabbar::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<checkbox>(widget)) {
            widget.cast<checkbox::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<table>(widget)) {
            widget.cast<table::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<box>(widget)) {
            widget.cast<box::lua_reference>()->draw();
        }
        else if (scripting::lua::ref_isa<separator>(widget)) {
            widget.cast<separator::lua_reference>()->draw();
        }
    }
}

