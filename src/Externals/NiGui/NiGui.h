#pragma once

#include "./Type/NiGui_Type_Component.h" // ComponentTypes
#include "./Type/NiGui_Type_Core.h" // NiGuiCoreState
#include "./Type/NiGui_Type_Various.h"
#include "./Type/NiGui_Type_Argument.h"
#include "./Type/NiGui_Enum.h" // enums
#include "./math/NiVec2.h" // NiVec2
#include "./Input/NiGui_Input.h" // NiUI_Input
#include "./Interface/NiGui_IDrawer.h"
#include "./Interface/NiGui_IDebug.h"

#include <unordered_map> // unordered_map
#include <string> // string
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // HWND, UINT, WPARAM, LPARAM

using NiID = unsigned int;

/// UIクラス
class NiGui
{
public: /// コンストラクタとデストラクタ
    NiGui() = default;
    ~NiGui() = default;


public: /// 一般

    // ルートウィンドウの初期化
    // クライアント領域の左上座標とサイズを指定してください。
    static void Initialize(const NiVec2& _size, const NiVec2& _leftTop = {0, 0});


    // フレームごとの初期化処理
    // UIコンポーネントを追加する前に呼び出してください。
    static void BeginFrame();


    // UIの更新及び描画
    // この関数を呼び出したら、BeginFrame()を呼び出すまでUIコンポーネントの追加は反映されません。
    static void DrawUI();


    // ウィンドウプロシージャハンドラ
    // 注意：ImGuiのハンドラより先に呼び出してください。
    static void NiGui_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


    static void DrawDebug() { debug_->DrawDebugUI(); };

public: /// UIコンポーネントの追加
    // ボタンの追加
    // ボタンのテクスチャ名、左上座標、サイズを指定してください。
    static NiGui_ButtonState Button(
        const std::string& _id,
        const std::string& _textureName,
        const NiVec4& _color,
        const NiVec2& _position,
        const NiVec2& _size,
        const NiVec2& _texSize = {},
        NiGui_StandardPoint _anchor = NiGui_StandardPoint::LeftTop,
        NiGui_StandardPoint _pivot = NiGui_StandardPoint::LeftTop
    );

    static NiGui_ButtonState Button(const NiGui_Arg_Button& _setting);

    // Divの追加
    // textureNameが空の場合はデフォルトの画像が使用されます。
    static bool BeginDiv(
        const std::string& _id,
        const std::string& _textureName,
        const NiVec4& _color,
        const NiVec2& _position,
        const NiVec2& _size,
        NiGui_StandardPoint _anchor = NiGui_StandardPoint::LeftTop,
        NiGui_StandardPoint _pivot = NiGui_StandardPoint::LeftTop
    );

    static bool BeginDiv(const NiGui_Arg_Div& _setting);

    static void EndDiv();

    // 移動可能なDivの追加
    // textureNameが空の場合はデフォルトの画像が使用されます。
    static bool BeginDivMovable(
        const std::string& _id,
        const std::string& _textureName,
        const NiVec4& _color,
        const NiVec2& _position,
        const NiVec2& _size,
        const NiGui_StandardPoint _anchor = NiGui_StandardPoint::LeftTop,
        const NiGui_StandardPoint _pivot = NiGui_StandardPoint::LeftTop
    );

    static bool BeginDivMovable(const NiGui_Arg_Div& _setting);

    static std::string DragItemArea(
        const std::string& _id,
        const std::string& _textureName,
        const NiVec4& _color,
        const NiVec2& _position,
        const NiVec2& _size,
        const NiGui_StandardPoint _anchor = NiGui_StandardPoint::LeftTop,
        const NiGui_StandardPoint _pivot = NiGui_StandardPoint::LeftTop
    );

    static std::string DragItemArea(const NiGui_Arg_DragItemArea& _setting);

    static std::string DragItem(
        const std::string& _id,
        const std::string& _textureName,
        const NiVec4& _color,
        const NiVec2& _position,
        const NiVec2& _size,
        const NiGui_StandardPoint _anchor = NiGui_StandardPoint::LeftTop,
        const NiGui_StandardPoint _pivot = NiGui_StandardPoint::LeftTop
    );

    static std::string DragItem(const NiGui_Arg_DragItem& _setting);

    static void ResetItemToArea();
    static void ResetDragItemOffsets();
    static void SetItemToArea(const std::string& _itemID, const std::string& _areaID);

