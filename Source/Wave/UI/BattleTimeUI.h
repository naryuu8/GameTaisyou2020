// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleTimeUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UBattleTimeUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateTime(const int time);
};
