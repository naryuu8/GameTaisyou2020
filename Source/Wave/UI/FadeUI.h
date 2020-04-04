// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/Color.h"
#include "FadeUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UFadeUI : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
private:
	FLinearColor FadeColor;
	bool IsFadeIn;
	bool IsEnable;//Fade中ならtrue
	float FadeSpeed;
public:
	UFadeUI(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	FORCEINLINE FLinearColor GetFadeColor() const { return FadeColor; }
	//フェード状態にする
	//第一引数 フェード色
	//第二引数 trueでフェードイン、falseでフェードアウト
	//第三引数 フェード速度
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void SetFade(const FLinearColor fade_color,const bool fade_in,const float fade_speed)
	{
		FadeColor = fade_color;
		IsFadeIn = fade_in;
		IsEnable = true;
		FadeSpeed = fade_speed;
		if (IsFadeIn)
		{
			FadeColor.A = 0.0f;
		}
		else
		{
			FadeColor.A = 1.0f;
		}
	}
};
