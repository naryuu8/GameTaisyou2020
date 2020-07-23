// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleResultUI.h"
#include "../InputManager.h"
#include "../SoundManager.h"

void UBattleResultUI::NextSelectState()
{
	SelectNumber++;
	if (SelectNumber > static_cast<int>(BattleResultState::STAGESELECT))
	{
		SelectNumber = static_cast<int>(BattleResultState::RESTART);
	}
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	ImageSizeChenge();
}

void UBattleResultUI::BackSelectState()
{
	SelectNumber--;
	if (SelectNumber < static_cast<int>(BattleResultState::RESTART))
	{
		SelectNumber = static_cast<int>(BattleResultState::STAGESELECT);
	}
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	ImageSizeChenge();
}

void UBattleResultUI::SelectStateAction()
{
	switch (SelectNumber)
	{
		case static_cast<int>(BattleResultState::RESTART) :
			RetryScene();
			ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
			break;
			case static_cast<int>(BattleResultState::STAGESELECT) :
				ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
				StageSelectChenge();
				break;
	}
}
