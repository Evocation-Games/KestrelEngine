// Copyright (c) 2023 Tom Hancocks
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

#include <libRenderCore/texture/store.hpp>
#include <libMath/geometry/rect.hpp>
#include <libFoundation/profile/profiler.hpp>

namespace renderer
{
    template<std::size_t MAX>
    struct occlusion_tester
    {
        occlusion_tester() = default;

        auto test(math::geometry::rect bounds, texture::id texture, bool full_occlusion = false) -> bool
        {
            return true;
            // TODO: This is a VERY naive approach to occlusion for 2d sprites. However it works for now.
            // Determine the geometry and test existing samples to see if we have the same
            // geometry. If we don't, then add the geometry to the samples and return true.
            for (auto i = 0; i < m_next_sample; ++i) {
                const auto& existing = (*m_samples)[i];
                if (existing.bounds == bounds) {
                    // We have found one! Check to see if its texture matches. The same texture would mean blitting
                    // the same pixels out, which is pointless. Otherwise if the existing sample has full
                    // occlusion, then also reject, as nothing is permitted to be drawn beneath.
                    if (existing.texture == texture || existing.full_occlusion) {
                        return false;
                    }
                }
            }

            // Reaching this point indicates that it is permitted
            (*m_samples)[m_next_sample].texture = texture;
            (*m_samples)[m_next_sample].bounds = bounds;
            (*m_samples)[m_next_sample++].full_occlusion = full_occlusion;

            return true;
        }

        auto reset() -> void
        {
            m_next_sample = 0;
        }

    private:
        struct sample {
            bool full_occlusion { false };
            math::geometry::rect bounds;
            texture::id texture { 0 };
        };
        std::unique_ptr<std::array<sample, MAX>> m_samples {
            std::make_unique<std::array<sample, MAX>>()
        };
        std::size_t m_next_sample { 0 };
    };
}
