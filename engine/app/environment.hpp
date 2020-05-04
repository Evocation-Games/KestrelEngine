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

#if !defined(KESTREL_ENVIRONMENT_HPP)
#define KESTREL_ENVIRONMENT_HPP

#include <memory>
#include "libDiamane/ui/window.hpp"
#include "foundation/math/size.hpp"

namespace kestrel { namespace app {

    class environment
    {
    private:
        std::shared_ptr<diamane::ui::window> m_game_window { nullptr };

        environment();

    public:
        environment(const environment&) = delete;
        environment& operator=(const environment&) = delete;
        environment(environment&&) = delete;
        environment& operator=(environment&&) = delete;

        static auto& global()
        {
            static environment instance;
            return instance;
        }

        auto start(int argc, const char **argv) -> int;

        auto size() const -> math::size;

        auto set_game_coordinates() const -> void;
        auto set_interface_coordinates() const -> void;
    };

}};

#endif //KESTREL_ENVIRONMENT_HPP
