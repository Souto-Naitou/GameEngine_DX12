#pragma once

#include "../Type/NiGui_Type_Core.h"

#include <string>
#include "../Type/NiGui_Type_Component.h"

class INiGuiDebug
{
public:
    INiGuiDebug() = default;
    virtual void DrawDebugUI() = 0;
    void SetIO(const NiGuiIO* _io) { io_ = _io; }
    void SetState(const NiGuiCoreState* _state) { state_ = _state; }
    void SetSetting(NiGuiSetting* _setting) { setting_ = _setting; }

protected:
    const NiGuiIO* io_ = nullptr;
    const NiGuiCoreState* state_ = nullptr;
    NiGuiSetting* setting_ = nullptr;

protected:
    bool GetComponentData(const std::string& _id, BaseDrawData& _out_data);
};