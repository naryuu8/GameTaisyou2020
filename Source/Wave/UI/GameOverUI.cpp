// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"

FSlateColor UGameOverUI::SelectTextColor(const GameOverState state)
{
	if (SelectNumber == static_cast<int>(state))
	{
		return FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	return FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

ESlateVisibility UGameOverUI::GetGameOverTextVisibility()
{
	if (IsPlayAnimation)
	{
		return ESlateVisibility::Hidden;
	}
	return ESlateVisibility::Visible;
}

void UGameOverUI::NextSelectState()
{
	if (IsPlayAnimation)return;
	SelectNumber++;
	if (SelectNumber > static_cast<int>(GameOverState::STAGESELECT))
	{
		SelectNumber = static_cast<int>(GameOverState::RESTART);
	}
}

void UGameOverUI::BackSelectState()
{
	if (IsPlayAnimation)return;
	SelectNumber--;
	if (SelectNumber < 0)
	{
		SelectNumber = static_cast<int>(GameOverState::STAGESELECT);
	}
}

void UGameOverUI::SelectStateAction()
{
	if (IsPlayAnimation)return;
	switch (SelectNumber)
	{
		case static_cast<int>(GameOverState::RESTART) :
			EndPlayAnimation();
				break;
			case static_cast<int>(GameOverState::STAGESELECT) :
				break;
	}
}
