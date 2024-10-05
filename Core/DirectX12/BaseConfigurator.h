#pragma once

template<typename T>
class BaseConfigurator
{
public:

    /// <summary>
    /// 変更を適用する
    /// </summary>
    /// <param name="_mode">モード</param>
    inline void Set(T& _data) const { data_ = _data; }

    inline T* Get() { return &data_; }

protected:
    T data_ = {};
};