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

#include <libKestrel/ui/imgui/image.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/legacy/spriteworld/sprite.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

// MARK: - Construction

kestrel::ui::imgui::image::image(luabridge::LuaRef src)
{
    if (lua::ref_isa<kestrel::image::static_image>(src)) {
        auto image = src.cast<kestrel::image::static_image::lua_reference>();
        m_sprite_sheet = image->sprite_sheet();
    }
    else if (lua::ref_isa<kestrel::image::legacy::spriteworld::sprite>(src)) {
        auto sprite = src.cast<kestrel::image::legacy::spriteworld::sprite::lua_reference>();
        m_sprite_sheet = sprite->sprite_sheet();
    }
    else if (lua::ref_isa<resource::descriptor>(src)) {
        auto descriptor = src.cast<resource::descriptor::lua_reference>();
        kestrel::image::static_image img(descriptor);
        m_sprite_sheet = img.sprite_sheet();
    }

    m_sprite_sheet->texture()->upload_to_gpu();
}

// MARK: - Drawing

auto kestrel::ui::imgui::image::draw() -> void
{
    auto texture = m_sprite_sheet->texture();
    auto sprite = m_sprite_sheet->at(m_frame);

    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    ImTextureID texture_id = reinterpret_cast<void *>(texture->handle());
    ImGui::Image(texture_id, ImVec2(m_sprite_sheet->sprite_size().width, m_sprite_sheet->sprite_size().height),
                 ImVec2(sprite.point().x, sprite.point().y),
                 ImVec2(sprite.point().x + sprite.size().width, sprite.point().y + sprite.size().height));
}