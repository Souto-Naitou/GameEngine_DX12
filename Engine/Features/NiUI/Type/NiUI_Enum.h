﻿#pragma once

// UI用列挙体
// UIクラスで使用する列挙体を定義します。
// この列挙体はUIクラスの内部で使用されます。


/// - - - - - 
/// レイアウト
/// - - - - - -


// フレックスコンテナの方向
enum class NiUI_Flex_Direction
{
    Row, // 横
    Column, // 縦
};

// フレックスコンテナの配置
enum class NiUI_Flex_JustifyContent
{
    Start, // 開始
    End, // 終了
    Center, // 中央
    SpaceBetween, // 間隔を均等に
    SpaceAround, // 間隔を均等に
};

// フレックスコンテナのアラインメント
enum class NiUI_Flex_AlignItems
{
    Start, // 開始
    End, // 終了
    Center, // 中央
};

// フレックスアイテムを複数行にしたとき
enum class NiUI_Flex_AlignContent
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
enum class NiUI_ButtonState
{
    None, // 何もない
    Hover, // ホバー
    Press, // 押下
};