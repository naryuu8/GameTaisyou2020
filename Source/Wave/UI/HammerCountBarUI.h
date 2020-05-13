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
	//現在のHPを受け取りゲージを減らす
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void UpdateBar(const float HammerHP);
	//現在の溜めている力に応じて消費するゲージ量を表示
	UFUNCTION(Category = "C++Event", BlueprintImplementableEvent, BlueprintCallable)
		void NowUpdateBar(const float Power);
	void SetMaxHammerHP(const float hp) { MaxHammerHP = hp; }
};
