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

#include "core/ui/imgui/table.hpp"
#include "core/ui/imgui/label.hpp"
#include "core/environment.hpp"

// MARK: - Luia

auto ui::imgui::table::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<table>("Table")
                .addConstructor<auto(*)(float)->void, lua_reference>()
                .addProperty("header", &table::header, &table::set_header)
                .addFunction("addRow", &table::add_row)
            .endClass()
        .endNamespace();

    cell::enroll_object_api_in_state(lua);
}

auto ui::imgui::table::cell::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<table::cell>("TableCell")
                .addConstructor<auto(*)()->void, lua_reference>()
                .addStaticFunction("Text", &cell::text)
                .addFunction("addWidget", &cell::add_widget)
                .addFunction("onSelected", &cell::on_selected)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::table::table(float width)
    : m_width(width)
{
}

ui::imgui::table::cell::cell()
{
}

auto ui::imgui::table::cell::text(const std::string &text) -> lua_reference
{
    table::cell::lua_reference cell_widget { new table::cell() };
    label::lua_reference text_widget { new label(text) };

    if (auto env = environment::active_environment().lock()) {
        auto lua = env->lua_runtime();
        cell_widget->add_widget(luabridge::LuaRef(lua->internal_state(), text_widget));
    }

    return cell_widget;
}

// MARK: - Operations

auto ui::imgui::table::add_row(luabridge::LuaRef row) -> void
{
    if (!row.state()) {
        return;
    }

    if (row.isFunction()) {
        table::cell::lua_reference cell { new table::cell() };
        row(cell);

        if (auto env = environment::active_environment().lock()) {
            m_rows.emplace_back(luabridge::LuaRef(env->lua_runtime()->internal_state(), cell));
        }
    }
    else {
        m_rows.emplace_back(row);
    }
}

auto ui::imgui::table::cell::add_widget(luabridge::LuaRef widget) -> void
{
    m_contents.add_widget(widget);
}

auto ui::imgui::table::cell::on_selected(luabridge::LuaRef callback) -> void
{
    m_on_selected = callback;
}

auto ui::imgui::table::cell::selected() -> void
{
    if (m_on_selected.state() && m_on_selected.isFunction()) {
        m_on_selected();
    }
}

// MARK: - Drawing

auto ui::imgui::table::draw() -> void
{
    auto size = ImGui::GetContentRegionAvail();

    if (m_width > 0) {
        size.x *= m_width;
    }

    if (ImGui::BeginTable(identifier_string(), 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_ScrollY, size)) {
        ImGui::TableSetupColumn(m_header.c_str(), ImGuiTableColumnFlags_WidthStretch);

        if (!m_header.empty()) {
            ImGui::TableHeadersRow();
        }

        auto row_count = m_rows.size();
        for (auto i = 0; i < row_count; ++i) {
            auto row = m_rows[i];
            auto selected = m_selected_row == i + 1;

            if (scripting::lua::ref_isa<table::cell>(row)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                auto cell = row.cast<table::cell::lua_reference>();
                cell->draw();

                ImGui::SameLine();
                if (ImGui::Selectable(cell->identifier_string(), selected, ImGuiSelectableFlags_SpanAllColumns)) {
                    m_selected_row = i + 1;
                    cell->selected();
                }
            }
        }

        ImGui::EndTable();
    }
}

auto ui::imgui::table::cell::draw() -> void
{
    m_contents.draw();
}