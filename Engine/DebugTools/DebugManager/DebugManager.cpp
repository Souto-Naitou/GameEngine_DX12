#include "DebugTools/DebugManager/DebugManager.h"

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx12.h>
#endif // _DEBUG

#include <Core/DirectX12/SRVManager.h>
#include <unordered_map>

DebugManager* DebugManager::GetInstance()
{
    static DebugManager instance;
    return &instance;
}

void DebugManager::PushLog(const std::string& _log)
{
    textLog_ += _log;
}

DebugManager::DebugManager()
{
}

DebugManager::~DebugManager()
{

}

void DebugManager::Window_Common()
{

}

void DebugManager::OverlayFPS() const
{
    #ifdef _DEBUG
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    const float PAD = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_size.x - PAD;
    window_pos.y = PAD;
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowBgAlpha(0.0f);

    if (ImGui::Begin(lang_dm_.window_overlayFPS.c_str(), nullptr, window_flags))
    {
        ImGui::Text("%.2lfFPS", fps_);
        ImGui::SameLine();
        ImGui::ProgressBar(static_cast<float>(fps_) / 60.0f, ImVec2(0, 0), "");

        auto wndSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2(work_size.x - wndSize.x - PAD, PAD));
    }
    ImGui::End();
    #endif // _DEBUG
}

void DebugManager::MeasureFPS()
{
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }
    /// フレームレート計算
    if (timer_.GetNow<double>() - elapsedFrameCount_ >= 0.1)
    {
        fps_ = frameCount_ * 1.0 / (timer_.GetNow<double>() - elapsedFrameCount_);
        frameCount_ = 0;
        elapsedFrameCount_ = timer_.GetNow<double>();
    }
    frameCount_++;

    std::rotate(fpsList_.begin(), fpsList_.begin() + 1, fpsList_.end());
    fpsList_.back() = static_cast<float>(fps_);
}

void DebugManager::MeasureFrameTime()
{
    #ifdef _DEBUG

    frameTime_ = frameTimer_.GetNow<double>();
    frameTimer_.Reset();
    frameTimer_.Start();

    #endif // _DEBUG
}

