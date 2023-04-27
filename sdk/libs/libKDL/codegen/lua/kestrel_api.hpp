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

#include <libKDL/codegen/lua/ast.hpp>

namespace kdl::codegen::lua
{
    struct kestrel_api
    {
        ast::class_definition *resource_reader { nullptr };
        ast::symbol *load_helper { nullptr };
        ast::function_definition *load_helper_function { nullptr };
        ast::function_definition *read_signed_byte { nullptr };
        ast::function_definition *read_signed_short { nullptr };
        ast::function_definition *read_signed_long { nullptr };
        ast::function_definition *read_signed_quad { nullptr };
        ast::function_definition *read_byte { nullptr };
        ast::function_definition *read_short { nullptr };
        ast::function_definition *read_long { nullptr };
        ast::function_definition *read_quad { nullptr };
        ast::function_definition *read_pstr { nullptr };
        ast::function_definition *read_cstr { nullptr };
        ast::function_definition *read_cstr_of_length { nullptr };
        ast::function_definition *read_color { nullptr };
        ast::function_definition *read_reference { nullptr };
        ast::function_definition *read_typed_reference { nullptr };
        ast::function_definition *read_macintosh_rect { nullptr };
        ast::function_definition *read_rect { nullptr };
        ast::function_definition *read_point { nullptr };
        ast::function_definition *read_size { nullptr };
        ast::function_definition *skip { nullptr };
        ast::class_definition *color_klass { nullptr };
        ast::function_definition *color_klass_color_value { nullptr };
        ast::class_definition *container_klass { nullptr };
        ast::function_definition *container_global { nullptr };
        ast::function_definition *identified_resource { nullptr };
        ast::class_definition *command_decoder { nullptr };
        ast::function_definition *command_decoder_from { nullptr };

        static auto create(ast::generator *gen = nullptr) -> kestrel_api;
    };
}