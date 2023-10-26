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

#include <iostream>
#include <libMetalRenderer/resource/shader/compiler.h>
#include <libMetalRenderer/resource/shader/default_shader.hpp>
#include <libMacOS/cocoa/string.h>

// MARK: - Construction

renderer::metal::resource::shader::compiler::compiler(id<MTLDevice> device)
    : m_device(device)
{}

// MARK: - Compiler

auto renderer::metal::resource::shader::compiler::compile(
    const renderer::shader::library &library,
    MTLPixelFormat format
) -> shader::program {
    auto vertex = library.vertex_function();
    auto fragment = library.fragment_function();

    // The first job is to actually assemble the library into a single string that can be passed to the
    // Metal device for compilation.
    std::string source(shader_code_template);

    std::string vertex_function_token("@@VERTEX_FUNCTION@@");
    auto vertex_function_it = source.find(vertex_function_token);
    if (vertex_function_it != std::string::npos) {
        source.replace(vertex_function_it, vertex_function_token.length(), vertex.code());
    }

    std::string fragment_function_token("@@FRAGMENT_FUNCTION@@");
    auto fragment_function_it = source.find(fragment_function_token);
    if (fragment_function_it != std::string::npos) {
        source.replace(fragment_function_it, fragment_function_token.length(), fragment.code());
    }

    // Now that the library source is produced, proceed to actually compiling. This is a potentially blocking
    // process.
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    NSString *library_source = macos::cocoa::string::to(source);
    __block id<MTLLibrary> new_library = nil;
    [m_device newLibraryWithSource:library_source options:nil completionHandler:^(id<MTLLibrary> metal_library, NSError *error) {
        if (metal_library && !error) {
            new_library = metal_library;
        }
        else {
            // TODO: Handle errors gracefully.
            std::cerr << error.description.UTF8String << std::endl;
        }
        dispatch_group_leave(group);
    }];

    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);

    if (!new_library) {
        // Failed to compile, so return a null program.
        return {};
    }

    // Now that the library is compiled, produce a shader program.
    id<MTLFunction> metal_vertex_function = [new_library newFunctionWithName:macos::cocoa::string::to(vertex.name())];
    id<MTLFunction> metal_fragment_function = [new_library newFunctionWithName:macos::cocoa::string::to(fragment.name())];

    // TODO: Check for missing functions

    auto program_id = m_next_program_id++;
    shader::program program(
        m_device,
        program_id,
        library.name(),
        metal_vertex_function,
        metal_fragment_function,
        format
    );
    return std::move(program);
}
