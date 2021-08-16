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
#include <tuple>
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/clock/clock.hpp"

namespace asset
{

    class cache
    {
    private:
        std::unordered_map<std::size_t, std::tuple<std::any, rtc::clock::time>> m_assets;

    public:
        cache() = default;

        auto add(const std::string& type, const asset::resource_descriptor::lua_reference& ref, const std::any& asset) -> void;
        auto fetch(const std::string& type, const asset::resource_descriptor::lua_reference& ref) -> std::optional<std::any>;

        auto purge_unused() -> void;
    };

}


#endif //KESTREL_CACHE_HPP
