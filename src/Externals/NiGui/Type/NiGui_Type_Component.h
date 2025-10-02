#pragma once

// コンポーネントデータ
// コンポーネントのデータを保持する構造体です。
// この構造体はUIクラスの内部で使用されます。


#include "../math/NiVec2.h" // NiVec2
#include "../math/NiVec4.h" // NiVec4
#include "./NiGui_Enum.h" // enums

#include <string> // string

// フレックスコンテナのデータ
struct FlexContainerData
{
    NiVec2 leftTop;
    NiVec2 size;
    NiVec2 padding;
    NiVec2 margin;
    NiGui_Flex_Direction direction;
    NiGui_Flex_JustifyContent justifyContent;
    NiGui_Flex_AlignItems alignItems;
    NiGui_Flex_AlignContent alignContent;
};


struct BaseDrawData
{
    std::string id;
    std::string textureName;
    NiVec4 color;
    NiVec2 leftTop;
    NiVec2 texLeftTop;
    NiVec2 size;
    NiVec2 texSize;
    uint32_t zOrder;
};

// ボタンデータ
struct ButtonData : public BaseDrawData
{
};

struct BaseRegionData : public BaseDrawData
{
    BaseRegionData* parent = nullptr;
};

struct DivData : public BaseRegionData 
{
};

struct DragItemAreaData : public BaseRegionData
{
};

struct DragItemData : public BaseRegionData
{
};