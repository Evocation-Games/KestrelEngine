// Copyright (c) 2020 Tom Hancocks
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

#if __APPLE__ && !defined(KESTREL_METAL_SESSION_WINDOW_H)
#define KESTREL_METAL_SESSION_WINDOW_H

#include <memory>
#include "core/graphics/metal/metal_view.h"
#include "core/graphics/common/session_window.hpp"
#include "core/graphics/common/entity.hpp"

namespace cocoa
{
    class window;
}

namespace graphics::metal
{

    class session_window: public graphics::session_window, public std::enable_shared_from_this<metal::session_window>
    {
    protected:
        std::shared_ptr<cocoa::window> m_window;
        std::shared_ptr<metal::view> m_view;

    public:
        explicit session_window(std::shared_ptr<environment> env);

        auto set_title(const std::string& title) -> void override;
        auto set_size(const math::size& size) -> void override;

        auto new_scene(const std::string& name, const scripting::lua::script& script) -> std::shared_ptr<graphics::scene> override;

        auto render() -> void override;

        auto create_texture(const math::size& size, std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture> override;
        auto create_texture(const math::size& size, const uint8_t *data) const -> std::shared_ptr<graphics::texture> override;

        auto draw_entity(const graphics::entity::lua_reference& entity) const -> void;
    };

};

#endif //KESTREL_METAL_SESSION_WINDOW_H
