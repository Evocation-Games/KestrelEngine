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

#if __APPLE__ && !defined(KESTREL_METAL_TEXTURE_H)
#define KESTREL_METAL_TEXTURE_H

#include "core/graphics/common/texture.hpp"

namespace graphics { namespace metal {

    class texture: public graphics::texture
    {
    private:
        int m_handle { -1 };

    public:
        texture(const double& width, const double& height);
        explicit texture(const math::size& size);
        texture(const double& width, const double& height, std::vector<uint32_t> data);
        texture(const math::size& size, std::vector<uint32_t> data);
        texture(const math::size& size, const uint8_t *data);

        auto bind() const -> void override;

        auto set_handle(const int& handle) -> void;
        auto handle() const -> int override;
    };

}}

#endif //KESTREL_METAL_TEXTURE_H
