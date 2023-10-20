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

#include <fstream>
#include <libKDL/project/project.hpp>
#include <libKDL/codegen/kdl/file.hpp>
#include <libKDL/codegen/lua/ast.hpp>
#include <libKDL/codegen/lua/kestrel_api.hpp>

// MARK: - Construction

kdl::project::project(const std::string& path, const std::string& type)
    : m_path(path),
      m_type(type),
      m_scenes_path(m_path.appending_path_component("scenes")),
      m_scripts_path(m_path.appending_path_component("scripts"))
{
    m_name = m_path.name();
}

// MARK: - Configuration

auto kdl::project::add_author(const std::string& name) -> void
{
    m_author.emplace_back(name);
}

auto kdl::project::set_version(const std::string& name) -> void
{
    m_version = name;
}

auto kdl::project::set_copyright(const std::string& name) -> void
{
    m_copyright = name;
}

auto kdl::project::set_license(const std::string& name) -> void
{
    m_license = name;
}

auto kdl::project::add_basic_scene(const std::string &name) -> void
{
    if (m_default_scene.empty()) {
        m_default_scene = name;
    }
    m_scenes.emplace_back(name);
}

auto kdl::project::add_basic_dialog(const std::string &name) -> void
{
    if (m_default_scene.empty()) {
        m_default_scene = name;
    }
    m_dialogs.emplace_back(name);
}

// MARK: - Build

auto kdl::project::build() -> void
{
    // If there are no scenes, then add an example.
    if (m_scenes.empty() && m_dialogs.empty()) {
        m_scenes.emplace_back("ExampleScene");
    }

    // Create the project directory structure.
    m_path.create_directory(false);
    m_scripts_path.create_directory(false);
    m_scenes_path.create_directory(false);

    build_project_file();
    build_main_script();

    for (const auto& scene : m_scenes) {
        build_scene_file(scene);
        build_scene_script(scene);
    }

    for (const auto& dialog : m_dialogs) {
        build_dialog_file(dialog);
        build_dialog_script(dialog);
    }
}

auto kdl::project::build_project_file() -> void
{
    codegen::kdl::file project_file(m_path.appending_path_component("project.kdlproj"));

    // Header Comments
    project_file.comment(" ");
    project_file.comment("" + m_name + " Project File");
    project_file.comment(" ");

    // Metadata
    project_file.nl();
    project_file.metadata_project(m_name);
    for (const auto& author : m_author) {
        project_file.metadata_author(author);
    }
    project_file.metadata_version(m_version);
    project_file.metadata_license(m_license);
    project_file.metadata_copyright(m_copyright);

    // Import Modules
    project_file.nl();
    project_file.import_module("Kestrel");

    // Import the scenes/dialogs
    if (!m_scenes.empty()) {
        project_file.nl();
        project_file.comment("Import any project scenes");
        for (const auto &scene: m_scenes) {
            project_file.import_file("scenes/" + scene + "/" + scene + ".kdl");
        }
    }

    if (!m_dialogs.empty()) {
        project_file.nl();
        project_file.comment("Import any project dialogs");
        for (const auto &dialog: m_dialogs) {
            project_file.import_file("scenes/" + dialog + "/" + dialog + ".kdl");
        }
    }

    // Setup a component for the main script.
    project_file.nl();
    project_file.comment("Define a component with the main game script.");
    project_file.component("_kestrelMain", "LuaScript", 0, [] (const auto& component) {
        component->comment("Lua Source Files");
        component->files("Scripts", [] (const auto& files) {
            files->add("main.lua");
        });
    });

    project_file.save();
}

auto kdl::project::build_scene_file(const std::string &name) -> void
{
    auto scene_dir = m_scenes_path.appending_path_component(name);
    scene_dir.create_directory(false);
    codegen::kdl::file scene_file(scene_dir.appending_path_component(name + ".kdl"));

    scene_file.comment(" ");
    scene_file.comment("Scene: " + name);
    scene_file.comment(" ");

    scene_file.nl();
    scene_file.component(name, "LuaScript", 1000, [name] (const auto& component) {
        component->files("scenes/" + name + "/scripts", [name] (const auto& files) {
            files->add(name + ".lua");
        });
    });

    scene_file.nl();
    scene_file.declaration(name, "SceneDefinition", [name, scene_file] (const auto& declaration) {
        declaration->new_resource(name + " Definition", [name] (const auto& resource) {
            resource->set("Script", resource->ref(name, "LuaScript", 1000));
            resource->set("Interface", resource->ref(name, "SceneInterface", 1000));
            resource->set("DLOG", resource->ref(-1));
        });
    });


    scene_file.nl();
    scene_file.scene(name, false, 1000, [name, scene_file] (const auto& scene) {
        scene->set("Title", scene->string(name));
        scene->set("Size", scene->frame_size(0, 0));
    });

    scene_file.save();
}

