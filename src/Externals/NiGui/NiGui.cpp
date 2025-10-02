#include "./NiGui.h"

#include <stdexcept> // runtime_error
#include <cassert>
#include <map>
#include <set>
#include <algorithm>

NiGui_Input         NiGui::input_ = NiGui_Input();
NiVec2              NiGui::leftTop_ = { 0, 0 };
NiVec2              NiGui::size_ = { 0, 0 };
INiGuiDrawer* NiGui::drawer_ = nullptr;

NiGuiIO             NiGui::io_ = NiGuiIO();
NiGuiCoreState      NiGui::state_ = NiGuiCoreState();
INiGuiDebug* NiGui::debug_ = nullptr;
NiGuiSetting        NiGui::setting_ = NiGuiSetting();
NiGuiStyle          NiGui::style_ = NiGuiStyle();

const NiVec4        NiGui::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
const NiVec4        NiGui::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
const NiVec4        NiGui::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const NiVec4        NiGui::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const NiVec4        NiGui::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const NiVec4        NiGui::YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
const NiVec4        NiGui::CIAN = { 0.0f, 1.0f, 1.0f, 1.0f };
const NiVec4        NiGui::MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };

std::unordered_map<std::string, NiVec2> NiGui::divOffset_ = std::unordered_map<std::string, NiVec2>();
std::unordered_map<std::string, NiVec2> NiGui::dragItemOffset_ = std::unordered_map<std::string, NiVec2>();



void NiGui::Initialize(const NiVec2& _size, const NiVec2& _leftTop)
{
    state_.valid.isInitialized = true;
    leftTop_ = _leftTop;
    size_ = _size;

    input_.Initialize();

    setting_.deleteElementThreshold = 35;

    style_.windowPadding = { 10, 10 };
    style_.itemSpacing = { 10, 10 };
    style_.divPadding = { 10, 10 };
    style_.color.backGround = { 0.0f, 0.0f, 0.0f, 0.7f };


    return;
}

void NiGui::BeginFrame()
{
    CheckValid_BeginFrame();

    // 入力データ更新
    input_.Update();

    // 入力データのコピー
    CopyInputData();

    // 確認用フラグを立てる
    state_.valid.isBeginFrame = true;

    // ウィンドウ比率の計算
    if (io_.windowInfo.clientSize != 0.0f)
    {
        io_.windowInfo.windowScale = size_.x / io_.windowInfo.clientSize.x;
    }
    else
    {
        io_.windowInfo.windowScale = 1.0f;
    }

    return;
}

void NiGui::DrawUI()
{
    // 描画処理に必要なデータの確認
    CheckValid_DrawUI();

    // コンポーネントの確定処理
    PostProcessComponents();

    // コンポーネントの描画データを追加
    ButtonDataEnqueue();
    DivDataEnqueue();
    DragItemAreaDataEnqueue();
    DragItemDataEnqueue();

    // 描画前処理
    drawer_->DrawSetting();

    // 描画
    drawer_->Draw();

    // 描画後処理
    drawer_->PostDraw();

    // 前フレームのデータとして保存
    SavePreData();

    // データのクリア
    ClearData();

    // 確認用フラグを倒す
    state_.valid.isBeginFrame = false;
}

void NiGui::NiGui_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    input_.WndProcHandler(hWnd, msg, wParam, lParam);
    return;
}

void NiGui::CheckValid_BeginFrame()
{
    if (!state_.valid.isInitialized)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if (state_.valid.isBeginFrame)
    {
        throw std::runtime_error("BeginFrameが連続で呼び出されています。");
    }
}

void NiGui::CheckValid_DrawUI()
{
    if (!state_.valid.isInitialized)
    {
        throw std::runtime_error("UIクラスが初期化されていません。");
    }

    if (!state_.valid.isBeginFrame)
    {
        throw std::runtime_error("BeginFrameが呼び出されていません。");
    }

    if (drawer_ == nullptr)
    {
        throw std::runtime_error("Drawerがセットされていません。");
    }

    if (state_.valid.nestCount != 0)
    {
        throw std::runtime_error("ネストが不正です。ネスト開始関数とネスト終了関数の数が正しく呼び出されていますか？");
    }
}

void NiGui::JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, NiGui_InputState& _input)
{
    NiVec2 leftTop = _leftTop + leftTop_;

    bool& hover = _input.isHover;
    bool& trigger = _input.isTrigger;
    bool& release = _input.isRelease;

    if (leftTop.x <= input_.GetMousePos().x && input_.GetMousePos().x <= leftTop.x + _size.x &&
        leftTop.y <= input_.GetMousePos().y && input_.GetMousePos().y <= leftTop.y + _size.y)
    {
        hover = true;
        if (input_.TriggerLeft())
        {
            trigger = true;
        }
        else
        {
            trigger = false;
        }
    }
    else
    {
        hover = false;
        trigger = false;
    }

    if (input_.ReleaseLeft())
    {
        release = true;
    }
    else
    {
        release = false;
    }
}

