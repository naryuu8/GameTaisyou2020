// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseUI.h"
#include "Kismet/GameplayStatics.h"
#define MAX_PAUSENUMBER (3)

void UPauseUI::NativeConstruct()
{
	IsPlayAnimation = false;
	InitPlayAnimation();
	SelectNumber = 0;
	UGameplayStatics::SetGamePaused(GetWorld(),true);
}

FSlateColor UPauseUI::SelectTextColor(const int number)
{
	if (SelectNumber == number)
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

void UPauseUI::AddSelectNumber()
{
	if (IsPlayAnimation)return;
	SelectNumber++;
	if (SelectNumber > MAX_PAUSENUMBER)
	{
		SelectNumber = 0;
	}
}

void UPauseUI::MinusSelectNumber()
{
	if (IsPlayAnimation)return;
	SelectNumber--;
	if (SelectNumber < 0)
	{
		SelectNumber = MAX_PAUSENUMBER;
	}
}
