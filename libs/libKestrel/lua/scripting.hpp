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

#pragma once

#include <memory>
#include <string>
#include <libKestrel/version.hpp>

#if !defined(_STR) && !defined(STR)
#   define STR(_s) _STR(_s)
#   define _STR(_s) #_s
#endif

#if !defined(Available)
#   define Available(_version)          "available=" STR(_version) "/"
#endif

#if !defined(Deprecated)
#   define Deprecated(_version)         "deprecated=" STR(_version) "/"
#endif

#if !defined(Available_Unknown)
#   define Available_Unknown            Available(LIB_KESTREL_VERSION_LEGACY)
#endif

#if !defined(Available_0_8)
#   define Available_0_8                Available(LIB_KESTREL_VERSION_0_8)
#endif
#if !defined(Deprecated_0_8)
#   define Deprecated_0_8               Deprecated(LIB_KESTREL_VERSION_0_8)
#endif

#if !defined(Available_0_9)
#   define Available_0_9                Available(LIB_KESTREL_VERSION_0_9)
#endif
#if !defined(Deprecated_0_9)
#   define Deprecated_0_9               Deprecated(LIB_KESTREL_VERSION_0_9)
#endif

#if !defined(CurrentVersion)
#   define CurrentVersion               Available_0_9
#endif

#if !defined(lua_api)
#   define lua_api(_name, _api_version) [[clang::annotate("luatool/symbol=" STR(_name) "/available=" STR(_api_version))]]
#endif

#if !defined(lua_use_namespace)
#   define lua_use_namespace [[clang::annotate("luatool/namespace")]]
#endif

#if !defined(luatool_type_fix)
#   define luatool_type_fix(_type, _name) [[clang::annotate("luatool/symbol=" STR(_name) "/type-fix=" STR(_type))]]
#endif

#if !defined(lua_declare_named)
#   define lua_declare_named(_name, _type)   [[clang::annotate("luatool/symbol=" STR(_name) "/template-variant=" STR(_type))]]
#endif

#if !defined(lua_setter)
#   define lua_setter(_name, _api_version)  [[clang::annotate("luatool/setter/symbol=" STR(_name) "/available=" STR(_api_version))]] [[maybe_unused]]
#endif

#if !defined(lua_getter)
#   define lua_getter(_name, _api_version)  [[clang::annotate("luatool/getter/symbol=" STR(_name) "/available=" STR(_api_version))]] [[maybe_unused]]
#endif

#if !defined(lua_data)
#   define lua_data(_name, _mutatability, _api_version)  [[clang::annotate("luatool/symbol=" STR(_name) "/" STR(_mutatability) "/available=" STR(_api_version))]]
#endif

#if !defined(lua_function)
#   define lua_function(_name, _api_version)  [[clang::annotate("luatool/symbol=" STR(_name) "/available=" STR(_api_version))]] [[maybe_unused]]
#endif

#if !defined(lua_constructor)
#   define lua_constructor(_api_version)  [[clang::annotate("luatool/constructor/available=" STR(_api_version))]] [[maybe_unused]]
#endif

#if !defined(lua_case)
#   define lua_case(_name, _api_version)  [[clang::annotate("luatool/symbol=" STR(_name) "/available=" STR(_api_version))]] [[maybe_unused]]
#endif

#if !defined(has_named_constructable_lua_api)
#   define has_named_constructable_lua_api(_type)  \
        [[clang::annotate("luatool/enrollment/reference=lua_reference/named")]] \
        static auto enroll_object_api_in_state(const std::string& name, const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void; \
        typedef luabridge::RefCountedPtr<_type> lua_reference;
#endif

#if !defined(has_constructable_lua_api)
#   define has_constructable_lua_api(_type)  \
        [[clang::annotate("luatool/enrollment/reference=lua_reference")]] \
        static auto enroll_object_api_in_state(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void; \
        typedef luabridge::RefCountedPtr<_type> lua_reference;
#endif

#if !defined(has_lua_api)
#   define has_lua_api \
        [[clang::annotate("luatool/enrollment")]] \
        static auto enroll_object_api_in_state(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void;
#endif

#if !defined(luatool_no_implementation)
#   define luatool_no_implementation
#endif

#if !defined(construct_custom_lua_api)
#   define construct_custom_lua_api(_type) \
        auto _type::enroll_object_api_in_state(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void
#endif