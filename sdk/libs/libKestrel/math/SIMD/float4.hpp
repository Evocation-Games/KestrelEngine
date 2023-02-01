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

#include <cassert>
#include <libKestrel/math/SIMD/SIMD.hpp>

namespace kestrel::math::SIMD
{
#if INTEL_SSE
    typedef __m128          f32x4;
#elif ARM_NEON
    typedef ::float32x4_t   f32x4;
#endif

    struct float4_s;
    typedef struct float4_s float4;

    struct float4_s
    {
        float data[4] { 0.f };

        // MARK: - Construction
        explicit float4_s(float x = 0, float y = 0, float z = 0, float w = 0)
        {
            data[0] = x;
            data[1] = y;
            data[2] = z;
            data[3] = w;
        }

        explicit float4_s(f32x4 v)
        {
            replace(v);
        }

        static inline auto constant(float v) -> float4
        {
            return float4(v, v, v, v);
        }

        // MARK: - Access
        inline auto operator[] (int i) const -> float
        {
            assert(i >= 0 && i <= 3);
            return data[i];
        }

        inline auto set(int i, float f) -> void
        {
            assert(i >= 0 && i <= 3);
            data[i] = f;
        }

        // MARK: - Operations
        inline auto reversed() const -> float4
        {
#if INTEL_SSE
            return float4(_mm_shuffle_ps(vector(), vector(), _MM_SHUFFLE(0, 1, 2, 3)));
#elif ARM_NEON
            return *this;
#endif
        }

        inline auto operator+ (const float4& other) const -> float4
        {
#if INTEL_SSE
            return float4(_mm_add_ps(vector(), other.vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        inline auto operator+ (float f) const -> float4
        {
            return *this + float4::constant(f);
        }

        inline auto operator+= (const float4& other) -> float4&
        {
            replace((*this + other).vector());
            return *this;
        }

        inline auto operator+= (float f) -> float4&
        {
            replace((*this + f).vector());
            return *this;
        }

        inline auto operator- (const float4& other) const -> float4
        {
#if INTEL_SSE
            return float4(_mm_sub_ps(vector(), other.vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        inline auto operator- (float f) const -> float4
        {
            return *this - float4::constant(f);
        }

        inline auto operator-= (const float4& other) -> float4&
        {
            replace((*this - other).vector());
            return *this;
        }

        inline auto operator-= (float f) -> float4&
        {
            replace((*this - f).vector());
            return *this;
        }

        inline auto operator* (const float4& other) const -> float4
        {
#if INTEL_SSE
            return float4(_mm_mul_ps(vector(), other.vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        inline auto operator* (float f) const -> float4
        {
            return *this * float4::constant(f);
        }

        inline auto operator*= (const float4& other) -> float4&
        {
            replace((*this * other).vector());
            return *this;
        }

        inline auto operator*= (float f) -> float4&
        {
            replace((*this * f).vector());
            return *this;
        }

        [[nodiscard]] inline auto squared() const -> float4
        {
            return *this * *this;
        }

        inline auto operator/ (const float4& other) const -> float4
        {
#if INTEL_SSE
            return float4(_mm_div_ps(vector(), other.vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        inline auto operator/ (float f) const -> float4
        {
            return *this / float4::constant(f);
        }

        inline auto operator/= (const float4& other) -> float4&
        {
            replace((*this / other).vector());
            return *this;
        }

        inline auto operator/= (float f) -> float4&
        {
            replace((*this / f).vector());
            return *this;
        }

        inline auto operator== (const float4& other) const -> bool
        {
#if INTEL_SSE
            auto m = _mm_movemask_epi8(_mm_cmpneq_ps(other.vector(), vector()));
            return (m == 0xFFFF);
#elif ARM_NEON
            return false;
#endif
        }

        inline auto operator!= (const float4& other) const -> bool
        {
            return !(*this == other);
        }

        [[nodiscard]] inline auto round() const -> float4
        {
#if INTEL_SSE
            return float4(_mm_round_ps(vector(), _MM_ROUND_NEAREST));
#elif ARM_NEON
            return *this;
#endif
        }

        [[nodiscard]] inline auto floor() const -> float4
        {
#if INTEL_SSE
            return float4(_mm_floor_ps(vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        [[nodiscard]] inline auto ceil() const -> float4
        {
#if INTEL_SSE
            return float4(_mm_ceil_ps(vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        [[nodiscard]] inline auto abs() const -> float4
        {
#if INTEL_SSE
            const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
            return float4(_mm_and_ps(mask, vector()));
#elif ARM_NEON
            return *this;
#endif
        }

        [[nodiscard]] inline auto lower() const -> float4
        {
            return float4(data[0], data[1]);
        }

        [[nodiscard]] inline auto upper() const -> float4
        {
            return float4(0, 0, data[2], data[3]);
        }

        [[nodiscard]] inline auto swapped() const -> float4
        {
            return float4(data[2], data[3], data[0], data[1]);
        }


    private:
        inline auto replace(f32x4 v) -> void
        {
#if INTEL_SSE
            _mm_storeu_ps((float *)&data[0], v);
#elif ARM_NEON
#endif
        }

        inline auto vector() const -> f32x4
        {
#if INTEL_SSE
            return _mm_loadu_ps((const float *)&data[0]);
#elif ARM_NEON
            return vdupq_n_f32(0);
#endif
        }

    };

}