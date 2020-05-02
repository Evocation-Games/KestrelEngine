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

#if !defined(KESTREL_MACINTOSH_PICTURE_HPP)
#define KESTREL_MACINTOSH_PICTURE_HPP


#include <vector>
#include <memory>
#include <string>
#include "scripting/lua/lua.hpp"
#include "foundation/gl/spritesheet.hpp"
#include "libGraphite/rsrc/manager.hpp"
#include "foundation/assets/resource_reference.hpp"

namespace kestrel { namespace assets {

    struct macintosh_picture: public lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<assets::macintosh_picture> lua_reference;

    private:
        int64_t m_id { INT64_MIN };
        std::string m_name { "" };
        std::shared_ptr<gl::spritesheet> m_spritesheet { nullptr };

        auto construct_spritesheet(std::weak_ptr<graphite::qd::surface> surface) -> void;

    public:
        static auto register_object() -> void;

        macintosh_picture(resource_reference::lua_reference ref);
        static auto load(resource_reference::lua_reference ref) -> macintosh_picture::lua_reference;

        auto valid() const -> bool;

        auto lua_reconfigure_spritesheet(math::size::lua_reference size, int total_sprites) -> void;
        auto reconfigure_spritesheet(const math::size& sprite_size, int total_sprites) -> void;
        auto spritesheet() -> std::weak_ptr<gl::spritesheet>;
        auto count() const -> int;
        auto size() const -> math::size;

        auto lua_draw(math::point::lua_reference position) const -> void;
        auto lua_draw_frame(math::point::lua_reference position, int frame) const -> void;
        auto draw(const math::vector& v = math::vector::zero(), int frame = 0) const -> void;
    };

}};

#endif //KESTREL_MACINTOSH_PICTURE_HPP
