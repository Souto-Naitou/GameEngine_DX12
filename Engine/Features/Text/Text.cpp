#include "Text.h"
#include <Utility/ConvertString/ConvertString.h>
#include <cassert>

#include <DebugTools/DebugManager/DebugManager.h>
#include <imgui.h>

#ifdef _DEBUG
#include <Features/Viewport/Viewport.h>
#endif

void Text::Initialize()
{
    pDirectX12_ = DirectX12::GetInstance();
    pTextSystem_ = TextSystem::GetInstance();
    dwriteFactory_ = pTextSystem_->GetDWriteFactory();
    d2dDeviceContext_ = pTextSystem_->GetD2D1DeviceContext();
    pViewport_ = pTextSystem_->GetViewport();

#ifdef _DEBUG
    DebugManager::GetInstance()->SetComponent("Text", name_, BINDCOMPONENT(Text, DebugWindow));
#endif // _DEBUG

    fontFamily_ = "Bahnschrift";
}

void Text::Update()
{
    if (isChangedParent_)
    {
        screenPosition_ = pParent_->screenPosition_ + position_;
        isChangedParent_ = false;
    }

    #ifdef _DEBUG
    UpdatePosition();
    #endif // _DEBUG

    if (isChanged_)
    {
        CreateTextLayout();
        isChanged_ = false;
    }
}

void Text::UpdatePosition()
{
    this->SetPosition(position_);
}

void Text::Draw()
{
    const auto solidColorBrush = pTextSystem_->GetColorBrush(keyColor_);
    d2dDeviceContext_->DrawTextLayout(
        D2D1::Point2F(screenPosition_.x, screenPosition_.y),
        textLayout_.Get(),
        solidColorBrush
    );
}

void Text::Finalize()
{
#ifdef _DEBUG
    DebugManager::GetInstance()->DeleteComponent("Text", name_.c_str());
#endif // _DEBUG
}

void Text::SetText(const std::string& _text)
{
    if (text_ == _text) return;

    text_ = _text;
    isChanged_ = true;
}

void Text::SetPosition(const Vector2& _pos)
{
    position_ = _pos;

    if (pParent_)
    {
        Vector2 pivotPos = ComputeStandardPosition(pivot_);
        screenPosition_ = pParent_->screenPosition_ + position_;
        screenPosition_ -= pivotPos * Vector2(metrics_.width, metrics_.height);
    }
    else
    {
        Vector2 anchorPos = ComputeStandardPosition(anchorPoint_);
        Vector2 pivotPos = ComputeStandardPosition(pivot_);


        #ifdef _DEBUG

        auto vpos = pViewport_->GetViewportPos();
        auto vsize = pViewport_->GetViewportSize();
        screenPosition_ = position_ + anchorPos * vsize + vpos;
        screenPosition_ -= pivotPos * Vector2(metrics_.width, metrics_.height);

        #else

        auto vp = pDirectX12_->GetViewport();
        screenPosition_ = position_ + anchorPos * Vector2(vp.Width, vp.Height) + Vector2(vp.TopLeftX, vp.TopLeftY);
        screenPosition_ -= pivotPos * Vector2(metrics_.width, metrics_.height);

        #endif
    }

    if (child_)
    {
        child_->UpdatePosition();
    }

    callCount_UpdatePosition_++;
}

void Text::SetParent(Text* _parent)
{
    pParent_ = _parent; 
    pParent_->SetChild(this);
    isChangedParent_ = true;
}

void Text::CreateTextLayout()
{
    std::wstring w_text = ConvertString(text_);

    textFormat_ = pTextSystem_->GetTextFormat(fontFamily_, fontSize_);

    HRESULT hr = dwriteFactory_->CreateTextLayout(
        w_text.c_str(),
        static_cast<uint32_t>(w_text.size()),
        textFormat_,
        maxsize_.x,
        maxsize_.y,
        &textLayout_
    );

    textLayout_->GetMetrics(&metrics_);

    UpdatePosition();

    assert(SUCCEEDED(hr) && "テキストレイアウトの生成に失敗");

    return;
}

void Text::DebugWindow()
{
#ifdef _DEBUG

    char buffer[256] = { 0 };
    strcpy_s(buffer, text_.c_str());

    ImGui::Text("ScreenPosition : %d, %d", static_cast<int>(screenPosition_.x), static_cast<int>(screenPosition_.y));

    if (ImGui::DragFloat2("Position", &position_.x))
    {
        this->SetPosition(position_);
    }

    if (ImGui::Combo("AnchorPoint", reinterpret_cast<int*>(&anchorPoint_), anchors_, 9))
    {
        this->SetPosition(position_);
    }
    if (ImGui::Combo("Pivot", reinterpret_cast<int*>(&pivot_), anchors_, 9))
    {
        this->SetPosition(position_);
    }

    ImGui::Checkbox("Visible Size", &isVisibleRange_);
    ImGui::DragFloat2("MaxSize", &maxsize_.x);

    if (ImGui::InputText("Text", buffer, IM_ARRAYSIZE(buffer)))
    {
        text_ = buffer;
        isChanged_ = true;
    }

    int fontSize = static_cast<int>(fontSize_);

    if (ImGui::InputInt("Font Size", &fontSize))
    {
        fontSize_ = static_cast<float>(fontSize);
        isChanged_ = true;
    }

    strcpy_s(buffer, fontFamily_.c_str());
    if (ImGui::InputText("Font Family", buffer, IM_ARRAYSIZE(buffer)))
    {
        fontFamily_ = buffer;
    }

    ImGui::Text("座標が更新された回数 : %d", callCount_UpdatePosition_);

#endif // _DEBUG
}

Vector2 Text::ComputeStandardPosition(TextStandardPoint _stdpoint)
{
    Vector2 result = {};

    switch (_stdpoint)
    {
    case TextStandardPoint::TopLeft:
        result.x = 0.0f;
        result.y = 0.0f;
        break;

    case TextStandardPoint::TopCenter:
        result.x = 0.5f;
        result.y = 0.0f;
        break;

    case TextStandardPoint::TopRight:
        result.x = 1.0f;
        result.y = 0.0f;
        break;

    case TextStandardPoint::CenterLeft:
        result.x = 0.0f;
        result.y = 0.5f;
        break;

    case TextStandardPoint::Center:
        result.x = 0.5f;
        result.y = 0.5f;
        break;

    case TextStandardPoint::CenterRight:
        result.x = 1.0f;
        result.y = 0.5f;
        break;

    case TextStandardPoint::BottomLeft:
        result.x = 0.0f;
        result.y = 1.0f;
        break;

    case TextStandardPoint::BottomCenter:
        result.x = 0.5f;
        result.y = 1.0f;
        break;

    case TextStandardPoint::BottomRight:
        result.x = 1.0f;
        result.y = 1.0f;
        break;

    default:
        break;
    }

    return result;
}
