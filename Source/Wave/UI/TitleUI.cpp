// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUI.h"
#include "../SoundManager.h"

void UTitleUI::NextSelectState()
{
	SelectNumber++;
	if (SelectNumber > static_cast<int>(TitleSelectState::GAME_END))
	{
		SelectNumber = static_cast<int>(TitleSelectState::GAME_START);
	}
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	ImageSizeChenge();
}

void UTitleUI::BackSelectState()
{
	SelectNumber--;
	if (SelectNumber < static_cast<int>(TitleSelectState::GAME_START))
	{
		SelectNumber = static_cast<int>(TitleSelectState::GAME_END);
	}
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	ImageSizeChenge();
}
