// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StampUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UStampUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void StampPlayAnimation();
};
