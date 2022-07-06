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

#include "core/ui/widgets/image_widget.hpp"
#include "core/ui/entity/scene_entity.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"
#include "core/asset/static_image.hpp"

// MARK: - Lua

auto ui::widgets::image_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<ui::widgets::image_widget>("Image")
                .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
                .addProperty("frame", &image_widget::frame, &image_widget::set_frame)
                .addProperty("image", &image_widget::image, &image_widget::set_image)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::image_widget::image_widget(const luabridge::LuaRef &image)
    : m_image_ref(image)
{
    resize(true);
}

// MARK: - Accessors

auto ui::widgets::image_widget::entity() const -> std::shared_ptr<ui::scene_entity>
{
    return m_entity;
}

auto ui::widgets::image_widget::frame() const -> math::rect
{
    return m_frame;
}

auto ui::widgets::image_widget::image() const -> luabridge::LuaRef
{
    return m_image_ref;
}

// MARK: - Setters

auto ui::widgets::image_widget::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
    resize();
}

auto ui::widgets::image_widget::set_image(const luabridge::LuaRef &image) -> void
{
    m_image_ref = image;
    resize(true);
}

// MARK: - Drawing

auto ui::widgets::image_widget::resize(bool reload) -> void
{
    if (reload) {
        if (scripting::lua::ref_isa<asset::static_image>(m_image_ref)) {
            auto entity = m_image_ref.cast<asset::static_image::lua_reference>()->spawn_entity({ 0, 0 });
            m_entity = std::make_shared<scene_entity>(entity);
        }
        else if (scripting::lua::ref_isa<asset::legacy::macintosh::quickdraw::picture>(m_image_ref)) {
            auto entity = m_image_ref.cast<asset::legacy::macintosh::quickdraw::picture::lua_reference>()->spawn_entity({ 0, 0 });
            m_entity = std::make_shared<scene_entity>(entity);
        }
        else if (scripting::lua::ref_isa<asset::legacy::macintosh::quickdraw::color_icon>(m_image_ref)) {
            auto entity = m_image_ref.cast<asset::legacy::macintosh::quickdraw::color_icon::lua_reference>()->spawn_entity({ 0, 0 });
            m_entity = std::make_shared<scene_entity>(entity);
        }
        else if (scripting::lua::ref_isa<asset::resource_descriptor>(m_image_ref)) {
            auto descriptor = m_image_ref.cast<asset::resource_descriptor::lua_reference>();
            asset::static_image img(descriptor);
            m_entity = std::make_shared<scene_entity>(img.spawn_entity({0, 0}));
        }
        else {
            // TODO: Handle unrecognised image format...
            m_entity = nullptr;
        }
    }

    switch (m_alignment) {
        default:
            m_entity->set_position({
                m_frame.get_x() + ((m_frame.get_width() - m_entity->size().get_width()) / 2),
                m_frame.get_y() + ((m_frame.get_height() - m_entity->size().get_height()) / 2)
            });
            break;
    }
}

auto ui::widgets::image_widget::draw() -> void
{
    // Do nothing...
}

