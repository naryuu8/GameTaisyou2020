// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseUI.h"
#include "Kismet/GameplayStatics.h"
#include "../Object/GameController.h"

void UPauseUI::NativeConstruct()
{
	IsPlayAnimation = false;
	IsNoInput = false;
	IsScoreCheck = false;
	InitPlayAnimation();
	SelectNumber = static_cast<int>(PauseState::GAMEBACK);
	UGameplayStatics::SetGamePaused(GetWorld(),true);
}

ESlateVisibility UPauseUI::GetPauseTextVisibility()
{
	if (IsPlayAnimation || IsScoreCheck)
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
	SelectSE_Play();
	if (SelectNumber > static_cast<int>(PauseState::STAGESELECT))
	{
		SelectNumber = static_cast<int>(PauseState::GAMEBACK);
	}
}

void UPauseUI::BackSelectState()
{
	if (IsPlayAnimation)return;
	if (IsNoInput)return;
	SelectNumber--;
	SelectSE_Play();
	if (SelectNumber < 0)
	{
		SelectNumber = static_cast<int>(PauseState::STAGESELECT);
	}
}

void UPauseUI::SelectStateAction()
{
	if (IsPlayAnimation)return;
	if (IsNoInput)return;
	if (IsScoreCheck)
	{
		IsScoreCheck = false;
		return;
	}
	IsNoInput = true;
	EnterSE_Play();
	switch (SelectNumber)
	{
		case static_cast<int>(PauseState::GAMEBACK):
		{
			EndPlayAnimation();
			AGameController* game = Cast<AGameController>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameController::StaticClass()));
			if (game)
			{
				game->SetTimeCountRePlay();
			}
			break;
		}
		case static_cast<int>(PauseState::RESTART) :
			Retry();
			break;
			case static_cast<int>(PauseState::SCORE) :
				IsScoreCheck = true;
				IsNoInput = false;
			break;
			case static_cast<int>(PauseState::STAGESELECT) :
			StageSelectChenge();
			break;
	}
}
