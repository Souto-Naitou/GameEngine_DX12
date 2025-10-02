// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.


#include "Easing.h"

#include "Manager/EasingManager.h"

#include <cassert>
#include <iostream>


#ifdef _DEBUG
#include <imgui.h>
#endif //_DEBUG

void (Easing::* Easing::easeFuncTable[])(float) =
{
	&Easing::EaseNone,

	&Easing::EaseInSine,
	&Easing::EaseOutSine,
	&Easing::EaseInOutSine,

	&Easing::EaseInQuad,
	&Easing::EaseOutQuad,
	&Easing::EaseInOutQuad,

	&Easing::EaseInCubic,
	&Easing::EaseOutCubic,
	&Easing::EaseInOutCubic,

	&Easing::EaseInQuart,
	&Easing::EaseOutQuart,
	&Easing::EaseInOutQuart,

	&Easing::EaseInBack,
	&Easing::EaseOutBack,
	&Easing::EaseInOutBack,

	&Easing::EaseInElastic,
	&Easing::EaseOutElastic,
	&Easing::EaseInOutElastic,

	&Easing::EaseInBounce,
	&Easing::EaseOutBounce,
	&Easing::EaseInOutBounce,
};


Easing::Easing(){
	timeBeginPeriod(1);
	QueryPerformanceFrequency(&mFreq_);
	EasingManager::GetInstance()->SetEasing(this);
	return;
}

Easing::Easing(std::string _ID): Easing(){
	easeType_ = EaseType::EaseInSine;
	useMode_ = UseMode::Sec;
	identifier_ = _ID;
	Initialize();
}

Easing::Easing(std::string _ID,EaseType _type): Easing(){
	easeType_ = _type;
	useMode_ = UseMode::Manual;
	identifier_ = _ID;
	Initialize();
	return;
}

Easing::Easing(std::string _ID,EaseType _type,double _secTarget): Easing(){
	easeType_ = _type;
	secTarget_ = _secTarget;
	useMode_ = UseMode::Sec;
	identifier_ = _ID;
	Initialize();
	return;
}

Easing::Easing(std::string _ID,EaseType _type,unsigned int _frameTarget): Easing(){
	easeType_ = _type;
	frameTarget_ = _frameTarget;
	useMode_ = UseMode::Frame;
	identifier_ = _ID;
	Initialize();
	return;
}

Easing::~Easing(){
	timeEndPeriod(1);
	EasingManager::GetInstance()->DeleteEasing(this);
	return;
}

void Easing::Initialize(){
	if(pCsvloaderInstance_){
		pCsvloaderInstance_->SaveFile();
	}
	pCsvloaderInstance_ = CSVLoader::GetInstance();
	if(pCsvloaderInstance_->LoadFile(fileName_.c_str())){
		pCsvLine_ = pCsvloaderInstance_->GetLine(fileName_.c_str(),identifier_);
		if(pCsvLine_) {
			(*pCsvLine_)[1].seekg(0);
			(*pCsvLine_)[1] >> selectedIndex_;
			easeType_ = static_cast<EaseType>(selectedIndex_);
			int inputUseMode = 0;
			(*pCsvLine_)[2].seekg(0);
			(*pCsvLine_)[2] >> inputUseMode;
			useMode_ = static_cast<UseMode>(inputUseMode);

			if(useMode_ == UseMode::Sec){
				(*pCsvLine_)[3].seekg(0);
				(*pCsvLine_)[3] >> secTarget_;
			} else if(useMode_ == UseMode::Frame){
				(*pCsvLine_)[3].seekg(0);
				(*pCsvLine_)[3] >> frameTarget_;
			}
			currentLinearT_ = 0;
			easedT_ = 0;
			countFrame_ = 0;
			isStart_ = false;
			isEnd_ = false;
		}
	}

}

float Easing::Update(){
	if(useMode_ == UseMode::Manual){
		assert(0 && L"マニュアルモードが選択されています。float型の引数を必要とするUpdateを使用してください。");
	}
	if(currentLinearT_ >= 1.0f && isSetMax_){
		currentLinearT_ = 1.0f;
		EasingExecution();
		isEnd_ = true;
		return easedT_;
	}

	/// 秒モード
	if(isStart_ && useMode_ == UseMode::Sec){
		LARGE_INTEGER mNow = {};
		QueryPerformanceCounter(&mNow);
		/// Start関数呼出してから初めてUpdateが呼び出されたとき
		if(mStart_.QuadPart == 0){
			QueryPerformanceCounter(&mStart_);
			mNow = mStart_;
		}
		elapsedTime_ = static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);

		currentLinearT_ = static_cast<float>(elapsedTime_ / secTarget_);
		if (currentLinearT_ > 1.0f) currentLinearT_ = 1.0f;
		EasingExecution();
	}
	/// フレームモード
	if(isStart_ && useMode_ == UseMode::Frame){
		currentLinearT_ = static_cast<float>(countFrame_) / static_cast<float>(frameTarget_);
		EasingExecution();
		countFrame_++;
	}

	return easedT_;
}

float Easing::Update(float _t){
	currentLinearT_ = _t;
	EasingExecution();
	return easedT_;
}

void Easing::Reset(){
	currentLinearT_ = 0.0f;
	easedT_ = 0.0f;
	elapsedTime_ = 0.0;
	mStart_ = LARGE_INTEGER();
	countFrame_ = 0u;
	isStart_ = false;
	isEnd_ = false;
}

#ifdef _DEBUG
void Easing::ImGuiComponent(){
	bool isChangeValue = false;

	ImGui::PushID(this);

	ImGui::TableNextRow();

	ImGui::TableNextColumn();
	ImGui::SetNextItemWidth(2000);
	ImGui::Text(identifier_.c_str());

	ImGui::TableNextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if(ImGui::Combo("## combo",&selectedIndex_,kEaseTypes_,6)){
		easeType_ = static_cast<EaseType>(selectedIndex_);
		isChangeValue = true;
	}

	ImGui::TableNextColumn();
	float secTarget = static_cast<float>(secTarget_);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if(ImGui::DragFloat("## secTarget",&secTarget,0.01f)){
		isChangeValue = true;
	}
	secTarget_ = static_cast<double>(secTarget);

	ImGui::TableNextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if(ImGui::Button("P")){
		Reset();
		isStart_ = true;
	}
	ImGui::TableNextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	if(ImGui::Button("R")) Reset();


	if(isChangeValue){
		// 変更されたら実行
		if(!pCsvLine_){
			pCsvLine_ = pCsvloaderInstance_->GetNewLine(fileName_);
			for(int i = 0; i < 4; i++) (*pCsvLine_).push_back({});
		}
		for(auto& word : *pCsvLine_){
			word.str("");
			word.clear();
		}
		(*pCsvLine_)[0] << identifier_;
		(*pCsvLine_)[1] << selectedIndex_;
		(*pCsvLine_)[2] << static_cast<int>(useMode_);
		if(useMode_ == UseMode::Sec){
			(*pCsvLine_)[3] << secTarget_;
		} else if(useMode_ == UseMode::Frame){
			(*pCsvLine_)[3] << frameTarget_;
		}
	}

	ImGui::PopID();
}
#endif //_DEBUG


void Easing::EasingExecution(){
	(this->*easeFuncTable[static_cast<unsigned int>(easeType_)])(currentLinearT_);
	return;
}