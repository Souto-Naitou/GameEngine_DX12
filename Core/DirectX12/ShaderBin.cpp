#include "ShaderBin.h"

#include <Utility/Logger.h>
#include <Utility/ConvertString.h>
#include <format>
#include <cassert>

void ShaderBin::Configurator::Initialize()
{
    /// dxcCompilerを初期化
    HRESULT hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
    assert(SUCCEEDED(hr_) && "DirectX Shader Compilerの初期化に失敗しました [DxcUtils]");
    hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
    assert(SUCCEEDED(hr_) && "DirectX Shader Compilerの初期化に失敗しました [DxcCompiler]");

    /// 現時点でincludeはしないが、includeに対応するための設定を行っておく
    hr_ = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
    assert(SUCCEEDED(hr_) && "includeに対応する設定に失敗しました");

    /// ShaderをCompileする
    vertexShaderBlob_ = CompileShader(L"Resources/shaders/Object3D.VS.hlsl", L"vs_6_6", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
    assert(vertexShaderBlob_ != nullptr);

    pixelShaderBlob_ = CompileShader(L"Resources/shaders/Object3D.PS.hlsl", L"ps_6_6", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
    assert(pixelShaderBlob_ != nullptr);
}

Microsoft::WRL::ComPtr<IDxcBlob> ShaderBin::Configurator::CompileShader(
    const std::wstring& filePath,
    const wchar_t* profile,
    const Microsoft::WRL::ComPtr<IDxcUtils>& dxcUtils,
    const Microsoft::WRL::ComPtr<IDxcCompiler3>& dxcCompiler,
    const Microsoft::WRL::ComPtr<IDxcIncludeHandler>& includeHandler
)
{
    /// 1. hlslファイルを読み込む

    // これからシェーダーをコンパイルする旨をログに出す
    Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
    // hlslファイルを読む
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
    HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
    // 読めなかったら止める
    assert(SUCCEEDED(hr));
    // 読み込んだファイルの内容を設定する
    DxcBuffer shaderSourceBuffer;
    shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF-8の文字コードであることを通知

    /// 2. Compileする
    LPCWSTR arguments[] = {
        filePath.c_str(),			// コンパイル対象のhlslファイル名
        L"-E", L"main",				// エントリーポイントの指定。基本的にmain以外にはしない
        L"-T", profile,				// ShaderProfileの設定
        L"-Zi", L"-Qembed_debug",	// デバッグ用の情報を埋め込む
        L"-Od",						// 最適化を外しておく
        L"-Zpr",					// メモリレイアウトは行優先
    };
    // 実際にShaderをコンパイルする
    Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
    hr = dxcCompiler->Compile(
        &shaderSourceBuffer,		// 読み込んだファイル
        arguments,					// コンパイルオプション
        _countof(arguments),		// コンパイルオプションの数
        includeHandler.Get(),				// includeが含まれた諸々
        IID_PPV_ARGS(&shaderResult)	// コンパイル結果
    );
    // コンパイルエラーではなくdxcが起動できないなど致命的な状況
    assert(SUCCEEDED(hr));

    /// 3. 警告・エラーが出ていないか確認する
    Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0)
    {
        Log(shaderError->GetStringPointer());
        // 警告・エラーダメゼッタイ
        assert(false);
    }

    /// 4. Compile結果を受け取って返す

    // コンパイル結果から実行用のバイナリ部分を取得
    Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
    hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
    assert(SUCCEEDED(hr));
    // 成功したログを出す
    Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
    // 実行用のバイナリを返却
    return shaderBlob;
}