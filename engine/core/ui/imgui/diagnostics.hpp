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

#include <imgui/imgui.h>
#include "core/ui/imgui/label.hpp"
#include "core/ui/imgui/window.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class diagnostics : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<diagnostics> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api diagnostics();

        auto draw() -> void override;

    private:
        uint32_t m_throttle { 0 };
        bool m_constructed { false };
        window m_window { "Diagnostics", {250, 150} };
        label::lua_reference m_frame_duration { new label("0") };
        label::lua_reference m_update_duration { new label("0") };
        label::lua_reference m_approx_fps { new label("0") };
        label::lua_reference m_target_fps { new label("0") };
        label::lua_reference m_driver { new label("OpenGL") };

        auto construct_contents() -> void;
    };
}
