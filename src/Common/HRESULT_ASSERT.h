#pragma once

#include <Windows.h>

class HRESULT_ASSERT
{
public:
    HRESULT_ASSERT operator=(const HRESULT& _lValue);
    HRESULT_ASSERT operator=(const HRESULT&& _rValue);
private:
    HRESULT hr_;
};