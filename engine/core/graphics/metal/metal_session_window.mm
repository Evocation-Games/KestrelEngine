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

#include <utility>
#include "core/graphics/metal/metal_session_window.h"
#include "core/support/macos/cocoa/window.h"

// MARK: - Construction

graphics::metal::session_window::session_window(std::shared_ptr<environment> env)
    : graphics::session_window(std::move(env)), m_window(std::make_shared<cocoa::window>())
{

}

// MARK: - Accessors

auto graphics::metal::session_window::set_title(const std::string &title) -> void
{
    m_window->set_title(title);
}

auto graphics::metal::session_window::set_size(const math::size &size) -> void
{
    m_window->set_size(static_cast<int>(size.width), static_cast<int>(size.height));
}
