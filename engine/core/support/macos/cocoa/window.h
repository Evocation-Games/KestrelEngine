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

#if __APPLE__ && !defined(KESTREL_WINDOW_H)
#define KESTREL_WINDOW_H

#include <memory>
#include "core/support/macos/cocoa/object.h"

namespace cocoa
{
    class view;

    class window: public std::enable_shared_from_this<cocoa::window>, public cocoa::object
    {
    public:
        explicit window();
        window(void *handle);

        auto set_title(const std::string& title) -> void;
        auto title() const -> std::string;

        auto set_size(const int& width, const int& height) -> void;

        auto set_content_view(const std::shared_ptr<cocoa::view>& view) -> void;
    };
}

#endif //KESTREL_WINDOW_H
