// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultUI.h"
#include "../InputManager.h"
#include "../SoundManager.h"

void UResultUI::NativeConstruct()
{
	IsPlayAnimation = false;
	ClearPlayAnimation();
	State = ResultState::RESULT_ANIME;
}

void UResultUI::NextSelectState()
{
	if (State != ResultState::INPUT)return;
	if (!IsDataTable)
	{
		SelectNumber++;
		if (SelectNumber > static_cast<int>(ResultSelectState2::STAGESELECT))
		{
			SelectNumber = static_cast<int>(ResultSelectState2::RESTART);
		}
		ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
		ImageSizeChenge();
		return;
	}
	SelectNumber++;
	if (SelectNumber > static_cast<int>(ResultSelectState2::STAGESELECT))
	{
		SelectNumber = static_cast<int>(ResultSelectState2::NEXTSTAGE);
	}
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	ImageSizeChenge();
}

void UResultUI::BackSelectState()
{
	if (State != ResultState::INPUT)return;
	if (!IsDataTable)
	{
		SelectNumber--;
		if (SelectNumber < static_cast<int>(ResultSelectState2::RESTART))
		{
			SelectNumber = static_cast<int>(ResultSelectState2::STAGESELECT);
		}
		ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
		ImageSizeChenge();
		return;
	}
	SelectNumber--;
	if (SelectNumber < static_cast<int>(ResultSelectState2::NEXTSTAGE))
	{
		SelectNumber = static_cast<int>(ResultSelectState2::STAGESELECT);
	}
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	ImageSizeChenge();
}

void UResultUI::SelectStateAction()
{
	if (State != ResultState::INPUT)return;
	//次のステージがなかったら次のステージへを出さないこと
	switch (SelectNumber)
	{
		case static_cast<int>(ResultSelectState2::NEXTSTAGE) :
			NextStageChenge();
			ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
			break;
			case static_cast<int>(ResultSelectState2::RESTART) :
				Retry();
				ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
				break;
			case static_cast<int>(ResultSelectState2::STAGESELECT) :
				ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
				StageSelectChenge();
				break;
	}
	State = ResultState::END;
}

void  UResultUI::SetStageSelectState()
{
	SelectNumber = static_cast<int>(ResultSelectState2::STAGESELECT);
	ImageSizeChenge();
}