auto kdl::project::build_dialog_file(const std::string &name) -> void
{
    auto scene_dir = m_scenes_path.appending_path_component(name);
    scene_dir.create_directory(false);
    codegen::kdl::file dialog_file(scene_dir.appending_path_component(name + ".kdl"));

    dialog_file.comment(" ");
    dialog_file.comment("Dialog: " + name);
    dialog_file.comment(" ");

    dialog_file.nl();
    dialog_file.component(name, "LuaScript", 1000, [name] (const auto& component) {
        component->files("scenes/" + name + "/scripts", [name] (const auto& files) {
            files->add(name + ".lua");
        });
    });

    dialog_file.nl();
    dialog_file.declaration(name, "SceneDefinition", [name, dialog_file] (const auto& declaration) {
        declaration->new_resource(name + " Definition", [name] (const auto& resource) {
            resource->set("Script", resource->ref(name, "LuaScript", 1000));
            resource->set("Interface", resource->ref(name, "SceneInterface", 1000));
            resource->set("DLOG", resource->ref(-1));
        });
    });

    dialog_file.nl();
    dialog_file.scene(name, true, 1000, [name, dialog_file] (const auto& dialog) {
        dialog->set("Title", dialog->string(name));
        dialog->set("Size", dialog->frame_size(300, 300));
        dialog->set("Flags", dialog->options({ "UseImGui", "ShowsTitleBar" }));
        dialog->nl();

        dialog->control("Button", "TestButton", [&] (const auto& btn) {
            btn->set("Frame", dialog->frame(20, 20, 100, 30));
            btn->set("Value", dialog->string("Test Action"));
            btn->set("Action", dialog->function("Action"));
        });
    });

    dialog_file.save();
}

auto kdl::project::build_main_script() -> void
{
    codegen::lua::ast::generator main_generator;
    auto kestrel = codegen::lua::kestrel_api::create(&main_generator);

    main_generator.add_comment(" ");
    main_generator.add_comment(m_name + " Main Script");
    main_generator.add_comment(" ");

    main_generator.new_line();
    main_generator.add_comment("Configure the Kestrel Runtime");
    main_generator.emit(
        main_generator.call(kestrel.set_game_window_title, {
            main_generator.string(m_name)
        })
    );
    main_generator.emit(
        main_generator.call(kestrel.set_game_window_size, {
            main_generator.construct(kestrel.size_klass, {
                main_generator.number(1280),
                main_generator.number(720)
            })
        })
    );

    main_generator.new_line();
    main_generator.add_comment("Push the default scene");
    main_generator.emit(
        main_generator.call(
            main_generator.call(kestrel.scene, { main_generator.string(m_default_scene) }),
            kestrel.push
        )
    );

    export_file(m_scripts_path.appending_path_component("main.lua"), main_generator.generate());
}

auto kdl::project::build_scene_script(const std::string &name) -> void
{
    codegen::lua::ast::generator script_generator;
    auto kestrel = codegen::lua::kestrel_api::create(&script_generator);

    script_generator.add_comment(" ");
    script_generator.add_comment("Scene: " + name);
    script_generator.add_comment("Project: " + m_name);
    script_generator.add_comment(" ");
    script_generator.new_line();

    auto scene = script_generator.symbol("scene");
    script_generator.local_assign(
        scene, script_generator.member(script_generator.symbol("current"), script_generator.symbol("Scene"))
    );

    script_generator.new_line();
    script_generator.emit(
        script_generator.call(scene, kestrel.update, {
            script_generator.annonymous_function([&] (auto fn) {
                script_generator.add_comment("Update entities here...");
            })
        })
    );

    export_file(
        m_scenes_path.appending_path_component(name)
            .appending_path_component("scripts")
            .appending_path_component(name + ".lua"),
        script_generator.generate()
    );
}

auto kdl::project::build_dialog_script(const std::string &name) -> void
{
    codegen::lua::ast::generator script_generator;
    auto kestrel = codegen::lua::kestrel_api::create(&script_generator);

    script_generator.add_comment(" ");
    script_generator.add_comment("Scene: " + name);
    script_generator.add_comment("Project: " + m_name);
    script_generator.add_comment(" ");
    script_generator.new_line();

    auto scene_dialog = script_generator.symbol(name + "Dialog");
    auto dialog = script_generator.symbol("dialog");
    script_generator.local_assign(
        scene_dialog, script_generator.member(script_generator.symbol("current"), script_generator.symbol("Scene"))
    );
    script_generator.local_assign(
        dialog, script_generator.member(script_generator.symbol("Dialog"), scene_dialog)
    );

    script_generator.new_line();
    script_generator.emit(
        script_generator.call(scene_dialog, kestrel.on_close, {
            script_generator.annonymous_function([&] (auto fn) {
                script_generator.assign(dialog, script_generator.nil());
                script_generator.assign(scene_dialog, script_generator.nil());
            })
        })
    );

    script_generator.new_line();
    script_generator.emit(
        script_generator.call(scene_dialog, kestrel.bind, {
            script_generator.userdata([&] (auto ud) {
                script_generator.assign(
                    script_generator.symbol("Action"),
                    script_generator.annonymous_function([&] (auto fn) {
                        script_generator.emit(script_generator.call(
                            script_generator.declare_function(true, script_generator.symbol("print")), {
                                script_generator.string("You pressed the button!")
                            }
                        ));
                    })
                );
            })
        })
    );

    export_file(
        m_scenes_path.appending_path_component(name)
                     .appending_path_component("scripts")
                     .appending_path_component(name + ".lua"),
        script_generator.generate()
    );
}

auto kdl::project::export_file(const foundation::filesystem::path &path, const std::string &contents) -> void
{
    // Ensure the directory for the path exists.
    path.create_directory();

    // Save the contents to disk.
    std::ofstream file(path.string(), std::ofstream::out);
    if (file.is_open()) {
        file << contents << std::endl;
        file.flush();
        file.close();
    }
}