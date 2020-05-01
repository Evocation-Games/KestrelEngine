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

#if !defined(KESTREL_CACHE_HPP)
#define KESTREL_CACHE_HPP

#include <any>
#include <string>
#include <optional>
#include <unordered_map>
#include "foundation/assets/resource_reference.hpp"

namespace kestrel { namespace assets {

    class cache
    {
    private:
        std::unordered_map<std::size_t, std::any> m_assets;

        cache();

    public:
        cache(const cache&) = delete;
        cache& operator=(const cache&) = delete;
        cache(cache&&) = delete;
        cache& operator=(cache&&) = delete;

        static auto& global()
        {
            static cache instance;
            return instance;
        }

        auto add(std::string type, resource_reference::lua_reference ref, std::any asset) -> void;
        auto fetch(std::string type, resource_reference::lua_reference ref) const -> std::optional<std::any>;
    };

}};

#endif //KESTREL_CACHE_HPP
