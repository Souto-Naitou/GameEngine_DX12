#include "../NiGui.h"

NiGui_ButtonState NiGui::Button(
    const std::string& _id,
    const std::string& _textureName,
    const NiVec4& _color,
    const NiVec2& _position,
    const NiVec2& _size,
    const NiVec2& _texSize,
    NiGui_StandardPoint _anchor,
    NiGui_StandardPoint _pivot)
{
    auto& buttonImage = state_.buffer.drawData.button[_id];
    NiGui_InputState istate = {};
    bool isHeld = false;

    // 描画用座標
    NiVec2 leftTopForDraw = {};
    // 当たり判定用座標
    NiVec2 leftTopForJudge = {};

    NiVec2 posInRegion = _position;
    NiVec2 parentSizeForDraw = io_.windowInfo.clientSize;
    NiVec2 parentSizeForJudge = size_;
    float gScale = io_.windowInfo.windowScale;

    NiVec2 texSize = _texSize;
    if (_texSize.Length() == 0.0f)
    {
        texSize = _size;
    }

    if (state_.buffer.currentRegion != nullptr)
    {
        posInRegion = _position + state_.buffer.currentRegion->leftTop;
        parentSizeForJudge = state_.buffer.currentRegion->size * gScale;
        parentSizeForDraw = state_.buffer.currentRegion->size;
    }

    leftTopForJudge = ComputeLeftTop(posInRegion * gScale, _size * gScale, parentSizeForJudge, _anchor, _pivot);
    leftTopForDraw = ComputeLeftTop(posInRegion, _size, parentSizeForDraw, _anchor, _pivot);
    auto texLeftTop = ComputeLeftTop(posInRegion, texSize, parentSizeForDraw, _anchor, _pivot);

    // 当たり判定
    JudgeClickRect(leftTopForJudge, _size * gScale, istate);

    // ボタンの挙動
    bool onButton = ButtonBehavior(_id, istate, isHeld);

    /// ボタンのデータを更新
    buttonImage.id = _id;
    buttonImage.textureName = _textureName;
    buttonImage.color = _color;
    buttonImage.leftTop = leftTopForDraw;
    buttonImage.texLeftTop = texLeftTop;
    buttonImage.size = _size;
    buttonImage.texSize = texSize;
    buttonImage.zOrder = state_.buffer.currentZOrder++;

    NiGui_ButtonState result = NiGui_ButtonState::None;
    if (onButton)
    {
        result = NiGui_ButtonState::Confirm;
    }
    else if (istate.isHover)
    {
        result = NiGui_ButtonState::Hover;
    }

    return result;
}

NiGui_ButtonState NiGui::Button(const NiGui_Arg_Button& _setting)
{
    return NiGui::Button(
        _setting.id,
        _setting.textureName,
        _setting.color,
        _setting.position,
        _setting.size,
        _setting.texSize,
        _setting.anchor,
        _setting.pivot
    );
}

bool NiGui::ButtonBehavior(const std::string& _id, const NiGui_InputState& _inputState, bool& _out_held)
{
    SetComponentId(_inputState, _id, "Button");


    if(state_.componentID.active == _id)
    {
        _out_held = true;

        if (_inputState.isRelease && _inputState.isHover)
        {
            return true;
        }
    }

    return false;
}

void NiGui::ButtonDataEnqueue()
{
    for(auto& buttonImage : state_.buffer.drawData.button)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&buttonImage.second);
    }
}