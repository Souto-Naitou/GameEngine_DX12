#include "FreeLookEye.h"
#include <imgui.h>
#include <list>
#include <string>
#include <utility>

FreeLookEye::FreeLookEye()
{
    pInput_ = Input::GetInstance();
}

FreeLookEye::~FreeLookEye()
{
}

void FreeLookEye::Update()
{
    prevCursorPos_ = currCursorPos_;
    GetCursorPos(&currCursorPos_);

    if(pInput_->PushMouse(Input::MouseNum::Right))
    {
        this->CatchMoveCommands();
        this->CatchRotateCommands();
    }

    GameEye::Update();
}

void FreeLookEye::CatchMoveCommands()
{
    Vector3 rotate = GetTransform().rotate;
    Quaternion yaw = Quaternion::RotateAxisAngleQuaternion({ 0.0f, 1.0f, 0.0f }, rotate.y);
    Quaternion pitch = Quaternion::RotateAxisAngleQuaternion({ 1.0f, 0.0f, 0.0f }, rotate.x);

    rotate_ = yaw * pitch;

    Vector3 forward = FMath::RotateVector({ 0.0f, 0.0f, 1.0f }, rotate_);
    Vector3 right = FMath::RotateVector({ 1.0f, 0.0f, 0.0f }, rotate_);

    moveSpeed_ += static_cast<float>(pInput_->GetWheelDelta()) * 0.0001f;

    if (moveSpeed_ > 2.0f) moveSpeed_ = 2.0f;
    else if (moveSpeed_ < -2.0f) moveSpeed_ = -2.0f;

    /// 向いている方向に移動
    if(pInput_->PushKeyC('W'))
    {
        SetTranslate(GetTransform().translate + forward * moveSpeed_);
    }
    if(pInput_->PushKeyC('S'))
    {
        SetTranslate(GetTransform().translate - forward * moveSpeed_);
    }
    if(pInput_->PushKeyC('A'))
    {
        SetTranslate(GetTransform().translate - right * moveSpeed_);
    }
    if(pInput_->PushKeyC('D'))
    {
        SetTranslate(GetTransform().translate + right * moveSpeed_);
    }
    if(pInput_->PushKeyC('Q'))
    {
        SetTranslate(GetTransform().translate + Vector3(0.0f, -moveSpeed_, 0.0f));
    }
    if(pInput_->PushKeyC('E'))
    {
        SetTranslate(GetTransform().translate + Vector3(0.0f, moveSpeed_, 0.0f));
    }
}

void FreeLookEye::CatchRotateCommands()
{
    Vector2 diff = { 
        static_cast<float>(currCursorPos_.x - prevCursorPos_.x), 
        static_cast<float>(currCursorPos_.y - prevCursorPos_.y) 
    };

    EulerTransform transform = GetTransform();
    transform.rotate.x += diff.y * 0.003f;
    transform.rotate.y += diff.x * 0.003f;

    SetTransform(transform);
}

void FreeLookEye::ImGui()
{
    GameEye::ImGui();

    static const std::list<std::pair<std::string, std::string>> controlExplainList = 
    {
        { "マウス右ボタン", "カメラ移動の有効化" },
        { "マウスカーソルの移動", "視点移動"},
        { "マウスホイール", "移動速度変更" },
        { "W", "前進" },
        { "S", "後退" },
        { "A", "左移動" },
        { "D", "右移動" },
        { "Q", "下移動" },
        { "E", "上移動" },
    };

    #ifdef _DEBUG
    if (ImGui::CollapsingHeader("操作方法"))
    {
        ImGuiTableFlags flag = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg;
        bool isTableOpen = ImGui::BeginTable("HowToUseTable", 2, flag);
        if (!isTableOpen) return;

        ImGui::TableSetupColumn("操作", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("説明", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (const auto& [key, value] : controlExplainList)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", key.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", value.c_str());
        }
        ImGui::EndTable();
        
    }
    ImGui::Text("MoveSpeed: %.3f", moveSpeed_);
    #endif //_DEBUG
}