NiVec2 NiGui::ComputeStandardPoint(NiGui_StandardPoint _stdpoint)
{
    NiVec2 result = {};

    switch (_stdpoint)
    {
    case NiGui_StandardPoint::LeftTop:
        result.x = 0.0f;
        result.y = 0.0f;
        break;

    case NiGui_StandardPoint::CenterTop:
        result.x = 0.5f;
        result.y = 0.0f;
        break;

    case NiGui_StandardPoint::RightTop:
        result.x = 1.0f;
        result.y = 0.0f;
        break;

    case NiGui_StandardPoint::LeftCenter:
        result.x = 0.0f;
        result.y = 0.5f;
        break;

    case NiGui_StandardPoint::Center:
        result.x = 0.5f;
        result.y = 0.5f;
        break;

    case NiGui_StandardPoint::RightCenter:
        result.x = 1.0f;
        result.y = 0.5f;
        break;

    case NiGui_StandardPoint::LeftBottom:
        result.x = 0.0f;
        result.y = 1.0f;
        break;

    case NiGui_StandardPoint::CenterBottom:
        result.x = 0.5f;
        result.y = 1.0f;
        break;

    case NiGui_StandardPoint::RightBottom:
        result.x = 1.0f;
        result.y = 1.0f;
        break;

    default:
        break;
    }

    return result;
}

NiVec2 NiGui::ComputeLeftTop(const NiVec2& _position, const NiVec2& _size, const NiVec2& _parentSize, NiGui_StandardPoint _anchor, NiGui_StandardPoint _pivot)
{
    NiVec2 result = {};

    NiVec2 normalAnchor = ComputeStandardPoint(_anchor);
    NiVec2 normalPivot = ComputeStandardPoint(_pivot);

    result = _position + normalAnchor * _parentSize;
    result -= _size * normalPivot;
    return result;
}

void NiGui::ClearData()
{
    auto& drawData = state_.buffer.drawData;

    auto& cid = state_.componentID;
    auto& buffer = state_.buffer;

    cid.hover = {};
    cid.typeHover = {};

    buffer.currentRegion = nullptr;
    buffer.currentZOrder = 0;

    if (buffer.areaToItem.size() > setting_.deleteElementThreshold)
    {
        std::vector<std::string> deleteKeys;
        for (auto& areaToItem : buffer.areaToItem)
        {
            if (areaToItem.second.empty())
            {
                deleteKeys.push_back(areaToItem.first);
            }
            else if (drawData.dragItemArea.find(areaToItem.first.c_str()) == drawData.dragItemArea.end())
            {
                deleteKeys.push_back(areaToItem.first);
            }
            else if (drawData.dragItem.find(areaToItem.second.c_str()) == drawData.dragItem.end())
            {
                deleteKeys.push_back(areaToItem.first);
            }
        }
        for (auto& key : deleteKeys)
        {
            buffer.areaToItem.erase(key);
        }
        if (buffer.areaToItem.size() > setting_.deleteElementThreshold) setting_.deleteElementThreshold *= 2;
    }

    drawData.button.clear();
    drawData.div.clear();
    drawData.dragItem.clear();
    drawData.dragItemArea.clear();

    return;
}

void NiGui::SavePreData()
{
    // 前フレームのホバー中のコンポーネントIDを保存
    state_.componentID.preHover = state_.componentID.hover;
    state_.componentID.preActive = state_.componentID.active;
}

void NiGui::CopyInputData()
{
    io_.input.isLeftPre = io_.input.isLeft;
    io_.input.isRightPre = io_.input.isRight;
    io_.input.isMiddlePre = io_.input.isMiddle;

    io_.input.isLeft = input_.PressLeft();
    io_.input.isRight = input_.PressRight();
    io_.input.isMiddle = input_.PressMiddle();

    io_.input.cursorPos = input_.GetMousePos();
    io_.input.triggeredPos = input_.GetTriggeredPos();
    io_.input.differencePos = input_.GetDifferencePos();
}

void NiGui::ClampRect(NiVec2& _leftTop, const NiVec2& _size, const NiVec2& _parentPos, const NiVec2& _parentSize)
{
    if (_leftTop.x < _parentPos.x) _leftTop.x = _parentPos.x;
    if (_leftTop.y < _parentPos.y) _leftTop.y = _parentPos.y;
    if (_leftTop.x + _size.x > _parentPos.x + _parentSize.x) _leftTop.x = _parentPos.x + _parentSize.x - _size.x;
    if (_leftTop.y + _size.y > _parentPos.y + _parentSize.y) _leftTop.y = _parentPos.y + _parentSize.y - _size.y;
}

