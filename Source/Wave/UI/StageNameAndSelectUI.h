// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageNameAndSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UStageNameAndSelectUI : public UUserWidget
{
	GENERATED_BODY()
		int StageNumber;
public:
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		UTexture2D* GetTexture2D(const FName AssetPath);
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		FORCEINLINE int GetStageNumber() const { return StageNumber; }
	UFUNCTION(BlueprintCallable, Category = "C++Library")
		void SetStageNumber(const int number)  { StageNumber = number; }
};
