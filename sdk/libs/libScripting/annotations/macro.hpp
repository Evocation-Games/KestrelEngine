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

#pragma once

#include <libFoundation/macro.hpp>

#if !defined(InstanceReference)
#   define InstanceReference    lua_reference
#endif

#if !defined(EnrollmentFunction)
#   define EnrollmentFunction    enroll_object_api_in_state
#endif

#if !defined(ScriptingAnnotation)
#   define ScriptingAnnotation(_lang, ...)  [[clang::annotate(STR(_lang) "/" __VA_ARGS__)]] [[maybe_unused]]
#   define ScriptingAnnotation1(_lang, _a1)  [[clang::annotate(STR(_lang) "/" STR(_a1))]] [[maybe_unused]]
#   define ScriptingAnnotation2(_lang, _a1, _a2)  [[clang::annotate(STR(_lang) "/" STR(_a1) STR(_a2))]] [[maybe_unused]]
#   define ScriptingAnnotation3(_lang, _a1, _a2, _a3)  [[clang::annotate(STR(_lang) "/" STR(_a1) STR(_a2) STR(_a3))]] [[maybe_unused]]
#   define ScriptingAnnotation4(_lang, _a1, _a2, _a3, _a4)  [[clang::annotate(STR(_lang) "/" STR(_a1) STR(_a2) STR(_a3) STR(_a4))]] [[maybe_unused]]
#endif

#if !defined(ScriptingAnnotationKeyValue)
#   define ScriptingAnnotationKeyValue(_key, _value)  STR(_key) "=" STR(_value) "/"
#endif

#if !defined(Version)
#   define Version(_major, _minor, _fix)    _major._minor._fix
#endif

#if !defined(Available)
#   define Available(_version)              ScriptingAnnotationKeyValue(available, _version)
#endif

#if !defined(Deprecated)
#   define Deprecated(_version)             ScriptingAnnotationKeyValue(deprecated, _version)
#endif

#if !defined(Undocumented)
#   define Undocumented                     "undocumented/"
#endif

#if !defined(Namespace)
#   define Namespace                        "namespace/"
#endif

#if !defined(Constructor)
#   define Constructor                      "constructor/"
#endif

#if !defined(Setter)
#   define Setter                           "setter/"
#endif

#if !defined(Getter)
#   define Getter                           "getter/"
#endif

#if !defined(Mutability)
#   define Mutability(_mode)                ScriptingAnnotationKeyValue(mutability, _mode)
#endif

#if !defined(RequiresEnrollment)
#   define RequiresEnrollment               "enrollment/"
#endif

#if !defined(RequiresEnrollmentName)
#   define RequiresEnrollmentName           "enrollment-name/"
#endif

#if !defined(Reference)
#   define Reference(_sym)                  ScriptingAnnotationKeyValue(reference, _sym)
#endif

#if !defined(Symbol)
#   define Symbol(_sym)                     ScriptingAnnotationKeyValue(symbol, _sym)
#endif

#if !defined(ParameterType)
#   define ParameterType(_type)             ScriptingAnnotationKeyValue(parameter-type, _type)
#endif

#if !defined(TemplateVariant)
#   define TemplateVariant(_type)           ScriptingAnnotationKeyValue(template-variant, _type)
#endif

#if !defined(lua_use_namespace)
#   define lua_use_namespace                ScriptingAnnotation(lua)
#endif

#if !defined(lua_fix_parameter_type)
#   define lua_fix_parameter_type(_type, _name) ScriptingAnnotation2(lua, Symbol(_name), ParameterType(_type))
#endif