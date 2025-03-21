#pragma once

#include <Features/NiUI/Interface/NiUI_IDrawer.h>

#include <Features/Sprite/Sprite.h>
#include <list>
#include <memory>

class Drawer : public IDrawer
{
public:
    Drawer() = default;
    ~Drawer() = default;

    void DrawSetting() override;
    void Draw() override;

private:
    std::list<std::unique_ptr<Sprite>> sprites_;
};