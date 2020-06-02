// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseUI.h"
#include "Kismet/GameplayStatics.h"

void UPauseUI::NativeConstruct()
{
	IsPlayAnimation = false;
	IsNoInput = false;
	InitPlayAnimation();
	SelectNumber = static_cast<int>(PauseState::GAMEBACK);
	UGameplayStatics::SetGamePaused(GetWorld(),true);
}

FSlateColor UPauseUI::SelectTextColor(const PauseState state)
{
	if (SelectNumber == static_cast<int>(state))
	{
		return FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	return FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
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
	IsNoInput = true;
	EnterSE_Play();
	switch (SelectNumber)
	{
		case static_cast<int>(PauseState::GAMEBACK):
			EndPlayAnimation();
			break;
		case static_cast<int>(PauseState::RESTART) :
			Retry();
			break;
			case static_cast<int>(PauseState::SCORE) :
				IsNoInput = false;
			break;
			case static_cast<int>(PauseState::STAGESELECT) :
			StageSelectChenge();
			break;
	}
}
