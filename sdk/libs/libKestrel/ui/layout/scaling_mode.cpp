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

#include <libKestrel/ui/layout/scaling_mode.hpp>

// MARK: - Calculations

static auto calculate_aspect_fit(const kestrel::math::size& container, const kestrel::math::size& size, kestrel::ui::layout::aspect_ratio_t ratio) -> kestrel::math::size
{
    auto result = size;
    if (kestrel::ui::layout::is_landscape(ratio)) {
        if (size.width() > container.width()) {
            result.set_width(container.width());
            result.set_height(result.width() / ratio);
        }
    }
    else {
        if (size.height() > container.height()) {
            result.set_height(container.height());
            result.set_width(result.height() * ratio);
        }
    }
    return size;
}

static auto calculate_aspect_fill(const kestrel::math::size& container, const kestrel::math::size& size, kestrel::ui::layout::aspect_ratio_t ratio) -> kestrel::math::size
{
    auto result = size;
    if (kestrel::ui::layout::is_landscape(ratio)) {
        if (size.height() > container.height()) {
            result.set_height(container.height());
            result.set_width(container.height() * ratio);
        }
    }
    else {
        if (size.width() > container.width()) {
            result.set_width(container.width());
            result.set_height(container.width() / ratio);
        }
    }
    return size;
}

static auto calculate_scale_fit(const kestrel::math::size& container, const kestrel::math::size& size, kestrel::ui::layout::aspect_ratio_t ratio) -> kestrel::math::size
{
    auto result = size;
    if (kestrel::ui::layout::is_landscape(ratio)) {
        result.set_width(container.width());
        result.set_height(result.width() / ratio);
    }
    else {
        result.set_height(container.height());
        result.set_width(result.height() * ratio);
    }
    return size;
}

static auto calculate_scale_fill(const kestrel::math::size& container, const kestrel::math::size& size, kestrel::ui::layout::aspect_ratio_t ratio) -> kestrel::math::size
{
    auto result = size;
    if (kestrel::ui::layout::is_landscape(ratio)) {
        result.set_height(container.height());
        result.set_width(result.height() * ratio);
    }
    else {
        result.set_width(container.width());
        result.set_height(result.width() / ratio);
    }
    return size;
}

// MARK: - Access Function

auto kestrel::ui::layout::calculate_size(enum scaling_mode mode, const math::size& container, const math::size& size, aspect_ratio_t ratio) -> math::size
{
    switch (mode) {
        case scaling_mode::normal:
            return size;

        case scaling_mode::aspect_fit: {
            return calculate_aspect_fit(container, size, ratio);
        }
        case scaling_mode::aspect_fill: {
            return calculate_aspect_fill(container, size, ratio);
        }

        case scaling_mode::scale_fit: {
            return calculate_scale_fit(container, size, ratio);
        }
        case scaling_mode::scale_fill: {
            return calculate_scale_fill(container, size, ratio);
        }
    }
}
