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

#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include "math/size.hpp"
#include <libGraphite/data/data.hpp>

namespace graphics
{
    class texture: public std::enable_shared_from_this<graphics::texture>
    {
    public:
        texture(uint32_t width, uint32_t height, const graphite::data::block& data);
        texture(const math::size& size, const graphite::data::block& data);
        texture(uint32_t width, uint32_t height, bool populate = false);
        explicit texture(const math::size& size, bool populate = false);

        [[nodiscard]] auto size() const -> math::size;
        [[nodiscard]] auto data() const -> const graphite::data::block&;
        [[nodiscard]] auto raw_data_ptr() const -> const void *;

        auto set_data(const graphite::data::block& data) -> void;

        virtual auto handle() const -> uint64_t;

        virtual auto bind() const -> void;

        virtual auto destroy() -> void;

    protected:
        math::size m_size;
        graphite::data::block m_data;

    };
}
