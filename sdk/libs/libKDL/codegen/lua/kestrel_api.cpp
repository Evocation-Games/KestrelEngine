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

#include <libKDL/codegen/lua/kestrel_api.hpp>

auto kdl::codegen::lua::kestrel_api::create(ast::generator *gen) -> kestrel_api
{
    kestrel_api api;
    if (!api.load_helper && gen) {
        // Resource Loader
        api.load_helper = gen->symbol("kdl_resourceLoader");
        api.load_helper_function = gen->declare_function(true, api.load_helper);

        // Resource Reader
        api.resource_reader = gen->declare_class("ResourceReader", true);
        api.read_signed_byte = gen->declare_function(true, gen->symbol("readSignedByte"), api.resource_reader);
        api.read_signed_short = gen->declare_function(true, gen->symbol("readSignedShort"), api.resource_reader);
        api.read_signed_long = gen->declare_function(true, gen->symbol("readSignedLong"), api.resource_reader);
        api.read_signed_quad = gen->declare_function(true, gen->symbol("readSignedQuad"), api.resource_reader);
        api.read_byte = gen->declare_function(true, gen->symbol("readByte"), api.resource_reader);
        api.read_short = gen->declare_function(true, gen->symbol("readShort"), api.resource_reader);
        api.read_long = gen->declare_function(true, gen->symbol("readLong"), api.resource_reader);
        api.read_quad = gen->declare_function(true, gen->symbol("readQuad"), api.resource_reader);
        api.read_pstr = gen->declare_function(true, gen->symbol("readPStr"), api.resource_reader);
        api.read_cstr = gen->declare_function(true, gen->symbol("readCStr"), api.resource_reader);
        api.read_cstr_of_length = gen->declare_function(true, gen->symbol("readCStrOfLength"), api.resource_reader);
        api.read_color = gen->declare_function(true, gen->symbol("readColor"), api.resource_reader);
        api.read_reference = gen->declare_function(true, gen->symbol("readResourceReference"), api.resource_reader);
        api.read_typed_reference = gen->declare_function(true, gen->symbol("readTypedResourceReference"), api.resource_reader);
        api.read_macintosh_rect = gen->declare_function(true, gen->symbol("readMacintoshRect"), api.resource_reader);
        api.read_rect = gen->declare_function(true, gen->symbol("readRect"), api.resource_reader);
        api.read_point = gen->declare_function(true, gen->symbol("readPoint"), api.resource_reader);
        api.read_size = gen->declare_function(true, gen->symbol("readSize"), api.resource_reader);
        api.skip = gen->declare_function(true, gen->symbol("skip"), api.resource_reader);

        // Color
        api.color_klass = gen->declare_class("Color", true);
        api.color_klass_color_value = gen->declare_static_function(true, gen->symbol("colorValue"), api.color_klass);

        // Container
        api.container_klass = gen->declare_class("Container", true);
        api.container_global = gen->declare_static_function(true, gen->symbol("global"), api.container_klass);
        api.identified_resource = gen->declare_function(true, gen->symbol("identifiedResource"), api.container_klass);

        // Command Decoder
        api.command_decoder = gen->declare_class("Kestrel.Command.Decoder", true);
        api.command_decoder_from = gen->declare_static_function(true, gen->symbol("from"), api.command_decoder);

        // Kestrel Properties
        api.kestrel_namespace = gen->declare_class("Kestrel", true);
        api.set_game_window_title = gen->declare_static_function(true, gen->symbol("setGameWindowTitle"), api.kestrel_namespace);
        api.set_game_window_size = gen->declare_static_function(true, gen->symbol("setGameWindowSize"), api.kestrel_namespace);
        api.fullscreen = gen->declare_property(api.kestrel_namespace, gen->symbol("fullscreen"), nullptr, true);

        // Size
        api.size_klass = gen->declare_class("Size", true);

        // Scene
        api.scene_klass = gen->declare_class("Scene", true);
        api.scene = gen->declare_static_function(true, gen->symbol("Scene"), api.kestrel_namespace);
        api.push = gen->declare_function(true, gen->symbol("push"), api.scene_klass);
        api.on_close = gen->declare_function(true, gen->symbol("onClose"), api.scene_klass);
        api.close = gen->declare_function(true, gen->symbol("close"), api.scene_klass);
        api.configure_elements = gen->declare_function(true, gen->symbol("configureElementsOnOpen"), api.scene_klass);
        api.bind = gen->declare_function(true, gen->symbol("bind"), api.scene_klass);
        api.update = gen->declare_function(true, gen->symbol("update"), api.scene_klass);
    }
    return api;
}