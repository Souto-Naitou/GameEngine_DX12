#pragma once

#include <functional>
#include <list>
#include <string>
#include <tuple>
#include <Core/DirectX12/DirectX12.h>
#include <Features/Text/Text.h>

#include <Timer/Timer.h>
#include <array>

class DebugManager
{
public:
    static DebugManager* GetInstance();

    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;
    DebugManager(DebugManager&&) = delete;
    DebugManager& operator=(const DebugManager&&) = delete;

    /// <summary>
    /// デバッグ用コンポーネントの登録
    /// </summary>
    /// <param name="_strID">タブに表示される名前</param>
    /// <param name="_component">関数ポインタ。std::bindを使用することがほとんど</param>
    void SetComponent(std::string& _strID, const std::function<void(void)>& _component)
    {
        componentList_.push_back(std::make_tuple(std::string("null-name"), std::ref(_strID), _component, false));
    }

    /// <summary>
    /// デバッグ用コンポーネントの登録 (リスト用)
    /// </summary>
    /// <param name="_parentID">オブジェクトの種類</param>
    /// <param name="_childID">オブジェクトの名前</param>
    /// <param name="_component">関数ポインタ。std::bindを使用することがほとんど</param>
    void SetComponent(std::string _parentID, const std::string& _childID, const std::function<void(void)>& _component)
    {
        for (auto& comp : componentList_)
        {
            if (std::get<0>(comp) == _parentID && std::get<1>(comp) == _childID)
            {
                return;
            }
        }

        componentList_.emplace(
            GetInsertIterator(_parentID),
            _parentID,
            _childID,
            _component,
            false
        );
    }

    void DeleteComponent(const char* _strID);
    void DeleteComponent(const char* _parentID, const char* _childID);

    void Update();
    void DrawUI();
    void ChangeFont();
    void EnableDocking();
    void DefaultStyle();
    void SetDisplay(bool _isEnable) { onDisplay_ = _isEnable; }
    bool GetDisplay() const { return onDisplay_; }

    double GetFPS() const { return fps_; }

    void PushLog(const std::string& _log)
    {
        textLog_ += _log;
        OutputDebugStringA(_log.c_str());
    }
    void PhotoshopStyle();
    void RoundedVisualStudioStyle();

private:
    DebugManager();
    ~DebugManager();

    std::list<std::tuple<std::string, const std::string&, const std::function<void(void)>, bool>> componentList_;
    Timer                   timer_ = {};
    Timer                   frameTimer_ = {};
    double                  elapsedFrameCount_ = 0.0;
    double                  fps_ = 0.0;
    std::array<float, 120>  fpsList_ = {};
    unsigned int            frameCount_ = 0u;
    std::string             textLog_ = {};
    double                  frameTime_ = 0.0;

    bool                    onDisplay_ = true;
    bool                    enableAutoScroll_ = true;
    bool                    isExistSettingFile_ = false;

private: /// 借 り 物
    DirectX12* pDX12_ = nullptr;

private:
    void MeasureFPS();
    void MeasureFrameTime();
    std::list<std::tuple<std::string, const std::string&, const std::function<void(void)>, bool>>::iterator
        GetInsertIterator(std::string _parentName);


private: /// Windows
    void OverlayFPS() const;
    void Window_ObjectList();
    void DebugInfoWindow();
    void ShowDockSpace();
    void DrawGameWindow();
    void DebugInfoBar() const;
};