// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultUI.h"

void UResultUI::NativeConstruct()
{
	IsPlayAnimation = false;
	ClearPlayAnimation();
	State = ResultState::RESULT_ANIME;
}