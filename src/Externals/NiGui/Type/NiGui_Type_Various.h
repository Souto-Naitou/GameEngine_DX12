#pragma once

#include "../math/NiVec2.h" // NiVec2

struct NiGui_Transform2d
{
    NiVec2 position;
    NiVec2 size;
};

struct NiGui_Transform2dEx : public NiGui_Transform2d
{
    NiVec2 parentPos;
    NiVec2 parentSize;
};

struct NiGui_InputState
{
    bool isHover;
    bool isTrigger;
    bool isRelease;
};