void NiGui::OffsetUpdate(
    const std::string& _id,
    const NiVec2& _originLeftTop,
    const NiGui_Transform2dEx& _transform,
    NiVec2& _offset)
{
    if (state_.componentID.active == _id)
    {
        if (io_.windowInfo.windowScale != 0.0f)
        {
            _offset += io_.input.differencePos / io_.windowInfo.windowScale;
        }
    }

    if (input_.ReleaseLeft() && state_.componentID.active == _id)
    {
        if (_transform.position.x == _transform.parentPos.x) _offset.x = _transform.parentPos.x - _originLeftTop.x;
        if (_transform.position.y == _transform.parentPos.y) _offset.y = _transform.parentPos.y - _originLeftTop.y;
        if (_transform.position.x + _transform.size.x == _transform.parentPos.x + _transform.parentSize.x) _offset.x = _transform.parentPos.x + _transform.parentSize.x - _originLeftTop.x - _transform.size.x;
        if (_transform.position.y + _transform.size.y == _transform.parentPos.y + _transform.parentSize.y) _offset.y = _transform.parentPos.y + _transform.parentSize.y - _originLeftTop.y - _transform.size.y;
    }
}

void NiGui::SetComponentId(const NiGui_InputState& _inputState, const std::string& _id, const std::string& _type)
{
    if (_id.empty()) assert(false && "unexpected");

    if (_inputState.isHover)
    {
        state_.componentID.hover = _id;
        state_.componentID.typeHover = _type;
    }
    if (_inputState.isTrigger)
    {
        state_.componentID.active = _id;
        state_.componentID.typeActive = _type;
    }
}

std::unordered_map<std::string, int> assignMaxZOrder(
    const std::unordered_map<std::string, int>& zOrderMap_,
    const std::string& targetKey)
{
    std::unordered_map<std::string, int> newMap_;
    std::map<int, std::string> sortedMap;

    int num_pick = zOrderMap_.at(targetKey);
    int num_temp = 0;
    int num_next = 0;

    bool isNextForPick = false;
    for (const auto& [key, value] : zOrderMap_)
    {
        sortedMap[value] = key;

        if (key == targetKey)
        {
            num_pick = value;
            num_temp = value;
            isNextForPick = true;
            continue;
        }

        if (isNextForPick) 
        {
            num_next = value;
            isNextForPick = false;
        }
    }

    std::map<int, std::string> newMap = sortedMap;
    while (true)
    {
        auto it = sortedMap.find(num_next);
        bool isBreak = false;
        if (it == sortedMap.end()) isBreak = true;
        else ++it;

        int temp = 0;
        if (it != sortedMap.end()) temp = it->first;

        newMap[num_temp] = sortedMap[num_next]; // 10

        if (isBreak)
        {
            newMap[num_temp] = targetKey;
            break;
        }

        num_temp = num_next; // 13
        num_next = temp; // 15
    }

    for (auto& [key, value] : newMap)
    {
        newMap_[value] = key;
    }

    return newMap_;
}

void NiGui::PostProcessComponents()
{
    auto& componentID = state_.componentID;
    auto& componentTime = state_.time;
    auto& drawData = state_.buffer.drawData;

    if (!io_.input.isLeft && io_.input.isLeftPre)
    {
        componentID.active = {};
        componentID.typeActive = {};
    }

    /// =========
    /// Active
    if (!componentID.active.empty())
    {
        /// アクティブIDが変更されたら
        if (componentID.active != componentID.preActive)
        {
            // アクティブ時間をリセット
            componentTime.active = 0;
        }
        ++componentTime.active;
    }
    else
    {
        componentTime.active = 0;
    }

    if (componentID.typeActive == "DragItem" && !componentID.active.empty())
    {
        std::unordered_map<std::string, int> zmap = {};
        for (const auto& [key, value] : drawData.dragItem)
        {
            zmap[key] = value.zOrder;
        }

        auto newZMap = assignMaxZOrder(zmap, componentID.active);

        for (const auto& [key, value] : newZMap)
        {
            if (drawData.dragItem[key].zOrder != value)
            {
                drawData.dragItem[key].zOrder = value;
            }
        }
    }

    /// ========
    /// Hover
    if (!componentID.hover.empty())
    {
        if (componentID.hover != componentID.preHover)
        {
            componentTime.hover = 0;
        }
        ++componentTime.hover;
    }
    else
    {
        componentTime.hover = 0;
    }

    /// ========
    /// Play SE
    PlaySE("Button", io_.audioHnd.buttonHover, io_.audioHnd.buttonConfirm, io_.audioHandler.buttonHover, io_.audioHandler.buttonConfirm);
}

void NiGui::PlaySE(const std::string& _type, uint32_t _hoverSE, uint32_t _confirmSE, void* _hoverHandler, void* _confirmHandler)
{
    auto& componentID = state_.componentID;
    auto& componentTime = state_.time;

    if (componentID.typeHover != _type) return;

    if (componentTime.active == 0 && componentID.preActive == componentID.preHover && !componentID.preActive.empty())
    {
        if (_confirmSE != -1)
        {
            drawer_->PlayAudio(_confirmSE);
        }
        else
        {
            drawer_->PlayAudio(_confirmHandler);
        }
    }
    if (componentTime.hover == 1)
    {
        if (_hoverSE != -1)
        {
            drawer_->PlayAudio(_hoverSE);
        }
        else
        {
            drawer_->PlayAudio(_hoverHandler);
        }
    }
}
