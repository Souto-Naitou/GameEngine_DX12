#include "NiGui_IDrawer.h"
#include <stdexcept>
#include <NiGui.h>

void INiGuiDrawer::DrawSetting()
{
    if (drawData_.empty()) return;

    /// スプライトリストのリサイズ
    drawDataZOrdered_.resize(drawData_.size());

    for (auto& data : drawData_)
    {
        if (data->zOrder >= drawDataZOrdered_.size())
        {
            throw std::runtime_error("zOrderが大きすぎます。");
        }

        drawDataZOrdered_[data->zOrder] = data;
    }

    ApplyButtonHoverAndClickEffects();
}

void INiGuiDrawer::PostDraw()
{
    /// 描画データのクリア
    drawDataZOrdered_.clear();
    drawData_.clear();
}

void INiGuiDrawer::ApplyButtonHoverAndClickEffects()
{
    /// UIクラスからデータを取得
    auto hoveredComponentID = NiGui::GetHoverComponentID();
    auto activeComponentID = NiGui::GetActiveComponentID();
    auto hoverType = NiGui::GetHoverComponentType();
    auto activeType = NiGui::GetActiveComponentType();

    for (auto& data : drawDataZOrdered_)
    {
        if (data->id == hoveredComponentID && hoverType == "Button")
        {
            data->color.Lerp(data->color, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.5f);
        }

        if (data->id == activeComponentID && activeType == "Button")
        {
            data->color.Lerp(data->color, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.5f);
        }
    }
}