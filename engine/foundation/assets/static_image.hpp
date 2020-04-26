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

#if !defined(KESTREL_STATIC_IMAGE_HPP)
#define KESTREL_STATIC_IMAGE_HPP

#include <vector>
#include <memory>
#include <string>
#include "foundation/gl/spritesheet.hpp"
#include "scripting/lua/lua.hpp"
#include "libGraphite/rsrc/manager.hpp"

namespace kestrel { namespace assets {

    struct static_image: public lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<assets::static_image> lua_static_image;
        enum type { unknown, pict, cicn };

    private:
        enum type m_type { unknown };
        int64_t m_id { INT64_MIN };
        std::string m_name { "" };
        std::shared_ptr<gl::spritesheet> m_spritesheet { nullptr };

        auto construct_spritesheet(std::weak_ptr<graphite::qd::surface> surface) -> void;

    public:
        static auto register_object() -> void;

        static_image(int64_t id, std::vector<assets::static_image::type> preference = { pict });
        static auto load(int64_t id) -> lua_static_image;

        auto valid() const -> bool;

        auto spritesheet() -> std::weak_ptr<gl::spritesheet>;
        auto count() const -> int;
        auto size() const -> math::size;

        auto lua_draw(std::vector<double> position) const -> void;
        auto draw(const math::vector& v = math::vector::zero(), int frame = 0) const -> void;
    };

}};

#endif //KESTREL_STATIC_IMAGE_HPP
