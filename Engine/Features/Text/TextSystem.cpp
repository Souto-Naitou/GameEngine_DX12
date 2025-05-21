#include "TextSystem.h"

#include <Utility/ConvertString/ConvertString.h>
#include <cassert>

void TextSystem::Initialize()
{
    /// インスタンスの取得
    pDirectX12_ = DirectX12::GetInstance();
    d2dFactory_ = pDirectX12_->GetD2D1Factory();
    dxgiDevice_ = pDirectX12_->GetDXGIDevice();
    d2dDevice_ = pDirectX12_->GetDirect2dDevice();
    d2dDeviceContext_ = pDirectX12_->GetDirect2dDeviceContext();
    d3d11On12Device_ = pDirectX12_->GetD3D11On12Device();
    d3d11On12DeviceContext_ = pDirectX12_->GetD3D11On12DeviceContext();

    /// D2D1デバイスの生成
    this->CreateDirectWriteFactory();

    /// カラーブラシの生成
    this->SetColorBrush("Black", D2D1::ColorF(D2D1::ColorF::Black));
    this->SetColorBrush("White", D2D1::ColorF(D2D1::ColorF::White));
    this->SetColorBrush("Red", D2D1::ColorF(D2D1::ColorF::Red));
    this->SetColorBrush("Green", D2D1::ColorF(D2D1::ColorF::Green));
    this->SetColorBrush("Blue", D2D1::ColorF(D2D1::ColorF::Blue));
    this->SetColorBrush("Yellow", D2D1::ColorF(D2D1::ColorF::Yellow));
    this->SetColorBrush("Cyan", D2D1::ColorF(D2D1::ColorF::Cyan));
}

void TextSystem::PresentDraw()
{
    const auto backBufferIndex = pDirectX12_->GetBackBufferIndex();
    const auto wrappedBackBuffer = pDirectX12_->GetD3D11WrappedBackBuffer(backBufferIndex);
    const auto backBufferForD2D = pDirectX12_->GetD2D1RenderTarget(backBufferIndex);
    d3d11On12Device_->AcquireWrappedResources(&wrappedBackBuffer, 1);
    d2dDeviceContext_->SetTarget(backBufferForD2D);
    d2dDeviceContext_->BeginDraw();
    d2dDeviceContext_->SetTransform(D2D1::Matrix3x2F::Identity());
}

void TextSystem::PostDraw()
{
    const auto backBufferIndex = pDirectX12_->GetBackBufferIndex();
    const auto wrappedBackBuffer = pDirectX12_->GetD3D11WrappedBackBuffer(backBufferIndex);
    d2dDeviceContext_->EndDraw();
    d3d11On12Device_->ReleaseWrappedResources(&wrappedBackBuffer, 1);
    d3d11On12DeviceContext_->Flush();
}

void TextSystem::SetColorBrush(const std::string _key, const D2D1::ColorF& _color)
{
    if (colorBrushes_.find(_key) != colorBrushes_.end())
    {
        return;
    }

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> colorBrush;
    d2dDeviceContext_->CreateSolidColorBrush(_color, colorBrush.GetAddressOf());
    colorBrushes_[_key] = colorBrush;
}

IDWriteTextFormat* TextSystem::GetTextFormat(const std::string& _fontFamily, float _fontSize)
{
    std::pair<std::string, float> key = std::make_pair(_fontFamily, _fontSize);

    if (textFormats_.find(key) == textFormats_.end())
    {
        Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;

        
        HRESULT hr = dwriteFactory_->CreateTextFormat(
            ConvertString(_fontFamily).c_str(),
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            _fontSize,
            L"ja-jp",
            textFormat.GetAddressOf()
        );

        if (FAILED(hr)) [[unlikely]]
        {
            assert(false && "フォントの生成に失敗");
        }

        textFormats_[key] = textFormat;
    }

    return textFormats_[key].Get();
}

ID2D1SolidColorBrush* TextSystem::GetColorBrush(const std::string& _key)
{
    if (colorBrushes_.find(_key) == colorBrushes_.end()) [[unlikely]]
    {
        assert(false && "指定したキーが存在しません");
    }

    return colorBrushes_[_key].Get();
}

void TextSystem::CreateDirectWriteFactory()
{
    /// DirectWriteのファクトリを生成
    hr_ = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory7), reinterpret_cast<IUnknown**>(dwriteFactory_.GetAddressOf()));
    assert(SUCCEEDED(hr_));
}
