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

#include <libKestrel/ui/imgui/table.hpp>
#include <libKestrel/ui/imgui/label.hpp>
#include <libKestrel/kestrel.hpp>

// MARK: - Construction

kestrel::ui::imgui::table::table(float width)
    : m_width(width)
{
}

kestrel::ui::imgui::table_cell::table_cell()
{
}

auto kestrel::ui::imgui::table_cell::text(const std::string &text) -> lua_reference
{
    lua_reference cell_widget { new table_cell() };
    label::lua_reference text_widget { new label(text) };
    cell_widget->add_widget(luabridge::LuaRef(kestrel::lua_runtime()->internal_state(), text_widget));
    return cell_widget;
}

// MARK: - Operations

auto kestrel::ui::imgui::table::add_row(luabridge::LuaRef row) -> void
{
    if (!row.state()) {
        return;
    }

    if (row.isFunction()) {
        table_cell::lua_reference cell { new table_cell() };
        row(cell);
        m_rows.emplace_back(luabridge::LuaRef(kestrel::lua_runtime()->internal_state(), cell));
    }
    else {
        m_rows.emplace_back(row);
    }
}

auto kestrel::ui::imgui::table_cell::add_widget(luabridge::LuaRef widget) -> void
{
    m_contents.add_widget(widget);
}

auto kestrel::ui::imgui::table_cell::on_selected(luabridge::LuaRef callback) -> void
{
    m_on_selected = callback;
}

auto kestrel::ui::imgui::table_cell::selected() -> void
{
    if (m_on_selected.state() && m_on_selected.isFunction()) {
        m_on_selected();
    }
}

auto kestrel::ui::imgui::table_cell::on_double_click(luabridge::LuaRef callback) -> void
{
    m_on_double_click = callback;
}

auto kestrel::ui::imgui::table_cell::double_click() -> void
{
    if (m_on_double_click.state() && m_on_double_click.isFunction()) {
        m_on_double_click();
    }
}

// MARK: - Drawing

auto kestrel::ui::imgui::table::draw() -> void
{
    auto size = ImGui::GetContentRegionAvail();

    if (m_width > 0) {
        size.x *= m_width;
    }

    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    if (ImGui::BeginTable(identifier_string(), 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_ScrollY, size)) {
        ImGui::TableSetupColumn(m_header.c_str(), ImGuiTableColumnFlags_WidthStretch);

        if (!m_header.empty()) {
            ImGui::TableHeadersRow();
        }

        auto row_count = m_rows.size();
        for (auto i = 0; i < row_count; ++i) {
            auto row = m_rows[i];

            if (lua::ref_isa<table_cell>(row)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                auto cell = row.cast<table_cell::lua_reference>();
                auto id = cell->draw(m_selected_identifier);
                if (id == cell->identifier_string()) {
                    m_selected_row = i + 1;
                    m_selected_identifier = cell->identifier_string();
                }
                else if (id != "") {
                    m_selected_identifier = id;
                }
            }
        }

        ImGui::EndTable();
    }
}

auto kestrel::ui::imgui::table_cell::draw() -> void
{
    draw("");
}

auto kestrel::ui::imgui::table_cell::draw(const std::string& selected_id) -> std::string
{
    bool selected = (identifier_string() == selected_id);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    if (has_children()) {
        bool open = ImGui::TreeNodeEx(identifier_string(), ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::SameLine();
        m_contents.draw();
        if (open) {
            for (auto& cell : m_children) {
                if (cell->draw(selected_id) == cell->identifier_string()) {
                    ImGui::TreePop();
                    return cell->identifier_string();
                }
            }
            ImGui::TreePop();
        }
    }
    else {

        m_contents.draw();

        ImGui::SameLine();
        if (ImGui::Selectable(identifier_string(), selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            this->selected();

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                double_click();
            }
            
            return identifier_string();
        }

    }

    return "";
}

// MARK: - Accessors

auto kestrel::ui::imgui::table::selected_index() const -> std::uint32_t
{
    return m_selected_row;
}

auto kestrel::ui::imgui::table::set_selected_index(std::uint32_t idx) -> void
{
    m_selected_row = idx;
}

// MARK: - Children

auto kestrel::ui::imgui::table_cell::add_child(luabridge::LuaRef child) -> void
{
    lua_reference cell(new table_cell());

    if (child.state() && child.isFunction()) {
        child(cell);
    }

    m_children.emplace_back(cell);
}

auto kestrel::ui::imgui::table_cell::has_children() const -> bool
{
    return !m_children.empty();
}