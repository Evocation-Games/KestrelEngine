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
#include <libScripting/annotations/macro.hpp>

#if !defined(Available_Unknown)
#   define Available_Unknown                Available(LIB_KESTREL_VERSION_LEGACY)
#endif

#if !defined(Available_0_8)
#   define Available_0_8                    Available(LIB_KESTREL_VERSION_0_8)
#endif
#if !defined(Deprecated_0_8)
#   define Deprecated_0_8                   Deprecated(LIB_KESTREL_VERSION_0_8)
#endif

#if !defined(Available_0_9)
#   define Available_0_9                    Available(LIB_KESTREL_VERSION_0_9)
#endif

#if !defined(Deprecated_0_9)
#   define Deprecated_0_9                   Deprecated(LIB_KESTREL_VERSION_0_9)
#endif

#if !defined(CurrentVersion)
#   define CurrentVersion                   LIB_KESTREL_VERSION_CURRENT
#endif

#if !defined(lua_api)
#   define lua_api(_name, _api_version)     ScriptingAnnotation2(lua, Symbol(_name), _api_version)
#endif

#if !defined(lua_use_namespace)
#   define lua_use_namespace                ScriptingAnnotation(lua, Namespace)
#endif

#if !defined(luatool_type_fix)
#   define luatool_type_fix(_type, _name)   ScriptingAnnotation2(lua, SymbolAnnotation(_name), Type(_type))
#endif

#if !defined(lua_declare_named)
#   define lua_declare_named(_name, _type)  ScriptingAnnotation2(lua, SymbolAnnotation(_name), TemplateVariant(_type))
#endif

#if !defined(lua_setter)
#   define lua_setter(_name, _api_version)  ScriptingAnnotation3(lua, SetterAnnotation, SymbolAnnotation(_name), _api_version) [[maybe_unused]]
#endif

#if !defined(lua_getter)
#   define lua_getter(_name, _api_version)  ScriptingAnnotation3(lua, GetterAnnotation, SymbolAnnotation(_name), _api_version) [[maybe_unused]]
#endif

#if !defined(lua_data)
#   define lua_data(_name, _mutatability, _api_version)  ScriptingAnnotation3(lua, SymbolAnnotation(_name), MutabilityAnnotation(_mutability), _api_version)
#endif

#if !defined(lua_function)
#   define lua_function(_name, _api_version)  ScriptingAnnotation2(lua, SymbolAnnotation(_name), _api_version) [[maybe_unused]]
#endif

#if !defined(lua_constructor)
#   define lua_constructor(_api_version)  ScriptingAnnotation2(lua, ConstructorAnnotation, _api_version) [[maybe_unused]]
#endif

#if !defined(lua_case)
#   define lua_case(_name, _api_version)  ScriptingAnnotation2(lua, SymbolAnnotation(_name), _api_version)
#endif

#if !defined(has_named_constructable_lua_api)
#   define has_named_constructable_lua_api(_type) \
        ScriptingAnnotation3(lua, RequiresEnrollment, RequiresEnrollmentName, ReferenceAnnotation(InstanceReference)) \
        static auto EnrollmentFunction(const std::string& name, const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void; \
        typedef luabridge::RefCountedPtr<_type> InstanceReference;
#endif

#if !defined(has_constructable_lua_api)
#   define has_constructable_lua_api(_type) \
        ScriptingAnnotation2(lua, RequiresEnrollment, ReferenceAnnotation(InstanceReference)) \
        static auto EnrollmentFunction(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void; \
        typedef luabridge::RefCountedPtr<_type> InstanceReference;
#endif

#if !defined(has_lua_api)
#   define has_lua_api \
        ScriptingAnnotation1(lua, RequiresEnrollment) \
        static auto EnrollmentFunction(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void;
#endif

#if !defined(luatool_no_implementation)
#   define luatool_no_implementation
#endif

#if !defined(construct_custom_lua_api)
#   define construct_custom_lua_api(_type) \
        auto _type::EnrollmentFunction(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void
#endif