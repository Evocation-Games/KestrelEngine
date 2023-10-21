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

#pragma once

#include <any>
#include <string>
#include <optional>
#include <unordered_map>
#include <tuple>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/key.hpp>
#include <libKestrel/clock/clock.hpp>

/**
 * The `kestrel::cache` namespace encapsulates functionality related to caching resource assets and data,
 * in order to help reducing asset loading and processing times.
 */
namespace kestrel::cache
{
    /**
     * Add the specified asset to the cache using the specified reference as an identifier.
     * @param ref   The `resource::descriptor` that identifies the asset.
     * @param asset The asset to cache.
     */
    auto add(const resource::descriptor::lua_reference& ref, const std::any& asset) -> void;

    /**
     * Fetch the asset for the specified `resource::descriptor` if it exists.
     * @param ref   The `resource::descriptor` that identifies the asset.
     * @return      The asset if it exists.
     */
    auto fetch(const resource::descriptor::lua_reference& ref) -> std::optional<std::any>;

    /**
     * Purge all assets from the cache regardless of current age and access time.
     */
    auto purge_all() -> void;

    /**
     * Remove all assets from the cache that have not been accessed for at least 2 minutes.
     */
    auto purge_unused() -> void;
}
