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

#include "core/ui/imgui/image.hpp"
#include "core/asset/static_image.hpp"
#include "core/asset/spritesheet.hpp"
#include "core/asset/legacy/spriteworld/sprite.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Lua

auto ui::imgui::image::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<image>("Image")
                .addConstructor<auto(*)(luabridge::LuaRef)->void, lua_reference>()
                .addProperty("position", &image::position, &image::set_position)
                .addProperty("size", &image::size, &image::set_size)
                .addProperty("frame", &image::frame, &image::set_frame)
                .addProperty("frameCount", &image::frame_count)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::image::image(luabridge::LuaRef src)
{
    if (scripting::lua::ref_isa<asset::static_image>(src)) {
        auto image = src.cast<asset::static_image::lua_reference>();
        m_spritesheet = image->sprite_sheet();
    }
    else if (scripting::lua::ref_isa<asset::legacy::spriteworld::sprite>(src)) {
        auto sprite = src.cast<asset::legacy::spriteworld::sprite::lua_reference>();
        m_spritesheet = sprite->sprite_sheet();
    }
    else if (scripting::lua::ref_isa<asset::spritesheet>(src)) {
        auto sheet = src.cast<asset::spritesheet::lua_reference>();
        m_spritesheet = sheet->sprite_sheet();
    }
    else if (scripting::lua::ref_isa<asset::resource_descriptor>(src)) {
        auto descriptor = src.cast<asset::resource_descriptor::lua_reference>();
        asset::static_image img(descriptor);
        m_spritesheet = img.sprite_sheet();
    }

    m_spritesheet->texture()->upload_to_gpu();
}

// MARK: - Drawing

auto ui::imgui::image::draw() -> void
{
    auto texture = m_spritesheet->texture();
    auto sprite = m_spritesheet->at(m_frame);

    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    ImTextureID texture_id = reinterpret_cast<void *>(texture->handle());
    ImGui::Image(texture_id, ImVec2(m_spritesheet->sprite_size().width, m_spritesheet->sprite_size().height),
                 ImVec2(sprite.point().x, sprite.point().y),
                 ImVec2(sprite.point().x + sprite.size().width, sprite.point().y + sprite.size().height));
}