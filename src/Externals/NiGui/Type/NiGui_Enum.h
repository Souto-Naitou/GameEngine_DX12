#pragma once

// UI用列挙体
// UIクラスで使用する列挙体を定義します。
// この列挙体はUIクラスの内部で使用されます。


/// - - - - - 
/// レイアウト
/// - - - - - -

// アラインメント
enum class NiGui_StandardPoint
{
    LeftTop, // 左上
    CenterTop, // 上
    RightTop, // 右上
    LeftCenter, // 左
    Center, // 中央
    RightCenter, // 右
    LeftBottom, // 左下
    CenterBottom, // 下
    RightBottom, // 右下
};

// フレックスコンテナの方向
enum class NiGui_Flex_Direction
{
    Row, // 横
    Column, // 縦
};

// フレックスコンテナの配置
enum class NiGui_Flex_JustifyContent
{
    Start, // 開始
    End, // 終了
    Center, // 中央
    SpaceBetween, // 間隔を均等に
    SpaceAround, // 間隔を均等に
};

// フレックスコンテナのアラインメント
enum class NiGui_Flex_AlignItems
{
    Start, // 開始
    End, // 終了
    Center, // 中央
};

// フレックスアイテムを複数行にしたとき
enum class NiGui_Flex_AlignContent
{
    Start, // 開始
    End, // 終了
    Center, // 中央
    SpaceBetween, // 間隔を均等に
    SpaceAround, // 間隔を均等に
};


/// - - - - - - - - - - -
/// コンポーネント・ステート
/// - - - - - - - - - - -

// ボタンの状態
enum class NiGui_ButtonState
{
    None, // 何もない
    Hover, // ホバー
    Confirm, // 確定
};