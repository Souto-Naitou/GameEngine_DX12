// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.


#ifndef EASING_H_
#define EASING_H_

#include <Features/CSV/CSVLoader.h>

#include <string>
#include <Windows.h>

class CSVLoader;

class Easing
{
public:
    /// <summary>
    /// モード
    /// </summary>
    enum class UseMode : int { Manual, Sec, Frame, COUNT };

    /// <summary>
    ///  イージング方式
    /// </summary>
    enum class EaseType : int
    {
        EaseNone,

        EaseInSine,
        EaseOutSine,
        EaseInOutSine,

        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,

        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,

        EaseInQuart,
        EaseOutQuart,
        EaseInOutQuart,

        EaseInBack,
        EaseOutBack,
        EaseInOutBack,

        EaseInElastic,
        EaseOutElastic,
        EaseInOutElastic,

        EaseInBounce,
        EaseOutBounce,
        EaseInOutBounce,

        COUNT
    };

    Easing();

    /// <summary>
    /// ファイルからパラメータをロードする。基本はこれ使おう。
    /// </summary>
    /// <param name="_ID"></param>
    Easing(std::string _ID);

    /// <summary>
    /// マニュアルモード。SetTを用いて操作する
    /// </summary>
    /// <param name="_type">イージングの種類</param>
    Easing(std::string _ID, EaseType _type);

    /// <summary>
    /// 秒モード。Startメンバ関数実行からn秒後に1.0fとなる
    /// </summary>
    /// <param name="_type">イージングの種類</param>
    /// <param name="_secTarget">目標時間</param>
    Easing(std::string _ID, EaseType _type, double _secTarget);

    /// <summary>
    /// フレームモード。Startメンバ関数実行からnフレーム後に1.0fとなる
    /// </summary>
    /// <param name="_type">イージングの種類</param>
    /// <param name="_frameTarget">目標フレーム</param>
    Easing(std::string _ID, EaseType _type, unsigned int _frameTarget);

    ~Easing();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新とイージング後のタイムを取得
    /// </summary>
    /// <returns>easedT</returns>
    float           Update();

    /// <summary>
    /// マニュアル用の更新とイージング後のタイムを取得
    /// </summary>
    /// <param name="_t">イージング前のt</param>
    /// <returns>easedT</returns>
    float           Update(float _t);

    /// <summary>
    /// イージングを開始(マニュアルモード時は不要)
    /// </summary>
    void            Start() { isStart_ = true; };

    /// <summary>
    /// Start前の状態に戻す
    /// </summary>
    void            Reset();

    /// 現在のcurrentLinearTを取得
    float           GetCurrentT()       const   { return currentLinearT_; }
    /// 現在の経過時間を取得
    double          GetElapsedTime()    const   { return elapsedTime_; }
    /// 現在のフレーム回数を取得
    unsigned int    GetCountFrame()     const   { return countFrame_; }
    /// IDの取得
    std::string     GetID()             const   { return identifier_; }

    bool            GetIsEnd()          const   { return isEnd_; }

    /// 1.0fで止めるかどうか (初期値:true)
    void            SetIsSetMax(bool _flag) { isSetMax_ = _flag; }

    void            SetUseMode(UseMode _useMode) { useMode_ = _useMode; }

    /// デバッグウィンドウ
    void            ImGuiComponent();


private:
    const std::string   fileName_           = "Easing.csv";                 // !< 書き込み先ファイル名
    const char          kEaseTypes_[290]    = "EaseNone\0EaseInSine\0EaseOutSine\0EaseInOutSine\0EaseInQuad\0EaseOutQuad\0EaseInOutQuad\0EaseInCubic\0EaseOutCubic\0EaseInOutCubic\0EaseInQuart\0EaseOutQuart\0EaseInOutQuart\0EaseInBack\0EaseOutBack\0EaseInOutBack\0EaseInElastic\0EaseOutElastic\0EaseInOutElastic\0EaseInBounce\0EaseOutBounce\0EaseInOutBounce\0";
    int                 selectedIndex_      = 0;

    EaseType            easeType_           = EaseType::COUNT;              // !< イージング方式
    UseMode             useMode_            = UseMode::COUNT;               // !< 使用モード

    double              secTarget_          = 0.0;                          // !< ターゲットタイム(秒)
    unsigned int        frameTarget_        = 0u;                           // !< ターゲットタイム(フレーム)

    float               currentLinearT_     = 0.0f;                         // !< 現在の線形t
    float               easedT_             = 0.0f;                         // !< イージング後のt
    double              elapsedTime_        = 0.0;                          // !< 経過時間
    LARGE_INTEGER       mFreq_              = {};                           // !< フリケンシ
    LARGE_INTEGER       mStart_             = {};                           // !< 開始時のフリケンシ
    unsigned int        countFrame_         = 0u;                           // !< フレーム回数

    bool                isEnd_              = false;                        // 終了フラグ (current == 1.0f)
    bool                isStart_            = false;                        // !< 開始フラグ
    bool                isSetMax_           = true;                         // !< tが1.0fのときに終了させるかどうか
    std::string         identifier_         = {};                           // !< 識別子
    CSVLoader*          pCsvloaderInstance_ = nullptr;                      // !< CSVローダーのインスタンス
    CSVLine*            pCsvLine_           = nullptr;                      // !< csvの１行分
    CSVLine             csvLine_            = {};

    static void (Easing::* easeFuncTable[])(float);


    /// 関数
    void EasingExecution();

    /// イージング関数郡
    void EaseNone(float _t);

    void EaseInSine(float _t);
    void EaseOutSine(float _t);
    void EaseInOutSine(float _t);

    void EaseInQuad(float _t);
    void EaseOutQuad(float _t);
    void EaseInOutQuad(float _t);

    void EaseInCubic(float _t);
    void EaseOutCubic(float _t);
    void EaseInOutCubic(float _t);

    void EaseInQuart(float _t);
    void EaseOutQuart(float _t);
    void EaseInOutQuart(float _t);

    void EaseInBack(float _t);
    void EaseOutBack(float _t);
    void EaseInOutBack(float _t);

    void EaseInElastic(float _t);
    void EaseOutElastic(float _t);
    void EaseInOutElastic(float _t);

    void EaseInBounce(float _t);
    void EaseOutBounce(float _t);
    void EaseInOutBounce(float _t);
};


#endif // EASING_H_