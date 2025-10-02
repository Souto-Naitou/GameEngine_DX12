#pragma once

#include "../Math/NiVec2.h"
#include "../Math/NiVec4.h"
#include "./NiGui_Enum.h"

#include <string>

/// コンポーネント呼出関数の引数に使用する型の定義
/// 引数に必要なデータをラップした構造体


struct NiGui_Arg_Button
{
    std::string id;             // コンポーネントのID
    std::string textureName;    // テクスチャ名
    NiVec4 color;               // 色
    NiVec2 position;            // 左上座標
    NiVec2 size;                // サイズ
    NiVec2 texSize;             // テクスチャサイズ
    NiGui_StandardPoint anchor; // アンカー
    NiGui_StandardPoint pivot;  // ピボット
};

struct NiGui_Arg_Div
{
    std::string id;             // コンポーネントのID
    std::string textureName;    // テクスチャ名
    NiVec4 color;               // 色
    NiVec2 position;            // 左上座標
    NiVec2 size;                // サイズ
    NiGui_StandardPoint anchor; // アンカー
    NiGui_StandardPoint pivot;  // ピボット
};

struct NiGui_Arg_DragItemArea
{
    std::string id;             // コンポーネントのID
    std::string textureName;    // テクスチャ名
    NiVec4 color;               // 色
    NiVec2 position;            // 左上座標
    NiVec2 size;                // サイズ
    NiGui_StandardPoint anchor; // アンカー
    NiGui_StandardPoint pivot;  // ピボット
};

struct NiGui_Arg_DragItem
{
    std::string id;             // コンポーネントのID
    std::string textureName;    // テクスチャ名
    NiVec4 color;               // 色
    NiVec2 position;            // 左上座標
    NiVec2 size;                // サイズ
    NiGui_StandardPoint anchor; // アンカー
    NiGui_StandardPoint pivot;  // ピボット
};