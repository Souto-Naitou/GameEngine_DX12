#pragma once

#include <Core/DirectX12/DirectX12.h>

#include "TextSystem.h"

enum class TextStandardPoint : int
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

class Text
{
public:
    Text() = default;
    ~Text() = default;

    void Initialize();
    void Update();
    void Draw();
    void Finalize();

public: /// Getter
    Vector2 GetSize() const { return { metrics_.width, metrics_.height }; }


public: /// Setter
    void SetName(const std::string& _name) { name_ = _name; }
    void SetText(const std::string& _text);
    void SetAnchorPoint(TextStandardPoint _anchor) { anchorPoint_ = _anchor; UpdatePosition(); }
    void SetPivot(TextStandardPoint _pivot) { pivot_ = _pivot; UpdatePosition(); }
    void SetPosition(const Vector2& _pos);
    void SetMaxSize(const Vector2& _maxsize) { maxsize_ = _maxsize; isChanged_ = true; }
    void SetFontFamily(const std::string& _fontFamily) { fontFamily_ = _fontFamily; isChanged_ = true; }
    void SetFontSize(float _fontSize) { fontSize_ = _fontSize; isChanged_ = true;}
    void SetColorName(const std::string& _key) { keyColor_ = _key; }
    void SetParent(Text* _parent);

    void CreateTextLayout();


private: /// メンバー
    std::string                                 text_                       = {};
    std::string                                 fontFamily_                 = {};
    Vector2                                     screenPosition_             = {};
    Vector2                                     position_                   = {};
    TextStandardPoint                           anchorPoint_                = TextStandardPoint::TopLeft;
    TextStandardPoint                           pivot_                      = TextStandardPoint::TopLeft;
    Vector2                                     maxsize_                    = {};
    float                                       fontSize_                   = 0.0f;
    std::string                                 keyColor_                   = "Black";

    bool                                        isChanged_                  = false;
    bool                                        isChangedParent_            = false;
    bool                                        isVisibleRange_             = false;

    IDWriteTextFormat*                          textFormat_                 = nullptr;

    Text*                                       pParent_                    = nullptr;
    Text*                                       child_                      = nullptr;
    Microsoft::WRL::ComPtr<IDWriteTextLayout>   textLayout_                 = nullptr;
    DWRITE_TEXT_METRICS                         metrics_                    = {};

private: /// 借り物
    IDWriteFactory7*                            dwriteFactory_              = nullptr;
    TextSystem*                                 pTextSystem_                = nullptr;
    ID2D1DeviceContext2*                        d2dDeviceContext_           = nullptr;
    Viewport*                                   pViewport_                  = nullptr;


private: /// デバッグ
    void ImGui();
    std::string                                 name_                       = {};
    const char*                                 anchors_[9]                 = { "TopLeft", "TopCenter", "TopRight", "CenterLeft", "Center", "CenterRight", "BottomLeft", "BottomCenter", "BottomRight" };
    int                                         callCount_UpdatePosition_   = 0;

private:
    Vector2 ComputeStandardPosition(TextStandardPoint _stdpoint);
    void UpdatePosition();
    void SetChild(Text* _child) { child_ = _child; }
};