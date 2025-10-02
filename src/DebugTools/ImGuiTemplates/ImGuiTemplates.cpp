#include "ImGuiTemplates.h"

#ifdef _DEBUG

#include <string>

void ImGuiTemplate::VariableTable(const std::string& _stringID, std::function<void()> _function)
{
    std::string id = _stringID + "_VARTABLE";
    ImGui::PushID(id.c_str());
    bool result = ImGui::BeginTable(_stringID.c_str(), 2, ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders);
    if (result)
    {
        ImGui::TableSetupColumn("変数名");
        ImGui::TableSetupColumn("値");
        ImGui::TableHeadersRow();

        _function();

        ImGui::EndTable();
    }

    ImGui::PopID();
    return;
}

#endif // DEBUG