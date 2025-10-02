#include "NiGuiDebug.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void NiGuiDebug::DrawDebugUI()
{
    #ifdef _DEBUG

    if (isDisplayComponentDataWindow_)
    {
        this->DisplayComponentDataWindow();
    }

    ImGuiTreeNodeFlags parentFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

    ImGuiWindowFlags wndflag = ImGuiWindowFlags_MenuBar;

    bool isOpen = ImGui::Begin("NiUI_DebugInfo", nullptr, wndflag);

    if (!isOpen)
    {
        ImGui::End();
        return;
    }

    this->DisplayMenuBar();

    if (ImGui::TreeNodeEx("Setting", parentFlags))
    {
        if (ImGui::TreeNode("Buffer Setting"))
        {
            ImGui::InputInt("Delete Element Threshold", &(setting_->deleteElementThreshold));
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("State", parentFlags))
    {
        if (ImGui::TreeNode("Validation"))
        {
            ImGui::Text("isInitialized : %s", state_->valid.isInitialized ? "true" : "false");
            ImGui::Text("isBeginFrame : %s", state_->valid.isBeginFrame ? "true" : "false");
            ImGui::Text("nestCount : %d", state_->valid.nestCount);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("ComponentID"))
        {
            ImGui::Text("TypeHover : %s", state_->componentID.typeHover.c_str());
            ImGui::Text("TypeActive : %s", state_->componentID.typeActive.c_str());
            ImGui::Text("ActiveComponentID : %s", state_->componentID.active.c_str());
            ImGui::Text("HoverComponentID : %s", state_->componentID.hover.c_str());
            ImGui::Text("PreHoverComponentID : %s", state_->componentID.preHover.c_str());
            ImGui::Text("PreActiveComponentID : %s", state_->componentID.preActive.c_str());
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Time"))
        {
            ImGui::Text("HoverTime : %d", state_->time.hover);
            ImGui::Text("ActiveTime : %d", state_->time.active);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("IO", parentFlags))
    {
        if (ImGui::TreeNode("WindowInfo"))
        {
            ImGui::Text("ClientSize : (%.2f, %.2f)", io_->windowInfo.clientSize.x, io_->windowInfo.clientSize.y);
            ImGui::Text("WindowScale : %.2f", io_->windowInfo.windowScale);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("InputData"))
        {
            ImGui::Text("CursorPos : (%.2f, %.2f)", io_->input.cursorPos.x, io_->input.cursorPos.y);
            ImGui::Text("TriggeredPos : (%.2f, %.2f)", io_->input.triggeredPos.x, io_->input.triggeredPos.y);
            ImGui::Text("DifferencePos : (%.2f, %.2f)", io_->input.differencePos.x, io_->input.differencePos.y);
            ImGui::Text("isLeft : %s", io_->input.isLeft ? "true" : "false");
            ImGui::Text("isLeftPre : %s", io_->input.isLeftPre ? "true" : "false");
            ImGui::Text("isRight : %s", io_->input.isRight ? "true" : "false");
            ImGui::Text("isRightPre : %s", io_->input.isRightPre ? "true" : "false");
            ImGui::Text("isMiddle : %s", io_->input.isMiddle ? "true" : "false");
            ImGui::Text("isMiddlePre : %s", io_->input.isMiddlePre ? "true" : "false");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("AudioHandle"))
        {
            ImGui::Text("ButtonHover : %d", io_->audioHnd.buttonHover);
            ImGui::Text("ButtonConfirm : %d", io_->audioHnd.buttonConfirm);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    ImGui::End();


    #endif // _DEBUG
}

void NiGuiDebug::DisplayComponentDataWindow()
{
    #ifdef _DEBUG

    bool isOpen = ImGui::Begin("Component data", &isDisplayComponentDataWindow_);

    if (!isOpen)
    {
        ImGui::End();
        return;
    }

    ImGui::SeparatorText("Search");

    std::string inputText = "";
    ImGuiInputFlags inputFlag = ImGuiInputTextFlags_EnterReturnsTrue;
    inputText.resize(64);

    bool isConfirm = ImGui::InputText("Component ID", inputText.data(), 64, inputFlag);
    if (isConfirm)
    {
        isFoundComponentData_ = INiGuiDebug::GetComponentData(inputText, foundComponentData_);
        isSearchComponentData_ = true;
    }

    ImGui::SeparatorText("Result");
    if (isFoundComponentData_)
    {
        ImGui::Text("Component Data");
        ImGui::Text("ID : %s", foundComponentData_.id.c_str());
        ImGui::Text("TextureName : %s", foundComponentData_.textureName.c_str());
        ImGui::Text("Color : (%.2f, %.2f, %.2f, %.2f)", foundComponentData_.color.x, foundComponentData_.color.y, foundComponentData_.color.z, foundComponentData_.color.w);
        ImGui::Text("LeftTop : (%.2f, %.2f)", foundComponentData_.leftTop.x, foundComponentData_.leftTop.y);
        ImGui::Text("Size : (%.2f, %.2f)", foundComponentData_.size.x, foundComponentData_.size.y);
    }
    else if (isSearchComponentData_)
    {
        ImGui::TextColored({1.0f, .0f,.0f, 1.0f}, "Component not found");
    }
    else
    {
        ImGui::TextDisabled("Press Enter to search component data");
    }

    /// 検索してからのフレームを計測
    if (isSearchComponentData_)
    {
        ++frameCountPressReturn_;
        if (frameCountPressReturn_ > 300)
        {
            isSearchComponentData_ = false;
            frameCountPressReturn_ = 0;
        }
    }

    ImGui::End();

    #endif // _DEBUG
}

void NiGuiDebug::DisplayMenuBar()
{
    #ifdef _DEBUG

    bool isOpen = ImGui::BeginMenuBar();
    if (!isOpen)
    {
        return;
    }

    if (ImGui::BeginMenu("Window"))
    {
        ImGui::MenuItem("Component data", nullptr, &isDisplayComponentDataWindow_);
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();

    #endif // _DEBUG
}
