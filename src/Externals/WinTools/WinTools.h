#pragma once

#include <string>

class WinTools
{
public:
    WinTools(WinTools&) = delete;
    WinTools& operator=(const WinTools&) = delete;
    WinTools(WinTools&&) = delete;
    WinTools& operator=(WinTools&&) = delete;

    static WinTools* GetInstance()
    {
        static WinTools instance;
        return &instance;
    }

    std::string OpenFileDialog();

private:
    WinTools();
    ~WinTools();

};