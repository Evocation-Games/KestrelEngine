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

#include <stdexcept>
#include <libKestrel/resource/reader.hpp>
#include <libKestrel/ui/scene/interface.hpp>
#include <libKestrel/ui/types/action/action.hpp>
#include <libKestrel/ui/types/value/value.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libKestrel/kestrel.hpp>
#include <libGraphite/rsrc/manager.hpp>
#include <libUI/format/interface/decoder.hpp>

// MARK: - Helpers

static inline auto convert_to_control_type(enum ::ui::format::element::type type) -> kestrel::ui::control_type
{
    switch (type) {
        case ui::format::element::type::button:             return kestrel::ui::control_type::button;
        case ui::format::element::type::label:              return kestrel::ui::control_type::label;
        case ui::format::element::type::checkbox:           return kestrel::ui::control_type::checkbox;
        case ui::format::element::type::popup_button:       return kestrel::ui::control_type::popup_button;
        case ui::format::element::type::canvas:             return kestrel::ui::control_type::canvas;
        case ui::format::element::type::image:              return kestrel::ui::control_type::image;
        case ui::format::element::type::sprite:             return kestrel::ui::control_type::sprite;
        case ui::format::element::type::text_field:         return kestrel::ui::control_type::text_field;
        case ui::format::element::type::text_area:          return kestrel::ui::control_type::text_area;
        case ui::format::element::type::grid:               return kestrel::ui::control_type::grid;
        case ui::format::element::type::list:               return kestrel::ui::control_type::list;
        case ui::format::element::type::separator:          return kestrel::ui::control_type::separator;
        case ui::format::element::type::slider:             return kestrel::ui::control_type::slider;
        case ui::format::element::type::radio_button:       return kestrel::ui::control_type::radio;
        case ui::format::element::type::scroll_area:        return kestrel::ui::control_type::scroll_area;
        case ui::format::element::type::vertical_spacer:    return kestrel::ui::control_type::spacer;
        case ui::format::element::type::horizontal_spacer:  return kestrel::ui::control_type::spacer;
        case ui::format::element::type::position:           return kestrel::ui::control_type::position;
    }
}

// MARK: - Construction

kestrel::ui::scene_interface::scene_interface(const resource::descriptor::lua_reference &ref)
{
    if (auto resource = ref->with_type(resource_type::code)->load()) {
        auto instance = ::ui::format::decode(resource->data());
        m_name = instance.name();
        m_flags = static_cast<enum scene_interface_flags>(instance.flags());
        m_scene_size = math::size(instance.frame().size);

        const auto background = instance.background();
        m_has_background = (background.type() != ::ui::format::background::type::none);
        if (m_has_background) {
            m_stretch_background = (background.type() == ::ui::format::background::type::is_dynamic);
            m_background = { new resource::descriptor(background.fill()) };
            if (m_stretch_background) {
                m_background_top = { new resource::descriptor(background.top()) };
                m_background_bottom = { new resource::descriptor(background.bottom()) };
            }
        }

        float y_position = 0;
        float y_advance = 0;
        std::vector<scene_interface_item::lua_reference> dynamic_height_items;
        for (const auto& element : instance.elements()) {
            const auto type = convert_to_control_type(element.type());
            auto item = scene_interface_item::lua_reference(new scene_interface_item());
            item->set_type(type);

            for (const auto& attribute : element.attributes()) {
                switch (attribute.code()) {
                    case ::ui::format::attribute::code::name: {
                        item->set_identifier(attribute.value().value<std::string>());
                        break;
                    }
                    case ::ui::format::attribute::code::frame: {
                        auto frame = attribute.value().value<::ui::rect>();
                        item->set_frame(math::rect(frame));
                        break;
                    }
                    case ::ui::format::attribute::code::color: {
                        item->set_color({ new graphics::color(attribute.value().value<std::uint32_t>()) });
                        break;
                    }
                    case ::ui::format::attribute::code::background_color: {
                        item->set_background_color({ new graphics::color(attribute.value().value<std::uint32_t>()) });
                        break;
                    }
                    case ::ui::format::attribute::code::selection_color: {
                        item->set_selection_color({ new graphics::color(attribute.value().value<std::uint32_t>()) });
                        break;
                    }
                    case ::ui::format::attribute::code::border_color: {
                        item->set_border_color({ new graphics::color(attribute.value().value<std::uint32_t>()) });
                        break;
                    }
                    case ::ui::format::attribute::code::secondary_color: {
                        item->set_secondary_color({ new graphics::color(attribute.value().value<std::uint32_t>()) });
                        break;
                    }
                    case ::ui::format::attribute::code::font: {
                        item->set_font(attribute.value().value<std::string>());
                        break;
                    }
                    case ::ui::format::attribute::code::font_size: {
                        item->set_font_size(static_cast<std::int16_t>(attribute.value().value<std::int64_t>()));
                        break;
                    }
                    case ::ui::format::attribute::code::hidden: {
                        item->set_hidden(attribute.value().value<bool>());
                        break;
                    }
                    case ::ui::format::attribute::code::disabled: {
                        item->set_disabled(attribute.value().value<bool>());
                        break;
                    }
                    case ::ui::format::attribute::code::weight: {
                        item->set_weight(static_cast<std::int16_t>(attribute.value().value<std::int64_t>()));
                        break;
                    }
                    case ::ui::format::attribute::code::horizontal_alignment: {
                        item->set_alignment(static_cast<enum ui::horizontal_alignment>(attribute.value().value<std::int64_t>()));
                        break;
                    }
                    case ::ui::format::attribute::code::action: {
                        item->set_action(ui::action(attribute.value().value<::ui::format::action>()));
                        break;
                    }
                    case ::ui::format::attribute::code::value: {
                        item->set_value(ui::value(attribute.value().value<::ui::format::value>()));
                        break;
                    }
                    case ::ui::format::attribute::code::y: {
                        if (type == control_type::position) {
                            y_position = static_cast<float>(attribute.value().value<std::int64_t>());
                            y_advance = 0;
                        }
                        break;
                    }
                    case ::ui::format::attribute::code::x: {
                        break;
                    }
                    case ::ui::format::attribute::code::size: {
                        if (type == control_type::spacer) {
                            y_position += y_advance + static_cast<float>(attribute.value().value<std::int64_t>());
                            y_advance = 0;
                        }
                        break;
                    }
                    default:
                        throw std::runtime_error("");
                }
            }

            if (m_flags & scene_interface_flags::vertical_flow_layout) {
                switch (type) {
                    case control_type::spacer:
                    case control_type::position:
                        continue;
                    default: {
                        auto r = item->frame();
                        r.set_y(y_position);
                        item->set_frame(r);
                        y_advance = std::max(y_advance, r.height());
                        break;
                    }
                }
            }

            m_items.emplace_back(item);
            if (item->frame().height() <= 0) {
                dynamic_height_items.emplace_back(item);
            }
        }

        if (m_flags & scene_interface_flags::vertical_flow_layout) {
            m_scene_size.set_height(y_position + y_advance);
            for (const auto& item : dynamic_height_items) {
                auto r = item->frame();
                r.set_height(m_scene_size.height());
                item->set_frame(r);
            }
        }

        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}
