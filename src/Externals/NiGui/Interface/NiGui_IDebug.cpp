#include "NiGui_IDebug.h"

bool INiGuiDebug::GetComponentData(const std::string& _id, BaseDrawData& _out_data)
{
    bool isfound = false;
    auto& drawData = state_->buffer.drawData;

    auto buttonItr = drawData.button.find(_id.c_str());
    isfound = isfound || (buttonItr != drawData.button.end());

    if (isfound)
    {
        _out_data = buttonItr->second;
        return true;
    }

    auto divItr = drawData.div.find(_id.c_str());
    isfound = isfound || (divItr != drawData.div.end());

    if (isfound)
    {
        _out_data = divItr->second;
        return true;
    }

    auto dragItemItr = drawData.dragItem.find(_id.c_str());
    isfound = isfound || (dragItemItr != drawData.dragItem.end());

    if (isfound)
    {
        _out_data = dragItemItr->second;
        return true;
    }

    auto dragItemAreaItr = drawData.dragItemArea.find(_id.c_str());
    isfound = isfound || (dragItemAreaItr != drawData.dragItemArea.end());

    if (isfound)
    {
        _out_data = dragItemAreaItr->second;
        return true;
    }

    // 見つからなかったら
    return false;
}
