// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.


#include "EasingManager.h"
#include "../Easing.h"


#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


void EasingManager::SetEasing(Easing* _easing)
{
	pEasings.push_back(_easing);
}

void EasingManager::DeleteEasing(Easing* _target)
{
	pEasings.remove_if([_target](Easing* _ptr)
		{
			return _ptr == _target;
		}
	);
}

Easing* EasingManager::GetEasing(std::string _ID)
{
	for (Easing* ease : pEasings)
	{
		if (ease->GetID().compare(_ID) == 0)
		{
			return ease;
		}
	}
	return nullptr;
}


void EasingManager::DrawUI()
{
	#ifdef _DEBUG

	if (isFirstFrame_)
	{
		// 最初のフレームのみ呼び出しされる
		ImGui::SetNextWindowSize(ImVec2(500, 600));
		isFirstFrame_ = false;
	}

    if (!isDisplay_) return;

	ImGui::Begin("Easing Parameters", nullptr);
	ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersH;
	if (ImGui::BeginTable("## table", 5, tableFlags))
	{
		ImGui::TableSetupColumn("ID");
		ImGui::TableSetupColumn("Easing Type");
		ImGui::TableSetupColumn("Value");
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (Easing* easing : pEasings)
		{
			easing->ImGuiComponent();
		}

		ImGui::EndTable();
	}

	ImGui::End();

	#endif // _DEBUG
}