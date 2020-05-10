// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HammerCountUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UHammerCountUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		int HammerCount = 0;
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	int GetHammerCount() { return HammerCount; };
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetHammerCount(const int count) { HammerCount = count; };
	void MinusHammerCount() { HammerCount--; if (HammerCount < 0) HammerCount = 0; };
	//カスタムイベント作成（BPで呼ぶので定義だけしておく）
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void MinusCountAnimation();
};
