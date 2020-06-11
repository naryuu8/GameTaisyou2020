// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimeUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UGameTimeUI : public UUserWidget
{
	GENERATED_BODY()
public:
	//カウントが0になったらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsCountZero = false;
	//時計の針が360度になったらtrue
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsTimeEnd = false;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float NowTimeAngle = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float NormaTimeAngle = 0.0f;
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpDateTime();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetTimeLimit(const int limit);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetCountDownTime(const int time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNormaTime(const int time);
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void AnimationPause();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void AnimationRePlay();
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsCountZero() const { return IsCountZero; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE bool GetIsTimeEnd() const { return IsTimeEnd; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetNowTimeAngle() const { return NowTimeAngle; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	float GetNormaTimeAngle() const { return NormaTimeAngle; }
};
