// Fill out your copyright notice in the Description page of Project Settings.


#include "CatFadeUI.h"

float UCatFadeUI::UpdateFadeTime(const float InDeltaTime)
{
	if (!IsEnable)return Scale;
	//フェード中ならタイプに合わせてアルファ値を変える
	if (IsFadeIn)
	{
		Scale += InDeltaTime * FadeSpeed;
		if (Scale >= 100.0f)
		{
			IsEnable = false;
		}
	}
	else
	{
		Scale -= InDeltaTime * FadeSpeed;
		if (Scale <= 0.0f)
		{
			IsEnable = false;
			RemoveFromParent();
		}
	}
	return Scale;
}