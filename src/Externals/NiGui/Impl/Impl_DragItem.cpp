#include "../NiGui.h"

std::string NiGui::DragItemArea(const std::string& _id, const std::string& _textureName, const NiVec4& _color, const NiVec2& _position, const NiVec2& _size, const NiGui_StandardPoint _anchor, const NiGui_StandardPoint _pivot)
{
    NiGui_Transform2dEx transformEx = {};
    transformEx.position = _position;
    transformEx.size = _size;
    NiGui_Transform2dEx transformExForDraw = {};

    ComputeRect(transformEx, transformExForDraw, _anchor, _pivot);

    /// =============
    /// 当たり判定と挙動

    NiGui_InputState istate = {};

    /// 当たり判定
    JudgeClickRect(transformEx.position, transformEx.size, istate);

    /// 挙動
    std::string result = DragItemAreaBehavior(_id, istate.isHover, istate.isTrigger, transformEx, _position);

    auto& areaData = state_.buffer.drawData.dragItemArea[_id];
    areaData.id = _id;
    areaData.textureName = _textureName;
    areaData.color = _color;
    areaData.leftTop = transformEx.position;
    areaData.texLeftTop = transformEx.position;
    areaData.size = _size;
    areaData.texSize = _size;
    areaData.zOrder = state_.buffer.currentZOrder++;

    return result;
}

std::string NiGui::DragItemArea(const NiGui_Arg_DragItemArea& _setting)
{
    return NiGui::DragItemArea(
        _setting.id,
        _setting.textureName,
        _setting.color,
        _setting.position,
        _setting.size,
        _setting.anchor,
        _setting.pivot
    );
}

std::string NiGui::DragItemAreaBehavior(const std::string& _id, bool _isHover, bool _isTrigger, NiGui_Transform2dEx& _transform, const NiVec2& _leftTop)
{
    SetComponentId({ .isHover = _isHover, .isTrigger = _isTrigger }, _id, "DragItemArea");

    auto item = state_.buffer.areaToItem.find(_id);
    if (item != state_.buffer.areaToItem.end())
    {
        return item->second;
    }
    else
    {
        return {};
    }

}

std::string NiGui::DragItemBehavior(const std::string& _id, const NiGui_InputState& _inputState, NiGui_Transform2dEx& _transform, const NiVec2& _originLeftTop)
{
    std::string preHoverId = state_.componentID.hover;
    std::string preTypeId = state_.componentID.typeHover;
    std::string result = {};

    SetComponentId(_inputState, _id, "DragItem");

    /// offsetの更新
    auto& offset = dragItemOffset_[_id];
    OffsetUpdate(_id, _originLeftTop, _transform, offset);

    /// areaToItemにセットされていたら座標をエリア内に変更
    for (auto& areaToItem : state_.buffer.areaToItem)
    {
        if (_inputState.isHover) break;
        if (areaToItem.second == _id)
        {
            const auto& area = state_.buffer.drawData.dragItemArea[areaToItem.first];
            NiVec2 diff = area.size - _transform.size;
            _transform.position = area.leftTop + diff * 0.5f;
            offset = _transform.position - _originLeftTop;
            break;
        }
    }

    if (_inputState.isRelease && state_.componentID.active == _id)
    {
        for (auto& areaToItem : state_.buffer.areaToItem)
        {
            if (areaToItem.second == _id)
            {
                areaToItem.second = {};
            }
        }
        if (preTypeId == "DragItemArea" && (state_.buffer.areaToItem[preHoverId].empty() || state_.buffer.areaToItem[preHoverId] == _id))
        {
            const auto& area = state_.buffer.drawData.dragItemArea[preHoverId];
            NiVec2 diff = area.size - _transform.size;
            _transform.position = area.leftTop + diff * 0.5f;
            offset = _transform.position - _originLeftTop;
            state_.buffer.areaToItem[preHoverId] = _id;
        }
        else if (preTypeId != "DragItemArea")
        {
            _transform.position = _originLeftTop;
            offset = {};
        }
        else
        {
            _transform.position = _originLeftTop;
            offset = {};
        }
    }

    for (auto& areaToItem : state_.buffer.areaToItem)
    {
        if (areaToItem.second == _id)
        {
            result = areaToItem.first;
        }
    }

    return result;
}

std::string NiGui::DragItem(const std::string& _id, const std::string& _textureName, const NiVec4& _color, const NiVec2& _position, const NiVec2& _size, const NiGui_StandardPoint _anchor, const NiGui_StandardPoint _pivot)
{
    NiGui_Transform2dEx transformEx = {};
    transformEx.position = _position;
    transformEx.size = _size;

    NiGui_Transform2dEx transformExForDraw = {};

    ComputeRect(transformEx, transformExForDraw, _anchor, _pivot);

    NiVec2 originLeftTop = transformEx.position;

    // Clamp
    transformEx.position += dragItemOffset_[_id];
    ClampRect(transformEx.position, transformEx.size, transformEx.parentPos, transformEx.parentSize);

    /// =============
    /// 当たり判定と挙動

    NiGui_InputState istate = {};

    /// 当たり判定
    JudgeClickRect(transformEx.position, transformEx.size, istate);

    /// 挙動
    std::string result = DragItemBehavior(_id, istate, transformEx, originLeftTop);

    /// データの更新
    auto& itemData = state_.buffer.drawData.dragItem[_id];
    itemData.id = _id;
    itemData.textureName = _textureName;
    itemData.color = _color;
    itemData.leftTop = transformEx.position;
    itemData.texLeftTop = transformEx.position;
    itemData.size = transformEx.size;
    itemData.texSize = transformEx.size;
    itemData.zOrder = state_.buffer.currentZOrder++;

    return result;
}

std::string NiGui::DragItem(const NiGui_Arg_DragItem& _setting)
{
    return NiGui::DragItem(
        _setting.id,
        _setting.textureName,
        _setting.color,
        _setting.position,
        _setting.size,
        _setting.anchor,
        _setting.pivot
    );
}

void NiGui::ResetItemToArea()
{
    state_.buffer.areaToItem.clear();
}

void NiGui::ResetDragItemOffsets()
{
    dragItemOffset_.clear();
}

void NiGui::SetItemToArea(const std::string& _itemID, const std::string& _areaID)
{
    state_.buffer.areaToItem[_areaID] = _itemID;
}

void NiGui::DragItemAreaDataEnqueue()
{
    for (auto& dragItemAreaData : state_.buffer.drawData.dragItemArea)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&dragItemAreaData.second);
    }
}

void NiGui::DragItemDataEnqueue()
{
    for (auto& dragItemData : state_.buffer.drawData.dragItem)
    {
        // 描画クラスにデータを追加
        drawer_->EnqueueDrawInfo(&dragItemData.second);
    }
}
