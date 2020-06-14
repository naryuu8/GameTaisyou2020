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
	bool IsSceneFade;//フェード終了時にシーン遷移するならtrue
	float FadeSpeed;	
	FName NextLevelName;// 次のレベルの名前
public:
	UFadeUI(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	FORCEINLINE FLinearColor GetFadeColor() const { return FadeColor; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	FORCEINLINE bool GetFadeIsEnable() const { return IsEnable; }
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void CatFadeEvent(const bool fade_in, const float fade_speed);
	//フェード状態にする
	//第一引数 フェード色
	//第二引数 trueでフェードイン、falseでフェードアウト
	//第三引数 フェード速度
	//第四引数 猫のシルエットフェードを出すならtrue
	//FLinearColorはFLinearColor::Blackのように呼び出すこともできる
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void SetFade(const FLinearColor fade_color,const bool fade_in,const float fade_speed,const bool IsCatFade)
	{
		FadeColor = fade_color;
		IsFadeIn = fade_in;
		IsEnable = true;
		IsSceneFade = false;
		FadeSpeed = 1.0f / fade_speed;
		if (IsFadeIn)
		{
			FadeColor.A = 0.0f;
		}
		else
		{
			FadeColor.A = 1.0f;
		}
		if (IsCatFade)
		{
			CatFadeEvent(IsFadeIn, fade_speed);
		}
	}
	//フェードインにする
	//第一引数 フェード色
	//第二引数 フェード速度
	//第三引数 シーンの名前
	//第四引数 猫のシルエットフェードを出すならtrue
	UFUNCTION(BlueprintCallable, Category = "C++Library")
	void SetFadeLevel(const FLinearColor fade_color, const float fade_speed, const FName levelname, const bool IsCatFade)
	{
		FadeColor = fade_color;
		IsFadeIn = true;
		IsEnable = true;
		IsSceneFade = true;
		FadeSpeed = 1.0f / fade_speed;
		NextLevelName = levelname;
		if (IsFadeIn)
		{
			FadeColor.A = 0.0f;
		}
		else
		{
			FadeColor.A = 1.0f;
		}
		if (IsCatFade)
		{
			CatFadeEvent(IsFadeIn, fade_speed);
		}
	}
};
