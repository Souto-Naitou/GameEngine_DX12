#include "HRESULT_ASSERT.h"

#include <cassert>

HRESULT_ASSERT HRESULT_ASSERT::operator=(const HRESULT& _lValue)
{
    hr_ = _lValue;
    if (FAILED(hr_))
    {
        assert(false && "HRESULT ERROR");
    }
    return *this;
}

HRESULT_ASSERT HRESULT_ASSERT::operator=(const HRESULT&& _rValue)
{
    hr_ = _rValue;
    if (FAILED(hr_))
    {
        assert(false && "HRESULT ERROR");
    }
    return *this;
}
