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
                .addProperty("dynamicResizing", &image_widget::dynamic_resizing, &image_widget::set_dynamic_resizing)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::image_widget::image_widget(const luabridge::LuaRef &image)
{
    if (image.state()) {
        m_image_ref = image;
    }

    auto internal_entity = std::make_shared<graphics::entity>(math::size(1));
    m_entity = { new scene_entity(internal_entity) };

    resize(true);
}

// MARK: - Accessors

auto ui::widgets::image_widget::entity() const -> ui::scene_entity::lua_reference
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

auto ui::widgets::image_widget::dynamic_resizing() const -> bool
{
    return m_dynamic_resizing;
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

auto ui::widgets::image_widget::set_dynamic_resizing(bool v) -> void
{
    m_dynamic_resizing = v;
    resize();
}

// MARK: - Drawing

auto ui::widgets::image_widget::resize(bool reload) -> void
{
    if (reload) {
        if (scripting::lua::ref_isa<asset::static_image>(m_image_ref)) {
            m_entity->change_internal_entity(m_image_ref.cast<asset::static_image::lua_reference>()->spawn_entity({ 0, 0 }));
        }
        else if (scripting::lua::ref_isa<asset::legacy::macintosh::quickdraw::picture>(m_image_ref)) {
            m_entity->change_internal_entity(m_image_ref.cast<asset::legacy::macintosh::quickdraw::picture::lua_reference>()->spawn_entity({ 0, 0 }));
        }
        else if (scripting::lua::ref_isa<asset::legacy::macintosh::quickdraw::color_icon>(m_image_ref)) {
            m_entity->change_internal_entity(m_image_ref.cast<asset::legacy::macintosh::quickdraw::color_icon::lua_reference>()->spawn_entity({ 0, 0 }));
        }
        else if (scripting::lua::ref_isa<asset::resource_descriptor>(m_image_ref)) {
            auto descriptor = m_image_ref.cast<asset::resource_descriptor::lua_reference>();
            asset::static_image img(descriptor);
            m_entity->change_internal_entity(img.spawn_entity({0, 0}));
        }
        else {
            // TODO: Handle unrecognised image format...
        }
    }

    if (m_dynamic_resizing) {
        m_entity->set_size(m_entity->internal_entity()->get_size());
        switch (m_alignment) {
            default:
                math::point position(
                    m_frame.get_x() + ((m_frame.get_width() - m_entity->size().get_width()) / 2),
                    m_frame.get_y() + ((m_frame.get_height() - m_entity->size().get_height()) / 2)
                );
                m_entity->set_position(position);
                break;
        }
    }
    else if (m_entity->internal_entity() && m_entity->internal_entity()->texture()) {
        auto texture = m_entity->internal_entity()->texture();

        auto source_size = texture->size();
        auto destination_size = m_frame.size;
        auto scale = source_size / destination_size;
        auto source_ar = source_size.aspect_ratio();
        auto destination_ar = destination_size.aspect_ratio();

        math::size target_size(source_size);

        // The first task is to scale the target size so that it fits within the destination.
        if (target_size.is_landscape()) {
            target_size.height = destination_size.height;
            target_size.width = target_size.height * source_ar;
        }
        else {
            target_size.width = destination_size.width;
            target_size.width = target_size.width * source_ar;
        }

        auto scale_factor = source_size.width / target_size.width;

        math::point output_origin(0);
        math::point output_uv_origin(0);
        math::size output_size(target_size);
        math::size output_uv_size(source_size);

        output_size.width = std::min(output_size.width, destination_size.width);
        output_size.height = std::min(output_size.height, destination_size.height);
        output_origin.x = std::max(0.0, (destination_size.width - output_size.width) / 2.0);
        output_origin.y = std::max(0.0, (destination_size.height - output_size.height) / 2.0);

        output_uv_origin = math::point(
            output_origin.x == 0 ? std::abs((destination_size.width - target_size.width) / 2.0) : 0,
            output_origin.y == 0 ? std::abs((destination_size.height - target_size.height) / 2.0) : 0
        ) * scale_factor;
        output_uv_size = output_size * scale_factor;

        // We need to aspect fill in the frame. First set up the frame and size of the entity...
        m_entity->set_render_size(output_size);
        m_entity->set_draw_size(output_size);
        m_entity->set_position(m_frame.origin + output_origin);

        m_entity->set_clipping_area(output_uv_size);
        m_entity->set_clipping_offset(output_uv_origin);
    }

}

auto ui::widgets::image_widget::draw() -> void
{
    // Do nothing...
}

