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


#include <libKestrel/util/availability.hpp>
#include <libKestrel/shared/shared_library_manager.hpp>
#include <libKestrel/shared/library.hpp>

// MARK: - Extension Loader

#if TARGET_MACOS
#include <dlfcn.h>

auto kestrel::shared_lib::manager::load_library(std::shared_ptr<kestrel::lua::runtime> runtime, const std::string &path) -> void
{
    auto dylib = dlopen(path.c_str(), RTLD_LAZY);
    if (!dylib) {
        // Log the error loading the dylib?
        return;
    }

    kestrel_shared_library_interface*(*dylib_initialize)() = (kestrel_shared_library_interface*(*)())dlsym(dylib, "initialize");
    if (!dylib_initialize) {
        // Log the error loading the entry point?
        return;
    }

    auto api = dylib_initialize();
    auto ns = runtime->global_namespace().beginNamespace("dylib");
    for (auto i = 0; i < api->count; ++i) {
        if (api->items[i].symbol_function) {
            ns.addCFunction(api->items[i].symbol_name, (int(*)(lua_State*))api->items[i].symbol_function);
        }
    }
    ns.endNamespace();
}
#else
auto kestrel::shared_lib::manager::load_library(std::shared_ptr<kestrel::lua::runtime> runtime, const std::string &path) -> void
{

}
#endif
