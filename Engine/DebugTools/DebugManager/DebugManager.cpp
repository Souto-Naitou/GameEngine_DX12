#include "DebugTools/DebugManager/DebugManager.h"

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx12.h>
#endif // _DEBUG

#include <Core/DirectX12/SRVManager.h>

DebugManager* DebugManager::GetInstance()
{
    auto size = sizeof(DebugManager);
    size;

    static DebugManager instance; 
    return &instance;
}

DebugManager::DebugManager()
{
    pDX12_ = DirectX12::GetInstance();
}

DebugManager::~DebugManager()
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

    if (ImGui::Begin("Overray FPS", nullptr, window_flags))
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
#ifdef _DEBUG

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

#endif // _DEBUG
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
    ImGui::PushID("WindowObjectList");
    if (ImGui::Begin("Objects"))
    {
        for (auto itr = componentList_.begin(); itr != componentList_.end();)
        {
            if (std::get<0>(*itr) == "null-name")
            {
                ImGui::Selectable(std::get<1>(*itr).c_str(), &std::get<3>(*itr));
                itr++;
            }
            else // null-nameじゃないとき
            {
                std::string parentName = std::get<0>(*itr).c_str();
                if (ImGui::TreeNodeEx(parentName.c_str(), ImGuiTreeNodeFlags_Framed))
                {
                    do
                    {
                        //parentName = std::get<0>(*itr);
                        ImGui::Selectable(std::get<1>(*itr).c_str(), &std::get<3>(*itr));
                        itr++;
                        if (itr == componentList_.end()) break;
                    } while (std::get<0>(*itr) == parentName);

                    ImGui::TreePop();
                }
                else
                {
                    while (std::get<0>(*itr) == parentName)
                    {
                        itr++;
                        if (itr == componentList_.end()) break;
                    }
                }
            }
        }
    }
    ImGui::End();
    ImGui::PopID();
#endif // DEBUG
}

void DebugManager::DebugInfoWindow()
{
    #ifdef _DEBUG

    if(ImGui::Begin("DebugInfo"))
    {
        ImGui::PlotLines("FPS", fpsList_.data(), static_cast<int>(fpsList_.size()), 0, nullptr, 0.0f, 60.0f, ImVec2(0, 80));
    }
    ImGui::End();

    #endif // _DEBUG
}


std::list<std::tuple<std::string, const std::string&, const std::function<void(void)>, bool>>::iterator
    DebugManager::GetInsertIterator(std::string _parentName)
{
    auto resultItr = std::find_if(
        componentList_.begin(),
        componentList_.end(),
        [_parentName](std::tuple<std::string, const std::string&, const std::function<void(void)>, bool> arg) {
        if (std::get<0>(arg).compare(_parentName) == 0)
        {
            return true;
        }
        return false;
    });

    if (resultItr == componentList_.end())
    {
        //return std::prev(resultItr);
        return componentList_.begin();
    }

    do
    {
        resultItr++;
        if (resultItr == componentList_.end())
        {
            return std::prev(resultItr);
        }
    } while (std::get<0>(*resultItr) == _parentName);

    return resultItr;
}

void DebugManager::DeleteComponent(const char* _strID)
{
    componentList_.remove_if([_strID](const auto& component) {
        return std::get<0>(component).compare("null-name") == 0 &&
            std::get<1>(component).compare(_strID) == 0;
    });
}

void DebugManager::DeleteComponent(const char* _parentID, const char* _childID)
{
    componentList_.remove_if([_parentID, _childID](const auto& component) {
        return std::get<0>(component).compare(_parentID) == 0 &&
            std::get<1>(component).compare(_childID) == 0;
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

    DebugInfoWindow();

    // 登録されていないなら早期リターン
    if (componentList_.size() == 0) return;

    ImGui::PushID("DEBUG_MANAGER");

    Window_ObjectList();

    // デバッグウィンドウ描画
    ImGui::Begin("デバッグ");

    ImGuiTabBarFlags tabFlag = {};
    tabFlag |= ImGuiTabBarFlags_Reorderable;
    tabFlag |= ImGuiTabBarFlags_FittingPolicyResizeDown;
    tabFlag |= ImGuiTabBarFlags_TabListPopupButton;

    ImGui::BeginTabBar("## TABBAR", tabFlag);
    for (auto& component : componentList_)
    {
        // componentを展開 (参照)
        auto& [parentID, childID, pFunc, enableTab] = component;

        if (enableTab)
        {
            std::string tabName;
            if (parentID.compare("null-name") == 0) tabName = childID;
            else tabName = parentID + '_' + childID;

            std::string id_str = tabName + "TABITEM";
            ImGui::PushID(id_str.c_str());
            if (enableTab && parentID.compare("#Window") == 0 && ImGui::Begin(childID.c_str(), &enableTab))
            {
                pFunc();
                ImGui::End();
            }
            else if (enableTab && ImGui::BeginTabItem(tabName.c_str(), &enableTab))
            {
                ImGui::Text(parentID.c_str());
                ImGui::TextDisabled("Name: %s", childID.c_str());
                ImGui::Separator();
                pFunc();
                ImGui::EndTabItem();
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

void DebugManager::EnableDocking()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif
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

void DebugManager::DefaultStyle()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.TabRounding = 0.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.03f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.8f, 0.1f, 0.1f, 0.75f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.90f);
#endif // _DEBUG
}

void DebugManager::DrawGameWindow()
{
#ifdef _DEBUG

    uint32_t srvIndex = pDX12_->GetGameWndSRVIndex();
    auto gpuHnd = SRVManager::GetInstance()->GetGPUDescriptorHandle(srvIndex);

    auto vp = pDX12_->GetViewport();

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

    if ( ImGui::Begin("DebugInfoBar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar) )
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

void DebugManager::PhotoshopStyle()
{
#ifdef _DEBUG
    // Photoshop style by Derydoca from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 4.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 2.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.FrameRounding = 2.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 13.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabMinSize = 7.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 1.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.2627451121807098f, 0.2627451121807098f, 0.2627451121807098f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1560000032186508f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.3910000026226044f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.2627451121807098f, 0.2627451121807098f, 0.2627451121807098f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.6700000166893005f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.5843137502670288f, 0.5843137502670288f, 0.5843137502670288f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.1560000032186508f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5860000252723694f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5860000252723694f);
#endif // _DEBUG
}

void DebugManager::RoundedVisualStudioStyle()
{
#ifdef _DEBUG
    // Rounded Visual Studio style by RedNicStone from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.FrameRounding = 2.5f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 11.0f;
    style.ScrollbarRounding = 2.5f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 3.5f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5921568870544434f, 0.5921568870544434f, 0.5921568870544434f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
#endif // _DEBUG
}