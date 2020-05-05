// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
	float SelectNumber;

};
