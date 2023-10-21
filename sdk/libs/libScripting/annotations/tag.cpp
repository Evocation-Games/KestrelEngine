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

#include <libScripting/annotations/tag.hpp>

auto scripting::annotation::tag_name::from(const std::string &value) -> enum tag
{
    if (value == as_namespace) {
        return tag::as_namespace;
    }
    else if (value == constructor) {
        return tag::is_constructor;
    }
    else if (value == setter) {
        return tag::is_setter;
    }
    else if (value == getter) {
        return tag::is_getter;
    }
    else if (value == available) {
        return tag::available;
    }
    else if (value == deprecated) {
        return tag::deprecated;
    }
    else if (value == mutability) {
        return tag::mutability;
    }
    else if (value == enrollment) {
        return tag::is_enrollment;
    }
    else if (value == enrollment_name) {
        return tag::requires_enrollment_name;
    }
    else if (value == reference) {
        return tag::reference;
    }
    else if (value == symbol) {
        return tag::symbol;
    }
    else if (value == parameter_type) {
        return tag::parameter_type;
    }
    else if (value == template_variant) {
        return tag::template_variant;
    }
    else if (value == undocumented) {
        return tag::undocumented;
    }
    else {
        return tag::undefined;
    }
}

auto scripting::annotation::tag_name::from(enum tag tag) -> std::string
{
    switch (tag) {
        case tag::as_namespace: return as_namespace;
        case tag::is_constructor: return constructor;
        case tag::is_setter: return setter;
        case tag::is_getter: return getter;
        case tag::available: return available;
        case tag::deprecated: return deprecated;
        case tag::mutability: return mutability;
        case tag::is_enrollment: return enrollment;
        case tag::requires_enrollment_name: return enrollment_name;
        case tag::reference: return reference;
        case tag::symbol: return symbol;
        case tag::parameter_type: return parameter_type;
        case tag::template_variant: return template_variant;
        case tag::undocumented: return undocumented;
        case tag::undefined: return undefined;
    }
}