// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeCountUI.h"

void UTimeCountUI::NativeDestruct()
{
	IsCountZero = false;
	TimeCount = 0;
}

FSlateColor UTimeCountUI::TimeTextColor(const int chenge_time)
{
	if (TimeCount <= chenge_time)
	{
		return FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
	return FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}