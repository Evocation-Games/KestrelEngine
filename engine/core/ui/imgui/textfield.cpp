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

#include "core/ui/imgui/textfield.hpp"

// MARK: - Lua

auto ui::imgui::textfield::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<textfield>("TextField")
                .addConstructor<auto(*)(std::size_t, const std::string&)->void, lua_reference>()
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::textfield::textfield(std::size_t buffer_size, const std::string &text)
    : m_buffer_size(buffer_size)
{
    m_buffer = new char[buffer_size + 1];
    memcpy(m_buffer, text.c_str(), std::min(text.size(), buffer_size));
}

// MARK: - Destruction

ui::imgui::textfield::~textfield()
{
    delete m_buffer;
}

// MARK: - Drawing

auto ui::imgui::textfield::draw() -> void
{
    ImGui::InputText(identifier_string(), m_buffer, m_buffer_size);
}