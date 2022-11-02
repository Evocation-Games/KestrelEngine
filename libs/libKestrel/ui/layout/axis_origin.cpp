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

#include <libKestrel/ui/layout/axis_origin.hpp>

// MARK: - Calculations

auto kestrel::ui::layout::origin_for_axis(const math::size &size, enum axis_origin axis) -> math::point
{
    switch (axis) {
        case ui::layout::axis_origin::top_left: {
            return { 0, 0 };
        }
        case ui::layout::axis_origin::center_left: {
            return { 0, size.height / 2.0 };
        }
        case ui::layout::axis_origin::bottom_left: {
            return { 0, size.height };
        }
        case ui::layout::axis_origin::top_center: {
            return { size.width / 2.0, 0 };
        }
        case ui::layout::axis_origin::center: {
            return { size.width / 2.0, size.height / 2.0 };
        }
        case ui::layout::axis_origin::bottom_center: {
            return { size.width / 2.0, size.height };
        }
        case ui::layout::axis_origin::top_right: {
            return { size.width, 0 };
        }
        case ui::layout::axis_origin::center_right: {
            return { size.width, size.height / 2.0 };
        }
        case ui::layout::axis_origin::bottom_right: {
            return { size.width, size.height };
        }
    }
}

auto kestrel::ui::layout::position_for_axis(const math::size &container, enum axis_origin axis, const math::size &size) -> math::point
{
    math::point origin;
    switch (axis) {
        case axis_origin::top_left: {
            break;
        }
        case axis_origin::center_left: {
            origin.y = (container.height - size.height) / 2.0;
            break;
        }
        case axis_origin::bottom_left: {
            origin.y = (container.height - size.height);
            break;
        }
        case axis_origin::top_center: {
            origin.x = (container.width - size.width) / 2.0;
            break;
        }
        case axis_origin::center: {
            origin.x = (container.width - size.width) / 2.0;
            origin.y = (container.height - size.height) / 2.0;
            break;
        }
        case axis_origin::bottom_center: {
            origin.x = (container.width - size.width) / 2.0;
            origin.y = (container.height - size.height);
            break;
        }
        case axis_origin::top_right: {
            origin.x = (container.width - size.width);
            break;
        }
        case axis_origin::center_right: {
            origin.x = (container.width - size.width);
            origin.y = (container.height - size.height) / 2.0;
            break;
        }
        case axis_origin::bottom_right: {
            origin.x = (container.width - size.width);
            origin.y = (container.height - size.height);
            break;
        }
    }
    return origin;
}