void DebugManager::Window_ObjectList()
{
    #ifdef _DEBUG

    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;

    std::list<ComponentData*> uncategorizedComponents;
    std::unordered_map<std::string, std::list<ComponentData*>> parentMap;

    for (auto& component : componentList_)
    {
        if (component.categoryId.has_value())
        {
            parentMap[component.categoryId.value()].push_back(&component);
        }
        else
        {
            uncategorizedComponents.push_back(&component);
        }
    }

    ImGui::PushID("WindowComponentList");
    if (ImGui::Begin(lang_dm_.window_componentList.c_str()))
    {
        // カテゴライズされたコンポーネント
        for (auto& [parentName, list] : parentMap)
        {
            if (ImGui::TreeNodeEx(parentName.c_str(), treeNodeFlags))
            {
                for (auto& component : list)
                {
                    std::string id = component->id_cpy;
                    if (component->id_ptr) id = *component->id_ptr;

                    ImGui::Selectable(id.c_str(), &component->isEnabled);
                }
                ImGui::TreePop();
            }
        }

        // 未カテゴリのコンポーネント
        for (auto& component : uncategorizedComponents)
        {
            std::string id = component->id_cpy;
            if (component->id_ptr) id = *component->id_ptr;

            if (ImGui::TreeNodeEx(lang_common_.uncategorized.c_str(), treeNodeFlags))
            {
                ImGui::Selectable(id.c_str(), &component->isEnabled);
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
    ImGui::PopID();
    #endif // DEBUG
}

void DebugManager::Window_DebugInfo()
{
    #ifdef _DEBUG

    if (ImGui::Begin(lang_dm_.window_Performance.c_str()))
    {
        ImGui::PlotLines("FPS", fpsList_.data(), static_cast<int>(fpsList_.size()), 0, nullptr, 0.0f, 60.0f, ImVec2(0, 80));
    }
    ImGui::End();

    #endif // _DEBUG
}

void DebugManager::SetComponent(const std::string& _name, const std::function<void(void)>& _component, bool isWindowMode)
{
    ComponentData data = {};
    data.id_ptr = &_name;
    data.id_cpy = _name;
    data.function = _component;
    data.isEnabled = false;
    data.isWindow = isWindowMode;
    componentList_.emplace_back(data);
}

void DebugManager::SetComponent(const std::string&& _name, const std::function<void(void)>& _component, bool isWindowMode)
{
    ComponentData data = {};
    data.id_ptr = nullptr;
    data.id_cpy = _name;
    data.function = _component;
    data.isEnabled = false;
    data.isWindow = isWindowMode;
    componentList_.emplace_back(data);
}

void DebugManager::SetComponent(const std::string& _category, const std::string& _name, const std::function<void(void)>& _component, bool isWindowMode)
{
    ComponentData data = {};
    data.categoryId = _category;
    data.id_ptr = &_name;
    data.id_cpy = _name;
    data.function = _component;
    data.isEnabled = false;
    data.isWindow = isWindowMode;
    componentList_.emplace_back(data);
}

void DebugManager::SetComponent(const std::string& _category, const std::string&& _name, const std::function<void(void)>& _component, bool isWindowMode)
{
    ComponentData data = {};
    data.categoryId = _category;
    data.id_ptr = nullptr;
    data.id_cpy = _name;
    data.function = _component;
    data.isEnabled = false;
    data.isWindow = isWindowMode;
    componentList_.emplace_back(data);
}

void DebugManager::DeleteComponent(const std::string& _name)
{
    componentList_.remove_if([_name](const ComponentData& component)
    {
        bool result = false;
        if (!component.categoryId.has_value())
        {
            std::string id = component.id_ptr ? *component.id_ptr : component.id_cpy;
            result = id.compare(_name) == 0;
        }
        return result;
    });
}

void DebugManager::DeleteComponent(const std::string& _categoryID, const std::string& _name)
{
    componentList_.remove_if([_categoryID, _name](const ComponentData& component)
    {
        bool result = false;
        if (component.categoryId.has_value())
        {
            std::string parentId = component.categoryId.value();
            std::string childId = component.id_ptr ? *component.id_ptr : component.id_cpy;
            result = parentId.compare(_categoryID) == 0 && childId.compare(_name) == 0;
        }
        return result;
    });
}

void DebugManager::Update()
{
    MeasureFrameTime();
    MeasureFPS();
}

void DebugManager::DrawUI()
{
    #ifdef _DEBUG

    if (!onDisplay_) return;

    ShowDockSpace();

    DebugInfoBar();

    Window_DebugInfo();

    // 登録されていないなら早期リターン
    if (componentList_.size() == 0) return;

    ImGui::PushID("DEBUG_MANAGER");

    Window_ObjectList();

    // デバッグウィンドウ描画
    ImGui::Begin(lang_dm_.window_debug.c_str());

    ImGuiTabBarFlags tabFlag = {};
    tabFlag |= ImGuiTabBarFlags_Reorderable;
    tabFlag |= ImGuiTabBarFlags_FittingPolicyResizeDown;
    tabFlag |= ImGuiTabBarFlags_TabListPopupButton;

    ImGui::BeginTabBar("## TABBAR", tabFlag);
    for (auto& component : componentList_)
    {
        bool isCategorized = component.categoryId.has_value();
        std::string id = component.id_ptr ? *component.id_ptr : component.id_cpy;
        std::string parentID = isCategorized ? component.categoryId.value() : lang_common_.uncategorized;

        if (component.isEnabled)
        {
            std::string tabName;
            if (isCategorized == false) tabName = id;
            else tabName = id + " - " + parentID;

            std::string id_str = tabName + "TABITEM";
            ImGui::PushID(id_str.c_str());
            {
                // ウィンドウモード
                if (component.isWindow && ImGui::Begin(id.c_str(), &component.isEnabled))
                {
                    component.function();
                    ImGui::End();
                }
                // デフォルトモード
                else if (ImGui::BeginTabItem(tabName.c_str(), &component.isEnabled))
                {
                    ImGui::Text((lang_common_.category + ": %s").c_str(), parentID.c_str());
                    ImGui::TextDisabled((lang_common_.name + ": %s").c_str(), id.c_str());
                    ImGui::Separator();
                    component.function();
                    ImGui::EndTabItem();
                }
            }
            ImGui::PopID();
        }
    }
    ImGui::EndTabBar();


    ImGui::End();

    ImGui::PopID();
    #endif // _DEBUG
}

void DebugManager::ChangeFont()
{
    #ifdef _DEBUG
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig fontcfg;
    fontcfg.MergeMode = 0;
    fontcfg.OversampleH = 1;
    fontcfg.PixelSnapH = 1;
    fontcfg.GlyphOffset = ImVec2(0.0f, 0.0f);

    //io.Fonts->AddFontDefault();

    ImFont* resultFont = io.Fonts->AddFontFromFileTTF(
        "EngineResources/Font/x12y16pxMaruMonica.ttf",
        16,
        &fontcfg,
        io.Fonts->GetGlyphRangesJapanese()
    );

    io.FontDefault = resultFont;

    io.Fonts->Build();
    ImGui_ImplDX12_CreateDeviceObjects();
    #endif // _DEBUG
}

void DebugManager::ShowDockSpace()
{
    #ifdef _DEBUG

    if (!(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
    {
        return;
    }

    auto vp = ImGui::GetMainViewport();

    ImGui::DockSpaceOverViewport(ImGui::GetID("Inspector"), vp, ImGuiDockNodeFlags_PassthruCentralNode);

    #endif // _DEBUG
}

void DebugManager::Window_GameScreen()
{
    #ifdef _DEBUG

    uint32_t srvIndex = pDx12_->GetGameWndSRVIndex();
    auto gpuHnd = SRVManager::GetInstance()->GetGPUDescriptorHandle(srvIndex);

    auto vp = pDx12_->GetViewport();

    uint32_t width = static_cast<uint32_t>(vp.Width);
    uint32_t height = static_cast<uint32_t>(vp.Height);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::Begin("GameWindow", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::Image((ImTextureID)gpuHnd.ptr, ImVec2(static_cast<float>(width), static_cast<float>(height)));
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    #endif // _DEBUG
}

void DebugManager::DebugInfoBar() const
{
    #ifdef _DEBUG

    if (ImGui::Begin("DebugInfoBar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::Text("%.2lfFPS", fps_);
        ImGui::SameLine();
        ImGui::ProgressBar(static_cast<float>(fps_) / 60.0f, ImVec2(200, 0), "");
        ImGui::SameLine();
        ImGui::Text("Update: %dms", static_cast<int>(frameTime_ * 1000));
    }
    ImGui::End();

    #endif // debug
}
