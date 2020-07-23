// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleTipsUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UTitleTipsUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		bool IsStickDraw = false;
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetNoDraw();
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void SetDraw();
	void SetIsStickDraw(const bool type) { IsStickDraw = type; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE	bool GetIsStickDraw() const { return IsStickDraw; }
};
