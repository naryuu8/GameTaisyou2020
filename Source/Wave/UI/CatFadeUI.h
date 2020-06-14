// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../SoundManager.h"
#include "CatFadeUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UCatFadeUI : public UUserWidget
{
	GENERATED_BODY()
private:
	bool IsFadeIn = false;
	bool IsEnable = false;//Fade中ならtrue
	float FadeSpeed = 0.0f;
	float Scale = 0.0f;//シルエットの大きさ(大きいほど小さい）
public:
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetFadeIsEnable() const { return IsEnable; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsFadeIn() const { return IsFadeIn; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		float UpdateFadeTime(const float InDeltaTime);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void SetFade(const bool fade_in, const float fade_speed)
	{
		IsFadeIn = fade_in;
		IsEnable = true;

		FadeSpeed = 100.0f / fade_speed;
		if (IsFadeIn)
		{
			ASoundManager::SafePlaySound(SOUND_TYPE::FADE_IN);
			Scale = 0.0f;
		}
		else
		{
			Scale = 100.0f;
		}
	}
};
