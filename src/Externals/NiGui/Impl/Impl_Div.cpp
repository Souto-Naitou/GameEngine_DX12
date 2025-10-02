#include "../NiGui.h"

#include <stdexcept> // runtime_error


bool NiGui::BeginDiv(const std::string& _id, const std::string& _textureName, const NiVec4& _color, const NiVec2& _position, const NiVec2& _size, const NiGui_StandardPoint _anchor, const NiGui_StandardPoint _pivot)
{
    NiGui_Transform2dEx transformEx = {};
    transformEx.position = _position;
    transformEx.size = _size;

    NiGui_Transform2dEx transformExForJudge = {};

    ComputeRect(transformEx, transformExForJudge, _anchor, _pivot);

    /// =============
    /// 当たり判定と挙動

    NiGui_InputState input = {};

    /// 当たり判定
    JudgeClickRect(transformEx.position, transformEx.size, input);

    /// 挙動
    DivBehavior(_id, input.isHover, input.isTrigger);

    /// データの更新
    auto& divData = state_.buffer.drawData.div[_id];
    divData.id = _id;
    divData.textureName = _textureName;
    divData.color = _color;
    divData.leftTop = transformEx.position;
    divData.texLeftTop = transformEx.position;
    divData.size = transformEx.size;
    divData.texSize = transformEx.size;
    divData.zOrder = state_.buffer.currentZOrder++;
    divData.parent = state_.buffer.currentRegion;

    /// 現在のリージョンを更新
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);


    return true;
}

bool NiGui::BeginDiv(const NiGui_Arg_Div& _setting)
{
    return NiGui::BeginDiv(
        _setting.id,
        _setting.textureName,
        _setting.color,
        _setting.position,
        _setting.size,
        _setting.anchor,
        _setting.pivot
    );
}

bool NiGui::BeginDivMovable(const std::string& _id, const std::string& _textureName, const NiVec4& _color, const NiVec2& _position, const NiVec2& _size, const NiGui_StandardPoint _anchor, const NiGui_StandardPoint _pivot)
{
    float gScale = io_.windowInfo.windowScale;

    NiGui_Transform2dEx transformExForDraw = {};
    transformExForDraw.position = _position;
    transformExForDraw.size = _size;

    // 判定用
    NiGui_Transform2dEx transformExForJudge = {};
    transformExForJudge.position = _position * gScale;
    transformExForJudge.size = _size * gScale;

    ComputeRect(transformExForDraw, transformExForJudge, _anchor, _pivot);

    NiVec2 originLeftTop = transformExForDraw.position;

    /// =============
    /// 当たり判定と挙動
    
    NiGui_InputState istate = {};

    transformExForDraw.position += divOffset_[_id];
    transformExForJudge.position += divOffset_[_id] * gScale;
    ClampRect(transformExForDraw.position, _size, transformExForDraw.parentPos, transformExForDraw.parentSize);

    /// 当たり判定
    JudgeClickRect(transformExForJudge.position, transformExForJudge.size, istate);

    /// 挙動
    auto& regionDiff = divOffset_[_id];

    DivBehavior(_id, istate.isHover, istate.isTrigger);

    OffsetUpdate(_id, originLeftTop, transformExForDraw, regionDiff);

    /// データの更新
    auto& divData = state_.buffer.drawData.div[_id];
    divData.id = _id;
    divData.textureName = _textureName;
    divData.color = _color;
    divData.leftTop = transformExForDraw.position;
    divData.texLeftTop = transformExForDraw.position;
    divData.size = transformExForDraw.size;
    divData.texSize = transformExForDraw.size;
    divData.zOrder = state_.buffer.currentZOrder++;
    divData.parent = state_.buffer.currentRegion;

    /// 現在のリージョンをこのDivに更新
    state_.buffer.currentRegion = dynamic_cast<BaseRegionData*>(&divData);

    return true;
}

bool NiGui::BeginDivMovable(const NiGui_Arg_Div& _setting)
{
    return NiGui::BeginDivMovable(
        _setting.id,
        _setting.textureName,
        _setting.color,
        _setting.position,
        _setting.size,
        _setting.anchor,
        _setting.pivot
    );
}

void NiGui::EndDiv()
{
    if (state_.valid.nestCount == 0)
    {
        throw std::runtime_error("ネスト終了関数の呼び出し過多です。");
    }

    state_.valid.nestCount--;

    state_.buffer.currentRegion = state_.buffer.currentRegion->parent;
}

void NiGui::DivBehavior(const std::string& _id, bool _isHover, bool _isTrigger)
{
    // ネストカウントを増やす
    state_.valid.nestCount++;

    SetComponentId({ .isHover = _isHover, .isTrigger = _isTrigger }, _id, "Div");
}

void NiGui::DivDataEnqueue()
{
    for (auto& divData : state_.buffer.drawData.div)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&divData.second);
    }
}

void NiGui::ComputeRect(NiGui_Transform2dEx& _transformForDraw, NiGui_Transform2dEx& _transformForJudge, const NiGui_StandardPoint _anchor, const NiGui_StandardPoint _pivot)
{
    /// 親の座標とサイズを取得
    if (state_.buffer.currentRegion == nullptr)
    {
        _transformForDraw.parentSize = io_.windowInfo.clientSize;
        _transformForJudge.parentSize = size_;
    }
    else
    {
        _transformForDraw.parentPos = state_.buffer.currentRegion->leftTop;
        _transformForDraw.parentSize = state_.buffer.currentRegion->size;
        _transformForJudge.parentPos = state_.buffer.currentRegion->leftTop * io_.windowInfo.windowScale;
        _transformForJudge.parentSize = state_.buffer.currentRegion->size * io_.windowInfo.windowScale;
    }

    /// 親の座標を考慮した座標を計算
    if (state_.buffer.currentRegion != nullptr)
    {
        _transformForDraw.position += state_.buffer.currentRegion->leftTop;
        _transformForJudge.position += state_.buffer.currentRegion->leftTop * io_.windowInfo.windowScale;
    }

    /// ピボットとアンカーを考慮した座標を計算
    _transformForDraw.position = ComputeLeftTop(_transformForDraw.position, _transformForDraw.size, _transformForDraw.parentSize, _anchor, _pivot);
    _transformForJudge.position = ComputeLeftTop(_transformForJudge.position, _transformForJudge.size, _transformForJudge.parentSize, _anchor, _pivot);
}