    // 座標自動計算を有効にする
    static void EnableAutoPosition() { state_.flags.autoPosition = true; };
    // 座標自動計算を無効にする
    static void DisableAutoPosition() { state_.flags.autoPosition = false; };

    
public: /// セッター
    static void SetDrawer(INiGuiDrawer* _drawer) { drawer_ = _drawer; }
    static void SetDebug(INiGuiDebug* _debug) { debug_ = _debug; }
    static void SetWindowInfo(const NiVec2& _size, const NiVec2& _leftTop) { size_ = _size; leftTop_ = _leftTop; }
    static void SetClientSize(const NiVec2& _size) { io_.windowInfo.clientSize = _size; }
    static void SetHoverSound(uint32_t _hoverSE) { io_.audioHnd.buttonHover = _hoverSE; }
    static void SetHoverSound(void* _hoverSE) { io_.audioHandler.buttonHover = _hoverSE; }
    static void SetConfirmSound(uint32_t _confirmSE) { io_.audioHnd.buttonConfirm = _confirmSE; }
    static void SetConfirmSound(void* _confirmSE) { io_.audioHandler.buttonConfirm = _confirmSE; }


public: /// ゲッター
    static const NiGuiIO& GetIO() { return io_; }
    static const NiGuiCoreState& GetState() { return state_; }
    static NiGuiSetting& GetSetting() { return setting_; }
    static const NiGuiStyle& GetStyle() { return style_; }

    static const std::string& GetActiveComponentID() { return state_.componentID.active; }
    static const std::string& GetHoverComponentID() { return state_.componentID.hover; }
    static const std::string& GetActiveComponentType() { return state_.componentID.typeActive; }
    static const std::string& GetHoverComponentType() { return state_.componentID.typeHover; }

    static uint32_t GetActiveTime() { return state_.time.active; }
    static uint32_t GetHoverTime() { return state_.time.hover; }


private: /// メンバ変数

    // 確認用フラグ

    static NiGuiIO              io_;
    static NiGuiCoreState       state_;
    static NiGuiSetting         setting_;
    static NiGuiStyle           style_;

    // 入力データ
    static NiGui_Input          input_;

    // ウィンドウのサイズ
    static NiVec2               leftTop_;
    static NiVec2               size_;

    // 描画クラス
    static INiGuiDrawer*        drawer_;

    // デバッグクラス
    static INiGuiDebug*         debug_;

    // エンドユーザーが変更したデータ
    static std::unordered_map<std::string, NiVec2> divOffset_;
    static std::unordered_map<std::string, NiVec2> dragItemOffset_;


private: /// 挙動
    static bool ButtonBehavior(const std::string& _id, const NiGui_InputState& _inputState, bool& _out_held);
    static void DivBehavior(const std::string& _id, bool _isHover, bool _isTrigger);
    static std::string DragItemAreaBehavior(const std::string& _id, bool _isHover, bool _isTrigger, NiGui_Transform2dEx& _transform, const NiVec2& _leftTop);
    static std::string DragItemBehavior(const std::string& _id, const NiGui_InputState& _inputState, NiGui_Transform2dEx& _transform, const NiVec2& originLeftTop);


private: /// 描画クラスにデータを送る関数
    static void ButtonDataEnqueue();
    static void DivDataEnqueue();
    static void DragItemAreaDataEnqueue();
    static void DragItemDataEnqueue();


private: /// ボタンの処理
    static void PostProcessComponents();
    static void PlaySE(const std::string& _type, uint32_t _hoverSE, uint32_t _confirmSE, void* _hoverHandler, void* _confirmHandler);


private: /// その他
    static void CheckValid_BeginFrame();
    static void CheckValid_DrawUI();
    static void JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, NiGui_InputState& _istate);
    static NiVec2 ComputeStandardPoint(NiGui_StandardPoint _stdpoint);
    static NiVec2 ComputeLeftTop(const NiVec2& _position, const NiVec2& _size, const NiVec2& _parentSize, NiGui_StandardPoint _anchor, NiGui_StandardPoint _pivot);
    // 各種座標を取得
    static void ComputeRect(NiGui_Transform2dEx& _transformForDraw, NiGui_Transform2dEx& _transformForJudge, const NiGui_StandardPoint _anchor, const NiGui_StandardPoint _pivot);
    static void ClearData();
    static void SavePreData();
    static void CopyInputData();
    static void ClampRect(NiVec2& _leftTop, const NiVec2& _size, const NiVec2& _parentPos, const NiVec2& _parentSize);
    static void OffsetUpdate(const std::string& _id, const NiVec2& _originLeftTop, const NiGui_Transform2dEx& _transform, NiVec2& _offset);
    static void SetComponentId(const NiGui_InputState& _inputState, const std::string& _id, const std::string& _type);


public:
    static const NiVec4 WHITE;
    static const NiVec4 BLACK;
    static const NiVec4 RED;
    static const NiVec4 GREEN;
    static const NiVec4 BLUE;
    static const NiVec4 YELLOW;
    static const NiVec4 CIAN;
    static const NiVec4 MAGENTA;
};