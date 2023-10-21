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

#include <libKestrel/ui/stencils/button_stencil.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>

// MARK: - Construction

kestrel::ui::stencils::button_stencil::button_stencil(button_drawing_style style)
    : m_style(style)
{

}

auto kestrel::ui::stencils::button_stencil::simple(const luabridge::LuaRef &image) -> lua_reference
{
    lua_reference ref { new button_stencil(button_drawing_style::simple) };
    ref->m_center = image::static_image::from(image);
    return ref;
}

auto kestrel::ui::stencils::button_stencil::stretch(const luabridge::LuaRef &left, const luabridge::LuaRef &center, const luabridge::LuaRef &right) -> lua_reference
{
    lua_reference ref { new button_stencil(button_drawing_style::stretch) };
    ref->m_left = image::static_image::from(left);
    ref->m_center = image::static_image::from(center);
    ref->m_right = image::static_image::from(right);
    return ref;
}

// MARK: - Masking

auto kestrel::ui::stencils::button_stencil::apply_simple_mask(const luabridge::LuaRef &image) -> void
{
    if (m_style != button_drawing_style::simple) {
        // TODO: Error
        return;
    }
    m_mask_center = image::static_image::from(image);
}

auto kestrel::ui::stencils::button_stencil::apply_stretch_mask(const luabridge::LuaRef &left, const luabridge::LuaRef &center, const luabridge::LuaRef &right) -> void
{
    if (m_style != button_drawing_style::stretch) {
        // TODO: Error
        return;
    }
    m_mask_left = image::static_image::from(left);
    m_mask_center = image::static_image::from(center);
    m_mask_right = image::static_image::from(right);
}

// MARK: - Drawing

auto kestrel::ui::stencils::button_stencil::draw(const std::shared_ptr<graphics::canvas> &canvas, const stencils::button_render_info::lua_reference& info) -> void
{
    canvas->clear();

    if (m_drawing_function.state() && m_drawing_function.isFunction()) {
        info->center = m_center;
        info->left = m_left;
        info->right = m_right;
        info->mask_center = m_mask_center;
        info->mask_left = m_mask_left;
        info->mask_right = m_mask_right;
        m_drawing_function(canvas, info);
    }
    else {
        switch (m_style) {
            case button_drawing_style::simple: {
                canvas->draw_static_image(m_center, math::rect(
                    math::point(
                        (canvas->get_bounds().width() - m_center->size().width()) / 2.0f,
                        (canvas->get_bounds().height() - m_center->size().height()) / 2.0f
                    ),
                    m_center->size()
                ));

                graphics::canvas::lua_reference mask { new graphics::canvas(canvas->get_bounds().size()) };
                if (m_mask_center.get()) {
                    mask->draw_static_image(m_mask_center, math::rect(
                        math::point(
                            (mask->get_bounds().width() - m_mask_center->size().width()) / 2.f,
                            (mask->get_bounds().height() - m_mask_center->size().height()) / 2.f
                        ),
                        m_mask_center->size()
                    ));
                }
                canvas->apply_mask(mask);

                break;
            }
            case button_drawing_style::stretch: {
                canvas->draw_static_image(m_left, math::rect(
                    math::point(0, (canvas->get_bounds().height() - m_left->size().height()) / 2.f),
                    m_left->size()
                ));

                canvas->draw_static_image(m_center, math::rect(
                    math::point(m_left->size().width(), (canvas->get_bounds().height() - m_center->size().height()) / 2.f),
                    math::size(canvas->get_bounds().width() - m_left->size().width() - m_right->size().width(), m_center->size().height())
                ));

                canvas->draw_static_image(m_right, math::rect(
                    math::point(
                           canvas->get_bounds().width() - m_right->size().width(),
                           (canvas->get_bounds().height() - m_right->size().height()) / 2.f
                    ),
                    m_right->size()
                ));

                graphics::canvas::lua_reference mask(new graphics::canvas(canvas->get_bounds().size()));
                mask->set_pen_color(graphics::color::black_color_ref());
                mask->fill_rect({{0, 0}, canvas->get_bounds().size()});

                if (m_mask_left.get()) {
                    mask->draw_static_image(m_mask_left, math::rect(
                        math::point(0, (canvas->get_bounds().height() - m_mask_left->size().height()) / 2.f),
                        m_mask_left->size()
                    ));
                }

                if (m_mask_center.get()) {
                    mask->draw_static_image(m_center, math::rect(
                        math::point(m_mask_left->size().width(), (canvas->get_bounds().height() - m_mask_center->size().height()) / 2.f),
                        math::size(canvas->get_bounds().width() - m_mask_left->size().width() - m_mask_right->size().width(), m_mask_center->size().height())
                    ));
                }

                if (m_mask_right.get()) {
                    mask->draw_static_image(m_mask_right, math::rect(
                        math::point(
                            canvas->get_bounds().width() - m_mask_right->size().width(),
                            (canvas->get_bounds().height() - m_mask_right->size().height()) / 2.f
                        ),
                        m_mask_right->size()
                    ));
                }
                canvas->apply_mask(mask);

                break;
            }
        }

        if (!info->text.empty()) {
            canvas->set_font(info->font);
            canvas->set_pen_color(info->text_color);
            auto size = canvas->layout_text_in_bounds(info->text, canvas->get_bounds().size());
            auto origin_size = (canvas->get_bounds().size() - size) / 2.f;
            auto origin = math::point(origin_size.width(), origin_size.height() - 1).floor();
            canvas->draw_text(origin);
        }

        if (info->icon.get()) {
            math::point origin(
                (canvas->get_bounds().size().width() - info->icon->size().width()) / 2.f,
                (canvas->get_bounds().size().height() - info->icon->size().height()) / 2.f
            );
            canvas->draw_static_image(info->icon, { origin.floor(), info->icon->size() });
        }
    }

    canvas->rebuild_texture();
}
