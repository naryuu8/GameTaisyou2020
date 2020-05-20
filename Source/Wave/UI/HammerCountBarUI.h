// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HammerCountBarUI.generated.h"

/**
 * 
 */
UCLASS()
class WAVE_API UHammerCountBarUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float MaxHammerHP;
	UPROPERTY(BlueprintReadWrite, Category = "C++Class")
		float DefaultHpSizeX;
public:
	//åªç›ÇÃHPÇéÛÇØéÊÇËÉQÅ[ÉWÇå∏ÇÁÇ∑
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateBar(const float HammerHP);
	void SetMaxHammerHP(const float hp) { MaxHammerHP = hp; }
};
