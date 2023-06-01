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

#include <cassert>
#include <complex>
#include <cmath>
#include <libKDL/modules/kdl/kdl.hpp>
#include <libFoundation/hashing/hashing.hpp>

auto kdl::modules::kdl::install_functions(interpreter::scope *scope) -> void
{
    assert(scope);

    scope->add_function({ "hash", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        auto str = arguments.at(0).string_value();
        auto hash = foundation::hashing::string(str);
        return interpreter::token(static_cast<std::int64_t>(hash));
    }});

    scope->add_function({ "StringID", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        auto str = arguments.at(0).string_value();
        auto hash = foundation::hashing::string(str);
        return interpreter::token(static_cast<std::int64_t>(hash));
    }});

    scope->add_function({ "to_integer", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(arguments.at(0).integer_value());
    }});

    scope->add_function({ "to_string", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(arguments.at(0).string_value());
    }});

    scope->add_function({ "sin", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(std::sin(arguments.at(0).decimal_value()));
    }});

    scope->add_function({ "cos", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(std::cos(arguments.at(0).decimal_value()));
    }});

    scope->add_function({ "tan", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(std::tan(arguments.at(0).decimal_value()));
    }});

    scope->add_function({ "to_degrees", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(arguments.at(0).decimal_value() * (180.0 / M_PI));
    }});

    scope->add_function({ "to_radians", [&] (interpreter::scope *scope, const std::vector<interpreter::token>& arguments) -> interpreter::token {
        return interpreter::token(arguments.at(0).decimal_value() * (M_PI / 180.0));
    }});

    scope->add_variable("pi", M_PI);

}