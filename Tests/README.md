# GoogleTest セットアップ

このプロジェクトにGoogleTestを導入しました。

## 構成

```
Tests/
├── Tests.vcxproj          # テストプロジェクトファイル
├── Tests.vcxproj.filters  # Visual Studioフィルター
├── main.cpp               # テストメイン関数
├── tests/
│   └── sample_test.cpp    # サンプルテスト
└── Bin/                   # ビルド出力ディレクトリ
```

## セットアップ手順

1. **GoogleTestライブラリのビルド**:
   ```bash
   .\build_gtest.bat
   ```

2. **テストプロジェクトのビルド**:
   - Visual Studio: `Engine/GameEngine_DX12.sln` を開いて `Tests` プロジェクトをビルド
   - コマンドライン: `MSBuild Tests\Tests.vcxproj /p:Configuration=Debug /p:Platform=x64`

3. **テストの実行**:
   ```bash
   .\run_tests.bat
   ```
   または
   ```bash
   .\Tests\Bin\Debug\Tests.exe
   ```

## テストの追加方法

新しいテストファイルを `Tests/tests/` ディレクトリに作成し、`Tests.vcxproj` ファイルに追加してください。

例：
```cpp
#include <gtest/gtest.h>

TEST(新しいテストスイート, テスト名) {
    EXPECT_EQ(1 + 1, 2);
}
```

## エンジンクラスのテスト

エンジンのクラスをテストする場合は、以下の手順でプロジェクト設定を更新してください：

1. `Tests.vcxproj` にエンジンのインクルードパスを追加
2. エンジンライブラリ（Engine.lib）への参照を追加
3. プロジェクト依存関係にEngineプロジェクトを追加

## 使用可能なGoogleTest機能

- **基本アサーション**: `EXPECT_EQ`, `EXPECT_NE`, `EXPECT_TRUE`, `EXPECT_FALSE`
- **浮動小数点**: `EXPECT_FLOAT_EQ`, `EXPECT_NEAR`
- **文字列**: `EXPECT_STREQ`, `EXPECT_STRCASEEQ`
- **例外**: `EXPECT_THROW`, `EXPECT_NO_THROW`
- **Google Mock**: モックオブジェクトのサポート

詳細については[GoogleTest公式ドキュメント](https://google.github.io/googletest/)を参照してください。
