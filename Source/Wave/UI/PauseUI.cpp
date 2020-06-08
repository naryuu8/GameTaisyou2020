// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseUI.h"
#include "Kismet/GameplayStatics.h"
#include "../Object/GameController.h"
#include "../SoundManager.h"

void UPauseUI::NativeConstruct()
{
	IsPlayAnimation = false;
	IsNoInput = false;
	InitPlayAnimation();
	SelectNumber = static_cast<int>(PauseSelectState::GAMEBACK);
	UGameplayStatics::SetGamePaused(GetWorld(),true);
}

ESlateVisibility UPauseUI::GetPauseTextVisibility()
{
	if (IsPlayAnimation)
	{
		return ESlateVisibility::Hidden;
	}
	return ESlateVisibility::Visible;
}

void UPauseUI::NextSelectState()
{
	if (IsPlayAnimation)return;
	if (IsNoInput)return;
	SelectNumber++;
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	if (SelectNumber > static_cast<int>(PauseSelectState::STAGESELECT))
	{
		SelectNumber = static_cast<int>(PauseSelectState::GAMEBACK);
	}
	ImageSizeChenge();
}

void UPauseUI::BackSelectState()
{
	if (IsPlayAnimation)return;
	if (IsNoInput)return;
	SelectNumber--;
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_SELECT);
	if (SelectNumber < 0)
	{
		SelectNumber = static_cast<int>(PauseSelectState::STAGESELECT);
	}
	ImageSizeChenge();
}

void UPauseUI::SelectStateAction()
{
	if (IsPlayAnimation)return;
	if (IsNoInput)return;
	IsNoInput = true;
	switch (SelectNumber)
	{
		case static_cast<int>(PauseSelectState::GAMEBACK):
		{
			EndAnimation();
			break;
		}
		case static_cast<int>(PauseSelectState::RESTART) :
			ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
			Retry();
			break;
			case static_cast<int>(PauseSelectState::STAGESELECT) :
			ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
			StageSelectChenge();
			break;
	}
}

void UPauseUI::EndAnimation()
{
	ASoundManager::SafePlaySound(SOUND_TYPE::MENU_DECISION);
	EndPlayAnimation();
	AGameController* game = Cast<AGameController>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameController::StaticClass()));
	if (game)
	{
		game->SetTimeCountRePlay();
	}
}